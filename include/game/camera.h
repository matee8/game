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

#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include <raylib.h>

/**
 * @struct camera
 * @brief Represents a 3D camera in the environment.
 *
 * The camera structure contains the Raylib Camera3D object and a pointer to the
 * previously active room for smooth transitions or updates.
 */
struct camera {
    Camera3D camera_m;       /**< The Raylib 3D camera */
    Vector3 target_position; /**< Previous target point of the camera */
    Vector3 target_center;
};

/**
 * @brief Initializes the camera with default settings.
 *
 * @param camera Pointer to the camera to initialize
 */
void init_camera(struct camera* camera);

/**
 * Updates the camera's target focus point.
 * The camera will smoothly interpolate its position and target towards the
 * new center point.
 *
 * @param camera The camera instance to update.
 * @param new_center The new world-space position the camera should look at.
 */
void camera_focus_on(struct camera* camera, Vector3 new_center);

#endif  // CAMERA_H
