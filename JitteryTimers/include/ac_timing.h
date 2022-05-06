
#ifndef AC_TIMER_H
#define AC_TIMER_H

#include <time.h>
#include <stdint.h>

#define AC_KILO 1000
#define AC_MEGA 1000000
#define AC_GIGA 1000000000

/**
 * @brief Timer with realtime monotonic clock
 */
typedef struct AcTimer
{
    /**
     * @brief Interval timer step size.
     *
     */
    struct timespec step_size;
    /**
     * @brief Start of interval timer.
     *
     */
    struct timespec start;
    /**
     * @brief Target time to begin next iteration of loop.
     *
     */
    struct timespec target;
    /**
     * @brief Time elapsed since start of timed loop.
     *
     */
    struct timespec elapsed;
} AcTimer;

/**
 * @brief Create timer.
 *
 * @return Interval timer with default values.
 */
AcTimer ac_timer_create();

/**
 * @brief Start timer with specified step size.
 *
 * @param timer Timer object.
 * @param step_size_usec Step size in microseconds
 * @return 0 on success, error code otherwise.
 */
int ac_timer_start(AcTimer *timer, const uint32_t step_size_usec);

/**
 * @brief Advance target time by step size and sleep until target.
 *
 * @param timer Timer object.
 * @return  0 on success, error code otherwise.
 */
int ac_timer_advance_and_wait(AcTimer *timer);

/**
 * @brief Get elapsed time since start in microseconds.
 *
 * @param timer Timer object.
 * @return Elapsed time in microseconds.
 */
uint64_t ac_timer_get_elapsed_usec(AcTimer *timer);

/**
 * @brief Get timestamp string.
 *
 * @param timestamp Resulting string containing timestamp on output.
 * @param size Size of timestamp string available for write.
 * @return 0 on success, error code otherwise.
 */
int ac_timing_get_timestamp(char *restrict timestamp, const size_t size);

#endif
