/**
 * @file player.h
 * @brief Defines the Player structure and its associated functions for a 3D
 * environment.
 *
 * This header file provides the definition of a `player` structure, which
 * represents the player in a 3D game or simulation, along with functions to
 * initialize and update the player.
 */

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <raylib.h>
#include "anim.h"
#include "enum.h"
#include <stdbool.h>

/**
 * @struct player
 * @brief Represents a player in the 3D environment.
 *
 * The player structure contains information about the player's position, speed,
 * health, direction, and 3D model/texture for rendering.
 */

#define NUM_DIRECTIONS 8

struct player {
    Vector3 position; /**< World position of the player */
    float speed;      /**< Movement speed of the player */
    int health;       /**< Current health of the player */
    enum direction direction;    /**< Current facing direction of the player */
    enum player_state state; /**< Current state of the player */

    struct anim idle_anim[NUM_DIRECTIONS]; /**< Animation for the player's idle state */
    struct anim run_anim[NUM_DIRECTIONS]; /**< Animation for the player's running state */
    struct anim death_anim[NUM_DIRECTIONS]; /**< Animation for the player's death state */
    struct anim attack_anim[NUM_DIRECTIONS]; /**< Animation for the player's attack state */
    struct anim reload_anim[NUM_DIRECTIONS]; /**< Animation for the player's reload state */
};

/**
 * @brief Initializes a player with position, speed, and health.
 *
 * @param player Pointer to the player to initialize
 * @param position Initial position of the player
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
 * @param player Pointer to the player to update
 */
void update_player(struct player* player);

void draw_player(struct player* player, Camera3D camera);
void unload_player(struct player* player);

#endif  // PLAYER_H
