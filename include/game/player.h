/**
 * @file player.h
 * @brief Defines the Player structure and its associated functions for a 3D environment.
 *
 * This header file provides the definition of a `player` structure, which represents
 * the player in a 3D game or simulation, along with functions to initialize and
 * update the player.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

/**
 * @struct player
 * @brief Represents a player in the 3D environment.
 *
 * The player structure contains information about the player's position, speed,
 * health, direction, and 3D model/texture for rendering.
 */
struct player {
    Vector3 position;  /**< World position of the player */
    float speed;       /**< Movement speed of the player */
    int health;        /**< Current health of the player */
    int direction;     /**< Current facing direction of the player */
    Model model;       /**< 3D model representing the player */
    Texture2D texture; /**< Texture applied to the player's model */
};

/**
 * @brief Initializes a player with position, speed, and health.
 * 
 * @param player Pointer to the player to initialize
 * @param position Initial position of the player
 * @param speed Movement speed of the player
 * @param health Initial health of the player
 */
void init_player(struct player* player, Vector3 position, float speed, int health);

/**
 * @brief Updates the player's state, position, and interactions.
 * 
 * @param player Pointer to the player to update
 */
void update_player(struct player* player);

#endif // PLAYER_H
