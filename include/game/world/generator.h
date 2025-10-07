/**
 * @file generator.h
 * @brief Defines the procedural world generation algorithm.
 *
 * This module is responsible for the logic of creating the room layout.
 * It uses the world grid as a canvas and the room templates as a palette
 * to build the world deterministically based on a seed.
 */
#ifndef GAME_WORLD_GENERATOR_H
#define GAME_WORLD_GENERATOR_H

#include <stdint.h>

/**
 * @brief Initializes the generator and the world's starting state.
 *
 * This function must be called once at the start of the game. It performs
 * two critical actions:
 * 1. It seeds the deterministic random number generator (`rng` module).
 * 2. It places the 'starting_room' at grid coordinate (0, 0) and generates
 *    the initial chunk of rooms around it, ensuring the player spawns into
 *    a populated area.
 *
 * @param seed The seed for the entire world generation process.
 */
int generator_init(unsigned int seed);

/**
 * @brief Generates a chunk of rooms centered around the given coordinates.
 *
 * This function implements the core "frontier" generation algorithm. It scans
 * a 5x5 area around the center point, identifies empty cells adjacent to
 * existing rooms, and fills them with compatible rooms from the template list.
 * This is designed to be called by the game world as the player moves to

 * generate the world dynamically.
 *
 * @param center_x The center grid x-coordinate of the chunk to generate.
 * @param center_y The center grid y-coordinate of the chunk to generate.
 */
int generator_create_chunk(int32_t center_x, int32_t center_y);

#endif
