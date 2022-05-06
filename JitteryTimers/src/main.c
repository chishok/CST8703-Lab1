#define _POSIX_C_SOURCE 200809L
#include <unistd.h>

#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ac_args.h"
#include "ac_file.h"
#include "ac_timing.h"

#ifndef AC_DATA_MAX_SIZE
/**
 * @brief Max number of data points to write to file.
 */
#define AC_DATA_MAX_SIZE 100000
#endif

/**
 * @brief Get string literal of scheduling policy.
 *
 * @param policy Scheduling policy.
 * @return Readable string (literal).
 */
static const char *ac_thread_policy_string(const int policy)
{
    const char *result = "";
    switch (policy)
    {
        case SCHED_RR:
        {
            result = "SCHED_RR";
            break;
        }
        case SCHED_FIFO:
        {
            result = "SCHED_FIFO";
            break;
        }
        case SCHED_OTHER:
        {
            result = "SCHED_OTHER";
            break;
        }
        default:
        {
            result = "UNKNOWN";
            break;
        }
    }
    return result;
}

/**
 * @brief Set real-time thread priority with Round-Robin scheduling.
 *
 * @param priority Thread priority.
 */
static void ro_thread_set_rt_print_info(const int priority)
{
    // set realtime scheduler priority
    pthread_t thread_id = pthread_self();
    int policy = SCHED_RR;
    struct sched_param param = {.sched_priority = priority};

    /*
     * STUDENT WORK SECTION BEGIN
     * ==========================
     */

    // 1. Set and get the current thread priority
    // 2. if there is an error returned by a pthread_* function set 'errno' and print using 'perror'
    //    More info on errno: https://man7.org/linux/man-pages/man3/errno.3.html
    //    Note: Many POSIX functions (especially pthread_*) don't typically set errno,
    //          but return non-zero integer which corresponds to a standard error code
    // -------------------------------------------------------------------------------

    // Useful functions (check man pages):
    // -----------------------------------
    // pthread_setschedparam
    // pthread_getschedparam
    // perror

    /*
     * ========================
     * STUDENT WORK SECTION END
     */

    char timestamp[30] = {'\0'};
    const size_t timestamp_size = 30;
    result = ac_timing_get_timestamp(timestamp, timestamp_size);
    if (result != 0)
    {
        perror("get_timestamp");
    }

    printf("\nProcess info:\n");
    printf("    timestamp: %s\n", timestamp);
    printf("    process id: %d\n", getpid());
    printf("    thread policy: %s\n", ac_thread_policy_string(policy));
    printf("    thread priority: %d\n", param.sched_priority);
    printf("    thread id: %lu\n", thread_id);
}

int main(int argc, char *argv[])
{
    // parse arguments
    AcArgs args = ac_args_create();
    ac_args_parse(argc, argv, &args);
    printf("Arguments parsed\n");
    printf("    step_size (us): %d\n", args.step_size);
    printf("    duration (s): %.2f\n", args.duration);
    printf("    filename: %s\n", args.filename);
    printf("    priority: %d\n", args.priority);
    ac_args_validate(&args);

    // data to save
    uint32_t data_usec[AC_DATA_MAX_SIZE] = {0};
    const size_t max_data_size = AC_DATA_MAX_SIZE;

    /*
     * STUDENT WORK SECTION BEGIN
     * ==========================
     */

    // 1. Get number of steps to execute based on step_size and duration,
    // without exceeding max_data_size
    // -------------------------------
    size_t execution_steps = 0;

    printf("Number of execution steps: %zu\n\n", execution_steps);

    /*
     * ========================
     * STUDENT WORK SECTION END
     */

    // set priority and print process info
    ro_thread_set_rt_print_info(args.priority);

    // create timer
    AcTimer timer = ac_timer_create();

    // flush streams
    fflush(NULL);

    // start timed process
    ac_timer_start(&timer, args.step_size);
    for (size_t idx = 0; idx < execution_steps; ++idx)
    {
        /*
         * STUDENT WORK SECTION BEGIN
         * ==========================
         */

        // 1. Wait (sleep) until next target time to start execution of timed loop
        // -----------------------------------------------------------------------

        /*
         * ========================
         * STUDENT WORK SECTION END
         */

        // Timed loop real-time processing happens here

        // save result to data log
        data_usec[idx] = ac_timer_get_elapsed_usec(&timer);
    }

    // write to file
    const size_t write_bytes = execution_steps * sizeof(data_usec[0]);
    if (ac_file_write(args.filename, data_usec, write_bytes) != 0)
    {
        perror("Failed to write data file");
        return EXIT_FAILURE;
    }
    printf("Number of bytes written to file '%s': %zu\n", args.filename, write_bytes);

    return EXIT_SUCCESS;
}
