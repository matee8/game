#ifndef ANIM_H
#define ANIM_H

#include <raylib.h>

/**
 * @file anim.h
 * @brief Provides structures and functions to handle GIF-like animations using
 * Raylib.
 */

/**
 * @struct anim
 * @brief Represents an animation with multiple frames.
 *
 * This structure holds the data required to manage
 * frame-based animations, including the image, texture,
 * current frame index, and timing information.
 */
struct anim {
    int frame_count;    /**< Total number of frames in the animation */
    Image gif_anim;     /**< Loaded image containing all animation frames */
    Texture2D tex_anim; /**< Texture used for rendering the current frame */
    unsigned int
        next_frame_offset; /**< Offset to the next frame in the image data */
    int current_frame;     /**< Index of the current frame */
    int frame_delay; /**< Number of updates to wait before switching to the next
                        frame */
    int frame_counter; /**< Counter used to track the frame delay */
};

/**
 * @brief Initializes an animation from a file path.
 *
 * Loads the image from the specified path and prepares the texture and frame
 * data.
 *
 * @param animation Pointer to the animation struct to initialize.
 * @param path Path to the animation file (e.g., GIF or sprite sheet).
 * @param frame_delay Number of update cycles to wait before advancing a frame.
 */
void init_anim(struct anim* animation, const char* path, int frame_delay);

/**
 * @brief Updates the animation to the next frame if the delay has passed.
 *
 * This function should be called every frame to progress the animation.
 *
 * @param animation Pointer to the animation struct to update.
 */
void update_anim(struct anim* animation);

/**
 * @brief Unloads the animation resources.
 *
 * Frees the image and texture associated with the animation.
 *
 * @param animation Pointer to the animation struct to unload.
 */
void unload_anim(struct anim* animation);

<<<<<<< HEAD
#endif  // ANIM_H
=======
#endif
>>>>>>> 6386b42 (chore: clang-tidy and format)
