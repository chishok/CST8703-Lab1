#define _POSIX_C_SOURCE 200809L
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include "ac_args.h"

AcArgs ac_args_create()
{
    return (AcArgs){.priority = 90, .step_size = 1000, .duration = 5.0, .filename = {"data/timeLog.dat"}};
}

void ac_args_parse(int argc, char *argv[], AcArgs *args)
{
    const char *options = "p:s:d:f:";
    opterr = 0;
    int opt = getopt(argc, argv, options);
    while (opt != -1)
    {
        switch (opt)
        {
            case 'p':
            {
                char *ptr = NULL;
                args->priority = strtol(optarg, &ptr, 10);
                break;
            }
            case 's':
            {
                char *ptr = NULL;
                args->step_size = strtol(optarg, &ptr, 10);
                break;
            }
            case 'd':
            {
                char *ptr = NULL;
                args->duration = strtod(optarg, &ptr);
                break;
            }
            case 'f':
            {
                int retval = snprintf(args->filename, AC_FILE_MAX_LEN, "%s", optarg);
                if (retval <= 0 || retval == AC_FILE_MAX_LEN)
                {
                    fprintf(stderr, "Failed to retrieve filename from option `-%c'\n", optopt);
                    exit(EXIT_FAILURE);
                }
                break;
            }
            case '?':
            {
                if (optopt == 'c')
                {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                }
                else
                {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                }
                exit(EXIT_FAILURE);
            }
            default:
            {
                fprintf(stderr, "Usage: %s [-p PRIORITY] [-s STEP_SIZE] [-d DURATION] [-f FILENAME]\n", argv[0]);
                exit(EXIT_FAILURE);
            }
        }
        opt = getopt(argc, argv, options);
    }
}

void ac_args_validate(const AcArgs *args)
{
    const double max_duration = 60.0;
    const double step_size = 1.0e-6 * (double)(args->step_size);
    if (args->step_size <= 0)
    {
        fprintf(stderr, "Step size (%d us) must be greater than 0\n", args->step_size);
        exit(EXIT_FAILURE);
    }
    else if (args->duration < step_size)
    {
        fprintf(stderr, "Duration (%.2f s) must be greater or equal to step size (%.2e s)\n", args->duration,
                step_size);
        exit(EXIT_FAILURE);
    }
    else if (args->duration > max_duration)
    {
        fprintf(stderr, "Duration (%.1f s) must be less than %.1f s\n", args->duration, max_duration);
        exit(EXIT_FAILURE);
    }
    else if (args->priority < 0 || args->priority > 98)
    {
        fprintf(stderr, "Priority (%d) must be in range [0..98]\n", args->priority);
        exit(EXIT_FAILURE);
    }
}
