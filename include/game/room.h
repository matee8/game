/**
 * @file room.h
 * @brief Defines the Room structure and its associated functions for a 3D
 * environment.
 *
 * This header file provides the definition of a `room` structure, which
 * represents a 3D room in a game or simulation, along with functions to
 * initialize the room, retrieve its world bounding box, get its center
 * position, and update it based on player interactions.
 */

#ifndef ROOM_H
#define ROOM_H

#include <raylib.h>
#include "player.h"

/**
 * @enum neighbor
 * @brief Enum representing possible directions for neighboring rooms.
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
 * @struct room
 * @brief Represents a 3D room in the environment.
 *
 * The room structure contains information about its model, position, scale,
 * bounding box, and neighboring rooms in all six directions.
 */
struct room {
    Model model;               /**< 3D model representing the room */
    BoundingBox bounds;        /**< Bounding box of the room */
    float scale;               /**< Scale factor of the room */
    Vector3 position;          /**< World position of the room */
    struct room* neighbors[6]; /**< Pointers to neighboring rooms (RIGHT, LEFT,
                                  UP, DOWN, FORWARD, BACKWARD) */
};

/**
 * @brief Initializes a room with a model, texture, scale, position, and
 * neighbors.
 *
 * @param room Pointer to the room to initialize
 * @param position World position of the room
 * @param scale Scale factor for the room
 * @param neighbors Array of pointers to neighboring rooms
 * @param model_path File path to the room's 3D model
 * @param texture_path File path to the room's texture
 */
void init_room(struct room* room,
               Vector3 position,
               float scale,
               struct room* neighbors[],
               char* model_path,
               char* texture_path);

/**
 * @brief Gets the world-space bounding box of the room.
 *
 * @param room Pointer to the room
 * @return BoundingBox The bounding box in world coordinates
 */
BoundingBox get_world_box(struct room* room);

/**
 * @brief Gets the center position of the room.
 *
 * @param room Pointer to the room
 * @return Vector3 The center position of the room
 */
Vector3 get_center(struct room* room);

/**
 * @brief Updates the room based on the player's state or position.
 *
 * @param room Pointer to a pointer to the room
 * @param player Pointer to the player
 */
void update_room(struct room** room, struct player* player);

#endif  // ROOM_H
