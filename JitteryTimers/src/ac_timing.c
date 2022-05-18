#define _POSIX_C_SOURCE 200809L
#include <unistd.h>

#include <errno.h>

#include "ac_timing.h"

AcTimer ac_timer_create()
{
    return (AcTimer){.step_size = {.tv_sec = 0, .tv_nsec = 0},
                     .start = {.tv_sec = 0, .tv_nsec = 0},
                     .target = {.tv_sec = 0, .tv_nsec = 0},
                     .elapsed = {.tv_sec = 0, .tv_nsec = 0}};
}

int ac_timer_start(AcTimer *timer, const uint32_t step_size_usec)
{
    int result = 0;
    if (timer == NULL)
    {
        result = EINVAL;
        errno = result;
    }
    else
    {
        // get start time
        result = clock_gettime(CLOCK_MONOTONIC, &timer->start);
        if (result == 0)
        {
            // get system time
            result = clock_gettime(CLOCK_REALTIME, &timer->real);
            // save start time as target
            timer->target = timer->start;
            timer->elapsed.tv_sec = 0;
            timer->elapsed.tv_nsec = 0;
            // step size
            timer->step_size.tv_sec = step_size_usec / AC_MEGA;
            timer->step_size.tv_nsec = (step_size_usec - (timer->step_size.tv_sec * AC_MEGA)) * AC_KILO;
        }
    }
    return result;
}

int ac_timer_advance_and_wait(AcTimer *timer)
{
    int result = 0;
    if (timer == NULL)
    {
        result = EINVAL;
        errno = result;
    }
    else
    {
        /*
         * STUDENT WORK SECTION BEGIN
         * ==========================
         */

        // 1. Advance target by adding step size to target 'timer->step_size' to `timer->target`
        // CAREFUL: timer->target.tv_nsec must always be within 0 and 1e9
        // --------------------------------------------------------------

        // 2. Sleep to target timer->target using clock_nanosleep
        // result = clock_nanosleep(...);
        // ------------------------------

        if (result == 0)
        {
            // get current monotonic time and system time after sleep
            result = clock_gettime(CLOCK_MONOTONIC, &timer->elapsed);
            clock_gettime(CLOCK_REALTIME, &timer->real);

            // 3. Substract timer->start from timer->elapsed to get elapsed time
            // CAREFUL: timer->elapsed.tv_nsec must always be within 0 and 1e9
            // -----------------------------------------------------------------
        }

        /*
         * STUDENT WORK SECTION END
         * ========================
         */
    }
    return result;
}

uint64_t ac_timer_get_elapsed_usec(AcTimer *timer)
{
    return (uint64_t)(timer->elapsed.tv_sec * AC_MEGA) + (uint64_t)(timer->elapsed.tv_nsec / AC_KILO);
}

int ac_timing_get_timestamp(char *restrict timestamp, const size_t size)
{
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    size_t len = strftime(timestamp, size, "%Y-%m-%d %H:%M:%S", &timeinfo);
    if (len == 0)
    {
        errno = EMSGSIZE;
    }
    return (len == 0 ? EMSGSIZE : 0);
}

int ac_timing_delay_seconds(double duration)
{
    // add time
    struct timespec ts;
    ts.tv_sec = (time_t)duration;
    ts.tv_nsec = (long)((duration - ((double)ts.tv_sec)) * AC_GIGA);
    return clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL);
}
