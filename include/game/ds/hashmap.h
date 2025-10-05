/**
 * @file hashmap.h
 * @brief Public interface for a generic hash map data structure.
 *
 * This file defines the API for a hash map that maps 64-bit unsigned integer
 * keys to `void*` values. It uses open addressing with linear probing for
 * collision resolution.
 */
#ifndef GAME_DS_HASHMAP_H
#define GAME_DS_HASHMAP_H

#include <stddef.h>
#include <stdint.h>

/**
 * @struct hashmap_entry
 * @brief An internal key-value pair stored within the hash map.
 */
struct hashmap_entry {
    uint64_t key; /**< The 64-bit unsigned integer key. */
    void* value;  /**< A pointer to the associated value. */
};

/**
 * @struct hashmap
 * @brief A hash map implementation using open addressing with linear probing.
 *
 * This hash map stores key-value pairs where the key is a 64-bit unsigned
 * integer and the value is a generic pointer. The map automatically handles
 * resizing when it becomes too full to maintain performance.
 */
struct hashmap {
    struct hashmap_entry*
        entries;     /**< The dynamically allocated array of entries. */
    size_t len;      /**< The current number of elements in the map. */
    size_t capacity; /**< The total number of available slots in the map. */
};

/**
 * @brief Initializes a hash map with a default initial capacity.
 * @param map A pointer to the hashmap structure to initialize.
 * @return 0 on success, -EINVAL if map is NULL, or -ENOMEM on allocation
 * failure.
 */
int hashmap_init(struct hashmap* map);

/**
 * @brief Frees all memory used by the hash map.
 *
 * This function frees the internal entry array. It does not free the memory
 * of the values that were stored in the map. After calling, the map
 * is in an uninitialized state and should be re-initialized before use.
 * @param map A pointer to the hash map to destroy.
 */
void hashmap_destroy(struct hashmap* map);

/**
 * @brief Inserts or updates a key-value pair in the hash map.
 *
 * If the key already exists, its associated value is updated with the new
 * value, and the old value is returned. If the key does not exist, a new entry
 * is created. The map may be resized if the load factor exceeds a threshold.
 *
 * @param map A pointer to the hash map.
 * @param key The 64-bit key for the entry.
 * @param value The pointer value to associate with the key.
 * @param[out] old_value A pointer to a `void*` that will receive the old value
 * if the key was updated. Can be NULL if the old value is not needed.
 * @return 0 on success, -EINVAL if map or value is NULL, or -ENOMEM on
 * allocation failure.
 */
int hashmap_set(struct hashmap* map,
                uint64_t key,
                void* value,
                void** old_value);

/**
 * @brief Retrieves the value associated with a given key.
 * @param map A pointer to the constant hash map.
 * @param key The key of the value to retrieve.
 * @return The value associated with the key, or NULL if the key is not found or
 * map is NULL.
 */
void* hashmap_get(const struct hashmap* map, uint64_t key);

/**
 * @brief Removes a key-value pair from the hash map.
 *
 * The removal is handled using a tombstone mechanism to preserve the integrity
 * of the probing chains.
 *
 * @param map A pointer to the hash map.
 * @param key The key of the entry to remove.
 * @return The value of the removed entry, or NULL if the key was not found or
 * map is NULL.
 */
void* hashmap_remove(struct hashmap* map, uint64_t key);

/**
 * @brief Gets the current number of elements in the hash map.
 * @param map A pointer to the constant hash map.
 * @return The number of elements, or 0 if map is NULL.
 */
size_t hashmap_len(const struct hashmap* map);

/**
 * @brief Gets the current storage capacity of the hash map.
 * @param map A pointer to the constant hash map.
 * @return The capacity of the hash map, or 0 if map is NULL.
 */
size_t hashmap_capacity(const struct hashmap* map);

/**
 * @brief Iterates over the key-value pairs in the hash map.
 *
 * To use, initialize a size_t iterator to 0 and call this function in a loop.
 * The function will find the next valid entry and update the iterator state.
 *
 * @example
 * size_t iter = 0;
 * uint64_t key;
 * void* value;
 * while (hashmap_next(my_map, &iter, &key, &value)) {
 *     // process key and value
 * }
 *
 * @param map A pointer to the constant hash map.
 * @param[in,out] iterator A pointer to a size_t variable that tracks the
 *                         iteration state. Must be initialized to 0 for the
 *                         first call.
 * @param[out] key A pointer that will receive the key of the next entry.
 * @param[out] value A pointer to a void* that will receive the value of the
 * next entry.
 * @return true if a key-value pair was found, false if the iteration is
 * complete.
 */
bool hashmap_iter(const struct hashmap* map,
                  size_t* iterator,
                  uint64_t* key,
                  void** value);

#endif
