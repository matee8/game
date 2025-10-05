/**
 * @file world.h
 * @brief The high-level orchestrator for the game world.
 *
 * This module connects the procedural generation systems to the main game loop.
 * It is responsible for initializing the world, tracking the player's position
 * to dynamically generate new chunks, streaming models in and out of memory,
 * and drawing the visible parts of the world.
 */
#ifndef GAME_WORLD_WORLD_H
#define GAME_WORLD_WORLD_H

#include "raylib.h"

/**
 * @brief Initializes the entire world system.
 *
 * This function orchestrates the initialization of all underlying world modules:
 * it loads room templates, initializes the world grid, and calls the generator
 * to create the starting area.
 *
 * @param seed The seed for the world generation.
 * @param assets_path The path to the directory containing room models.
 * @return 0 on success, or a negative error code if initialization fails.
 */
int world_init(unsigned int seed, const char* assets_path);

/**
 * @brief Cleans up and destroys the world, freeing all associated resources.
 */
void world_destroy(void);

/**
 * @brief Updates the state of the game world.
 *
 * Called every frame, this function tracks the player's position, triggers
 * new chunk generation when the player moves to a new grid cell, and manages
 * the loading/unloading of room models based on proximity to the player.
 * @param player_pos The player's current 3D world position.
 */
int world_update(Vector3 player_pos);

/**
 * @brief Draws all visible and loaded rooms in the world.
 */
int world_draw(void);

#endif
