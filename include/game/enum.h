#ifndef ENUM_H
#define ENUM_H

/**
 * @file enum.h
 * @brief Defines enums for neighbors, directions, and player states in the
 * game.
 */

/**
 * @enum neighbor
 * @brief Represents the relative position of a neighbor in a 3D space.
 */
enum neighbor {
    RIGHT,   /**< Neighbor to the right */
    LEFT,    /**< Neighbor to the left */
    UP,      /**< Neighbor above */
    DOWN,    /**< Neighbor below */
    FORWARD, /**< Neighbor in front */
    BACKWARD /**< Neighbor behind */
};

/**
 * @enum direction
 * @brief Represents compass directions for navigation or orientation.
 */
enum direction {
    WEST,       /**< West direction */
    EAST,       /**< East direction */
    NORTH,      /**< North direction */
    SOUTH,      /**< South direction */
    NORTH_EAST, /**< North-East direction */
    NORTH_WEST, /**< North-West direction */
    SOUTH_EAST, /**< South-East direction */
    SOUTH_WEST, /**< South-West direction */
};

/**
 * @enum player_state
 * @brief Represents the various states a player can be in.
 */
enum player_state {
    STATE_IDLE,      /**< Player is idle, not performing any action */
    STATE_RUNNING,   /**< Player is running */
    STATE_ATTACKING, /**< Player is attacking */
    STATE_RELOADING, /**< Player is reloading their weapon */
    STATE_DEAD       /**< Player is dead */
};

#endif  // ENUM_H
