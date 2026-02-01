#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int64_t
timespec_to_ns(struct timespec t)
{
    return (int64_t)t.tv_sec * 1'000'000'000LL + t.tv_nsec;
}

static int64_t
monotonic_now()
{
    struct timespec t = {};
    (void)clock_gettime(CLOCK_MONOTONIC, &t);
    return timespec_to_ns(t);
}

int
main(int argc, char *argv[])
{
    constexpr size_t memset_size = (1ULL<<30) * 10; // 10 gigas per memset

    int memset_iterations = 1;
    if (argc >= 2) {
        memset_iterations = atoi(argv[1]);
    }

    // The following code is not optimized out when compiling the
    // benchmark without optimizations, and both malloc and memset
    // themselves lives in libc and are optimized which is what we want
    // to measure.

    void* mem = malloc(memset_size);

    const int64_t t0 = monotonic_now();

    for (int i = 0; i < memset_iterations; i++) {
      memset(mem, 0, memset_size);
    }

    const int64_t t1 = monotonic_now();
    const int64_t total_memset_ns = t1 - t0;
    const size_t total_memset_size = memset_size * memset_iterations;

    /* printf("memset of %zu bytes %d times with throughput %zu B/s\n", */
    /*        memset_size, memset_iterations, (total_memset_size * 1'000) / (total_memset_ns / 1'000'000)); */

    printf("%zu\n", (total_memset_size * 1'000) / (total_memset_ns / 1'000'000));
}
