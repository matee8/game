#include "game/rng.h"

#include <assert.h>
#include <stdint.h>

static struct {
    uint64_t s[4];
} rng_state;

static inline uint64_t rotl(const uint64_t x, unsigned int k);

void rng_init(uint64_t seed) {
    uint64_t z = seed + 0x9e3779b97f4a7c15;
    for (int i = 0; i < 4; i++) {
        z = (z ^ (z >> 30U)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27U)) * 0x94d049bb133111eb;
        rng_state.s[i] = z ^ (z >> 31U);
    }
}

uint64_t rng_next_u64(void) {
    const uint64_t result = rotl(rng_state.s[1] * 5, 7) * 9;
    const uint64_t t = rng_state.s[1] << 17U;

    rng_state.s[2] ^= rng_state.s[0];
    rng_state.s[3] ^= rng_state.s[1];
    rng_state.s[1] ^= rng_state.s[2];
    rng_state.s[0] ^= rng_state.s[3];

    rng_state.s[2] ^= t;
    rng_state.s[3] = rotl(rng_state.s[3], 45);

    return result;
}

int rng_get_range(int min, int max) {
    assert(min <= max && "rng_get_range: min cannot be greater than max");

    uint64_t range = (uint64_t)(max - min) + 1;
    if (range == 0) {
        return (int)rng_next_u64();
    }

    uint64_t threshold = UINT64_MAX - (UINT64_MAX % range);
    uint64_t n = 0;

    do {
        n = rng_next_u64();
    } while (n >= threshold);

    return (int)(n % range) + min;
}

static inline uint64_t rotl(const uint64_t x, unsigned int k) {
    return (x << k) | (x >> (64U - k));
}
