
#ifndef AC_MOCK_SENSOR_H
#define AC_MOCK_SENSOR_H

/**
 * @brief Sleep for duration
 *
 * @param seed         Random number generator seed
 * @param min_duration Minimum duration in seconds
 * @param max_duration Maximum duration in seconds
 * @return             Return value of clock_nanosleep
 */
int ac_mock_delay(unsigned int *seed, double min_duration, double max_duration);

#endif
