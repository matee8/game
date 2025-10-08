/**
 * @file camera.h
 * @brief Defines the Camera structure and its associated functions for a 3D
 * environment.
 *
 * This header file provides the definition of a `camera` structure, which
 * represents the camera in a 3D game or simulation, along with functions to
 * initialize and update the camera based on the player's perspective and room
 * interactions.
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <raylib.h>
#include "room.h"

/**
 * @struct camera
 * @brief Represents a 3D camera in the environment.
 *
 * The camera structure contains the Raylib Camera3D object and a pointer to the
 * previously active room for smooth transitions or updates.
 */
struct camera {
    Camera3D camera_m; /**< The Raylib 3D camera */
    struct room*
        prev_current_room;   /**< Pointer to the previously active room */
    Vector3 prev_position;   /**< Previous position of the camera */
    Vector3 target_position; /**< Previous target point of the camera */
    Vector3 prev_center;
    Vector3 target_center;
};

/**
 * @brief Initializes the camera with default settings.
 *
 * @param camera Pointer to the camera to initialize
 */
void init_camera(struct camera* camera);

/**
 * @brief Updates the camera based on the current room and its state.
 *
 * @param camera Pointer to the camera to update
 * @param current_room Pointer to the current active room
 */
void update_camera(struct camera* camera, struct room* current_room);

#endif  // CAMERA_H
