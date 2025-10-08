#ifndef GAME_WORLD_ROOM_DEF_H
#define GAME_WORLD_ROOM_DEF_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Bitmask representing the four cardinal door directions.
 *
 * This allows for efficient checking of connections. A room with a DOOR_NORTH
 * can connect to a room with a DOOR_SOUTH.
 */
#define DOOR_NORTH (1U << 0U)  // +Y in a 2D grid context
#define DOOR_SOUTH (1U << 1U)  // -Y
#define DOOR_EAST (1U << 2U)   // +X
#define DOOR_WEST (1U << 3U)   // -X

/**
 * @struct room_def
 * @brief Stores the metadata for a single type of room.
 *
 * This structure holds the information parsed from a room model file,
 * such as its connection points (doors) and its asset path.
 */
struct room_def {
    char* model_path;  /**< Heap-allocated path to the .glb model file. */
    uint8_t door_mask; /**< Bitmask of door connections using DOOR_* flags. */
    int weight; /**< The probability weight for procedural generation. Higher is
                   more common. */
};

/**
 * @brief Loads all room templates from a given directory.
 *
 * This function scans the specified directory for .glb files, parses their
 * filenames to determine their door configurations, and stores them in an
 * internal list. It must be called before any other functions in this module.
 *
 * @param directory_path The path to the directory containing room models.
 * @return The number of templates successfully loaded on success.
 * @return On failure, returns a negative errno code (e.g., -ENOMEM, -EIO).
 */
int room_def_load_all(const char* directory_path);

/**
 * @brief Frees all memory associated with the loaded room templates.
 *
 * Should be called when the game is shutting down or changing levels to
 * prevent memory leaks.
 */
void room_def_unload_all(void);

/**
 * @brief Gets the total number of loaded room templates.
 * @return The number of templates available.
 */
size_t room_def_get_count(void);

/**
 * @brief Retrieves a room template by its index in the internal list.
 * @param index The index of the template to retrieve.
 * @return A constant pointer to the struct room_def, or NULL if the index is
 * invalid.
 */
const struct room_def* room_def_get_by_index(size_t index);

/**
 * @brief Finds a random room template that satisfies both required and
 * forbidden door constraints.
 *
 * This is the primary search function for the generator. It ensures that placed
 * rooms correctly connect to adjacent open doorways while simultaneously not
 * creating new doorways that lead into adjacent solid walls.
 *
 * A template is considered a match if:
 * 1. It contains all doors specified in the `required_doors` mask.
 * 2. It contains none of the doors specified in the `forbidden_doors` mask.
 *
 * @param required_doors A bitmask of doors the room must have to be considered
 * a match.
 * @param forbidden_doors A bitmask of doors the room must NOT have to be
 * considered a match.
 * @return A constant pointer to a suitable RoomTemplate, or nullptr if no
 * template satisfies the constraints.
 */
const struct room_def* room_def_find_constrained(uint8_t required_doors,
                                                 uint8_t forbidden_doors);

/**
 * @brief Removes a room definition from the available generation pool.
 *
 * This function is used to prevent special, one-off rooms (like the starting
 * room) from being procedurally generated after they have been manually placed.
 * It efficiently removes the specified template from the internal list using a
 * "swap and pop" operation.
 *
 * @param room_to_remove A constant pointer to the room definition to remove. If
 *                       the pointer is not found in the list, the function does
 * nothing.
 */
void room_def_remove(const struct room_def* room_to_remove);

#endif
