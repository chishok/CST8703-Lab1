#define _POSIX_C_SOURCE 200809L
#include <unistd.h>

#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "ac_file.h"

int ac_file_write(const char *restrict path, const void *restrict data, const size_t data_size)
{
    FILE *fptr = NULL;

    size_t len = strnlen(path, AC_FILE_MAX_LEN);
    if (len == 0 || len == AC_FILE_MAX_LEN) {
        return -1;
    }

    fptr = fopen(path, "w");
    if (fptr == NULL) {
        return -1;
    }

    if (fwrite(data, data_size, 1, fptr) != 1)
    {
        fclose(fptr);
        return -1;
    }

    if (fclose(fptr) != 0)
    {
        return -1;
    }

    return 0;
}
