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
 * @brief Finds a random room template that has a specific set of doors.
 *
 * This function is useful for finding rooms that fit perfectly into a generated
 * space, such as finding a dead-end room that only has a north door.
 *
 * @param required_doors A bitmask of the exact doors the room must have.
 * @return A constant pointer to a matching struct room_def, or NULL if no match
 * is found.
 */
const struct room_def* room_def_find_matching(uint8_t required_doors);

/**
 * @brief Finds a random room template that is compatible with a set of doors.
 *
 * This function is useful for growing the map organically. It finds a room
 * that has AT LEAST the required doors, allowing for more open and varied
 * layouts (e.g., a cross-room is a compatible match for a required north door).
 *
 * @param required_doors A bitmask of the doors the room must have.
 * @return A constant pointer to a compatible struct room_def, or nullptr if no
 * match is found.
 */
const struct room_def* room_def_find_compatible(uint8_t required_doors);

#endif
