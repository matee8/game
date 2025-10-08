/**
 * @file grid.h
 * @brief Manages the state of the infinite 2D world grid.
 *
 * This module acts as the logical map of the game world. It uses a hash map
 * to store which room exists at any given (x, y) coordinate, making it
 * efficient for sparse, procedurally generated environments. It is responsible
 * for the lifecycle of WorldCell data and the loading/unloading of their
 * associated 3D models.
 */
#ifndef GAME_WORLD_GRID_H
#define GAME_WORLD_GRID_H

#include "raylib.h"

#include "game/world/room_def.h"

/**
 * @struct world_cell
 * @brief Represents a single cell in the world grid containing a room.
 */
struct world_cell {
    const struct room_def* template; /**< Pointer to the room's blueprint.
                                      */
    Model model;                     /**< The loaded Raylib 3D model. */
    bool is_model_loaded; /**< True if the 3D model is currently in VRAM. */
    int grid_x;           /**< The cell's X coordinate on the grid. */
    int grid_y;           /**< The cell's Y coordinate on the grid. */
};

/**
 * @brief Initializes the world grid system.
 *
 * Must be called before any other functions in this module.
 * @return 0 on success, or -ENOMEM if the underlying hash map cannot be
 * initialized.
 */
int grid_init(void);

/**
 * @brief Frees all memory used by the world grid.
 *
 * This function iterates through all placed rooms, unloads any loaded models,
 * frees all WorldCell memory, and destroys the underlying hash map.
 */
void grid_destroy(void);

/**
 * @brief Places a room at a specific grid coordinate.
 *
 * This creates a WorldCell and adds it to the grid. The room's 3D model is
 * NOT loaded by this function. If a room already exists at the given
 * coordinates, this function succeeds but does nothing.
 *
 * @param x The grid x-coordinate.
 * @param y The grid y-coordinate.
 * @param room_template A pointer to the template of the room to place.
 * @return 0 on success, -EINVAL if room_template is NULL, or -ENOMEM on
 * allocation failure.
 */
int grid_place_room(int x, int y, const struct room_def* room_template);

/**
 * @brief Retrieves a cell from the grid.
 * @param x The grid x-coordinate.
 * @param y The grid y-coordinate.
 * @return A pointer to the WorldCell, or NULL if no room exists at that
 * coordinate.
 */
struct world_cell* grid_get_cell(int x, int y);

/**
 * @brief Ensures the 3D model for a room is loaded into memory.
 *
 * If the model is already loaded, this function succeeds immediately.
 * @param cell A pointer to the WorldCell whose model should be loaded.
 * @return 0 on success, or -EINVAL if cell is NULL.
 */
int grid_load_model(struct world_cell* cell);

/**
 * @brief Ensures the 3D model for a room is unloaded from memory.
 *
 * If the model is not loaded, this function succeeds immediately.
 * @param cell A pointer to the WorldCell whose model should be unloaded.
 * @return 0 on success, or -EINVAL if cell is NULL.
 */
int grid_unload_model(struct world_cell* cell);

#endif
