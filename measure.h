#ifndef PARALLEL_RADIX_SORT_MEASURE_H
#define PARALLEL_RADIX_SORT_MEASURE_H

#include "parallel_radix_sort.h"
#include <stdarg.h>
#include <sys/time.h>
#include <cstdio>

struct __bench__ {
    double start;
    char msg[100];
    __bench__(const char* format, ...)
    __attribute__((format(printf, 2, 3)))
    {
        va_list args;
        va_start(args, format);
        vsnprintf(msg, sizeof(msg), format, args);
        va_end(args);

        start = sec();
    }
    ~__bench__() {
        fprintf(stderr, "%s: %.6f sec\n", msg, sec() - start);
    }
    double sec() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec + tv.tv_usec * 1e-6;
    }
    operator bool() { return false; }
};

#define benchmark(...) if (__bench__ __b__ = __bench__(__VA_ARGS__)); else

#endif //PARALLEL_RADIX_SORT_MEASURE_H
