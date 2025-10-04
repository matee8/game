/**
 * @file vector.h
 * @brief A generic, pointer-based dynamic array implementation.
 *
 * This file defines the interface for a dynamic vector data structure.
 * It provides functions for creating, modifying, and destroying a vector
 * that stores `void*` elements.
 */

#ifndef GAME_DS_VECTOR_H
#define GAME_DS_VECTOR_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @struct vector
 * @brief A dynamic array that stores pointers to elements.
 *
 * The vector manages its own memory, growing automatically when new elements
 * are added. It stores elements as `void*`, making it a generic container.
 * The user is responsible for managing the memory of the elements themselves.
 */
struct vector {
    void**
        data; /**< A pointer to the dynamically allocated array of elements. */
    size_t len;      /**< The current number of elements in the vector. */
    size_t capacity; /**< The total number of elements the vector can hold
                        before resizing. */
};

/**
 * @brief Initializes a vector with zero capacity.
 * @param vec A pointer to the vector structure to initialize.
 * @return 0 on success, or -EINVAL if vec is NULL.
 */
int vector_init(struct vector* vec);

/**
 * @brief Initializes a vector with a specified initial capacity.
 * @param vec A pointer to the vector structure to initialize.
 * @param capacity The initial storage capacity of the vector.
 * @return 0 on success, -EINVAL if vec is NULL, or -ENOMEM on allocation
 * failure.
 */
int vector_init_with_capacity(struct vector* vec, size_t capacity);

/**
 * @brief Appends an element to the end of the vector.
 *
 * If the vector is at full capacity, its storage will be reallocated to
 * accommodate the new element.
 * @param vec A pointer to the vector.
 * @param element The pointer to the element to add.
 * @return 0 on success, -EINVAL if vec is NULL, or -ENOMEM on allocation
 * failure.
 */
int vector_push(struct vector* vec, void* element);

/**
 * @brief Removes and returns the last element of the vector.
 * @param vec A pointer to the vector.
 * @return The last element, or NULL if the vector is empty or vec is NULL.
 */
void* vector_pop(struct vector* vec);

/**
 * @brief Retrieves the element at a specific index.
 * @param vec A pointer to the constant vector.
 * @param index The index of the element to retrieve.
 * @return The element at the specified index, or NULL if the index is out of
 * bounds or vec is NULL.
 */
void* vector_get(const struct vector* vec, size_t index);

/**
 * @brief Replaces the element at a specific index.
 * @param vec A pointer to the vector.
 * @param index The index of the element to replace.
 * @param element The new element to place at the index.
 * @return 0 on success, or -EINVAL if the index is out of bounds or vec is
 * NULL.
 */
int vector_set(struct vector* vec, size_t index, void* element);

/**
 * @brief Gets the current number of elements in the vector.
 * @param vec A pointer to the constant vector.
 * @return The number of elements, or 0 if vec is NULL.
 */
size_t vector_len(const struct vector* vec);

/**
 * @brief Gets the current storage capacity of the vector.
 * @param vec A pointer to the constant vector.
 * @return The capacity of the vector, or 0 if vec is NULL.
 */
size_t vector_capacity(const struct vector* vec);

/**
 * @brief Checks if the vector contains any elements.
 * @param vec A pointer to the constant vector.
 * @return true if the vector's length is 0, false otherwise. Returns true if
 * vec is NULL.
 */
bool vector_is_empty(const struct vector* vec);

/**
 * @brief Removes all elements from the vector.
 *
 * This function sets the vector's length to 0 but does not change its capacity
 * or free the allocated memory. It does not free the memory of the elements
 * that were stored in the vector.
 * @param vec A pointer to the vector.
 */
void vector_clear(struct vector* vec);

/**
 * @brief Ensures the vector has at least a given capacity.
 *
 * If the current capacity is less than the requested capacity, the vector's
 * storage will be reallocated. If the current capacity is sufficient,
 * nothing is done.
 * @param vec A pointer to the vector.
 * @param new_capacity The minimum desired capacity.
 * @return 0 on success, -EINVAL if vec is NULL, or -ENOMEM on allocation
 * failure.
 */
int vector_reserve(struct vector* vec, size_t new_capacity);

/**
 * @brief Reduces the vector's capacity to match its current length.
 *
 * This operation reallocates the internal storage to free unused memory.
 * If the length is 0, all memory is freed and capacity is set to 0.
 * @param vec A pointer to the vector.
 * @return 0 on success, -EINVAL if vec is NULL, or -ENOMEM on allocation
 * failure.
 */
int vector_shrink_to_fit(struct vector* vec);

/**
 * @brief Frees all memory used by the vector.
 *
 * This function frees the internal data array. It does not free the memory
 * of the elements that were stored in the vector. After calling, the vector
 * is in an uninitialized state and should be re-initialized before use.
 * @param vec A pointer to the vector to destroy.
 */
void vector_destroy(struct vector* vec);

#endif
