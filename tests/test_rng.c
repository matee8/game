#include "game/rng.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define RUN_TEST(test)                          \
    do {                                        \
        printf("Running test: %s...\n", #test); \
        test();                                 \
    } while (0)

void test_determinism(void) {
    const int sequence_len = 100;
    int sequence1[sequence_len];
    int sequence2[sequence_len];

    rng_init(12345);
    for (int i = 0; i < sequence_len; i++) {
        sequence1[i] = rng_get_range(-1000, 1000);
    }

    rng_init(12345);
    for (int i = 0; i < sequence_len; i++) {
        sequence2[i] = rng_get_range(-1000, 1000);
    }

    for (int i = 0; i < sequence_len; i++) {
        assert(sequence1[i] == sequence2[i]);
    }
}

void test_different_seeds_produce_different_sequences(void) {
    const int sequence_len = 100;
    int sequence1[sequence_len];
    int sequence2[sequence_len];

    rng_init(42);
    for (int i = 0; i < sequence_len; i++) {
        sequence1[i] = rng_get_range(0, 100);
    }

    rng_init(43);
    for (int i = 0; i < sequence_len; i++) {
        sequence2[i] = rng_get_range(0, 100);
    }

    bool are_identical = true;
    for (int i = 0; i < sequence_len; i++) {
        if (sequence1[i] != sequence2[i]) {
            are_identical = false;
            break;
        }
    }
    assert(!are_identical);
}

void test_range_bounds(void) {
    rng_init(999);
    const int min = -10;
    const int max = 10;

    for (int i = 0; i < 10000; i++) {
        int val = rng_get_range(min, max);
        assert(val >= min && val <= max);
    }
}

void test_single_value_range(void) {
    rng_init(101);
    for (int i = 0; i < 100; i++) {
        assert(rng_get_range(5, 5) == 5);
    }
}

int main(void) {
    puts("Starting rng tests.\n");

    RUN_TEST(test_determinism);
    RUN_TEST(test_different_seeds_produce_different_sequences);
    RUN_TEST(test_range_bounds);
    RUN_TEST(test_single_value_range);

    puts("\nAll rng tests passed successfully!");

    return EXIT_SUCCESS;
}
