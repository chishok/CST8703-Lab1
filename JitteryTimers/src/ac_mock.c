#define _POSIX_C_SOURCE 200809L
#include <unistd.h>

#include <time.h>
#include <stdlib.h>

#include "ac_mock.h"
#include "ac_timing.h"

int ac_mock_delay(unsigned int *seed, const double min_duration, const double max_duration)
{
    int delay = rand_r(seed);
    double delay_seconds = min_duration + (max_duration - min_duration) * ((double) delay) / ((double) RAND_MAX);
    return ac_timing_delay_seconds(delay_seconds);
}
