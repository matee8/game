#ifndef ENUM_H
#define ENUM_H

enum neighbor {
    RIGHT,   /**< Neighbor to the right */
    LEFT,    /**< Neighbor to the left */
    UP,      /**< Neighbor above */
    DOWN,    /**< Neighbor below */
    FORWARD, /**< Neighbor in front */
    BACKWARD /**< Neighbor behind */
};

enum direction{
    WEST,
    EAST,
    NORTH,
    SOUTH,
    NORTH_EAST,
    NORTH_WEST,
    SOUTH_EAST,
    SOUTH_WEST,
};

enum player_state {
    IDLE,
    RUNNING,
    ATTACKING,
    RELOADING,
    DEAD
}

#endif
