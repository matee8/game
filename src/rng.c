#include "game/rng.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

void rng_init(uint32_t seed) {
    srand(seed);
}

int32_t rng_next_i32(void) {
    // NOLINTNEXTLINE(cert-msc30-c,concurrency-mt-unsafe,cert-msc50-cpp)
    return rand();
}

int32_t rng_get_range(int32_t min, int32_t max) {
    assert(min <= max && "rng_get_range: min cannot be greater than max");
    // NOLINTNEXTLINE(cert-msc30-c,concurrency-mt-unsafe,cert-msc50-cpp)
    return min + (rand() % (max - min));
}
