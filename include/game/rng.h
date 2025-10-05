/**
 * @file rng.h
 * @brief Provides a seeded, deterministic pseudo-random number generator.
 *
 * This module is designed to produce a predictable sequence of random numbers
 * based on an initial seed. It manages its own internal state, which is
 * completely separate from Raylib's global RNG or the C standard library's
 * rand(). This is intended for all procedural generation tasks (world layout,
 * item placement) to ensure worlds can be reproduced from a seed.
 *
 * For non-deterministic randomness (e.g., enemy AI, particle effects),
 * use Raylib's GetRandomValue() directly.
 */
#ifndef GAME_RNG_H
#define GAME_RNG_H

#include <stdint.h>

/**
 * @brief Initializes the global random number generator state with a seed.
 *
 * This function must be called once before any other function in this module
 * to start the deterministic sequence.
 * @param seed The initial 64-bit seed for the random sequence.
 */
void rng_init(uint64_t seed);

/**
 * @brief Generates the next 64-bit unsigned integer in the sequence.
 *
 * This can be useful for tasks that require a raw 64-bit random value.
 * For generating numbers in a specific range, rng_get_range() is preferred.
 * @return A pseudo-random 64-bit integer.
 */
uint64_t rng_next_u64(void);

/**
 * @brief Generates a pseudo-random integer within a specified inclusive range.
 *
 * This is the primary function to be used for most game logic, as it provides
 * an unbiased random number between min and max.
 * @param min The minimum inclusive value of the range.
 * @param max The maximum inclusive value of the range.
 * @return A pseudo-random integer within the specified range.
 */
int rng_get_range(int min, int max);

#endif
