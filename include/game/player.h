/**
 * @file player.h
 * @brief Defines the Player structure and its associated functions for a 3D
 * environment.
 *
 * This header file provides the definition of a `player` structure, which
 * represents the player in a 3D game or simulation, along with functions to
 * initialize, update, draw, and unload the player.
 */

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <raylib.h>
#include <stdbool.h>
#include "anim.h"
#include "enum.h"

/**
 * @def NUM_DIRECTIONS
 * @brief Number of directions for directional animations (e.g., NORTH, SOUTH,
 * etc.).
 */
#define NUM_DIRECTIONS 8

/**
 * @struct player
 * @brief Represents a player in the 3D environment.
 *
 * The player structure contains information about the player's position, speed,
 * health, current state, facing direction, and animations for different
 * actions.
 */
struct player {
    Vector3 position;         /**< World position of the player */
    float speed;              /**< Movement speed of the player */
    int health;               /**< Current health of the player */
    enum direction direction; /**< Current facing direction of the player */
    enum player_state state;  /**< Current state of the player */

    struct anim idle_anim[NUM_DIRECTIONS]; /**< Animations for the player's idle
                                              state */
    struct anim run_anim[NUM_DIRECTIONS];  /**< Animations for the player's
                                              running state */
    struct anim death_anim[NUM_DIRECTIONS];  /**< Animations for the player's
                                                death state */
    struct anim attack_anim[NUM_DIRECTIONS]; /**< Animations for the player's
                                                attack state */
    struct anim reload_anim[NUM_DIRECTIONS]; /**< Animations for the player's
                                                reload state */
};

/**
 * @brief Initializes a player with position, speed, and health.
 *
 * This function sets up the player's initial position, speed, health, state,
 * and loads the animations.
 *
 * @param player Pointer to the player to initialize
 * @param position Initial world position of the player
 * @param speed Movement speed of the player
 * @param health Initial health of the player
 */
void init_player(struct player* player,
                 Vector3 position,
                 float speed,
                 int health);

/**
 * @brief Updates the player's state, position, and interactions.
 *
 * This function should be called every frame to update the player's movement,
 * state transitions, and animations.
 *
 * @param player Pointer to the player to update
 */
void update_player(struct player* player);

/**
 * @brief Draws the player in the 3D world using the provided camera.
 *
 * @param player Pointer to the player to draw
 * @param camera Camera3D object for rendering
 */
void draw_player(struct player* player, Camera3D camera);

/**
 * @brief Unloads the player's resources.
 *
 * Frees any textures or animations associated with the player.
 *
 * @param player Pointer to the player to unload
 */
void unload_player(struct player* player);

#endif  // GAME_PLAYER_H
