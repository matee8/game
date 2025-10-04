/**
 * @file string.h
 * @brief A dynamic string with Small String Optimization (SSO).
 *
 * This file provides the API for a string buffer designed for efficient
 * string construction and manipulation. It avoids heap allocations for small
 * strings, offering significant performance benefits in common use cases.
 */
#ifndef COMPILER_DS_STRING_H
#define COMPILER_DS_STRING_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @struct string
 * @brief A dynamic string builder with Small String Optimization (SSO).
 *
 * This structure provides efficient string manipulation. For small strings,
 * it avoids heap allocation by storing the content directly within the struct's
 * own memory (SSO). When the string grows beyond this capacity, it
 * transparently switches to a heap-allocated buffer.
 */
struct dstring {
    union {
        struct large_string {
            size_t capacity; /**< Capacity of the heap-allocated buffer. */
            char* ptr;       /**< Pointer to the heap-allocated buffer. */
        } heap;
        char sso[sizeof(
            struct large_string)]; /**< Buffer for Small String Optimization. */
    } data;
    size_t len;   /**< The current length of the string (excluding null
                     terminator). */
    bool on_heap; /**< True if the string is stored on the heap, false for SSO.
                   */
};

/**
 * @brief Initializes a string to an empty state.
 *
 * Sets the buffer to use Small String Optimization by default with zero length.
 * @param sb A pointer to the string structure to initialize.
 * @return 0 on success, or -EINVAL if sb is NULL.
 */
int dstring_init(struct dstring* sb);

/**
 * @brief Frees all heap-allocated memory used by the string.
 *
 * If the buffer was using the heap, its memory is freed. The buffer is then
 * reset to a valid, empty initial state.
 * @param sb A pointer to the string to destroy.
 */
void dstring_destroy(struct dstring* sb);

/**
 * @brief Appends a character sequence of a specified length to the string.
 *
 * This is the primary method for adding content. It will automatically handle
 * memory allocation and resizing if necessary.
 * @param sb A pointer to the string.
 * @param str The character sequence to append.
 * @param len The number of characters to append from the sequence.
 * @return 0 on success, -EINVAL for invalid arguments, or -ENOMEM on allocation
 * failure.
 */
int dstring_append(struct dstring* sb, const char* str, size_t len);

/**
 * @brief Appends a null-terminated C-style string to the string.
 *
 * A convenience wrapper around `string_append` that automatically
 * calculates the length of the source string.
 * @param sb A pointer to the string.
 * @param str The null-terminated C-style string to append.
 * @return 0 on success, or an error code on failure.
 */
int dstring_append_cstr(struct dstring* sb, const char* str);

/**
 * @brief Gets a non-owning, constant pointer to the internal null-terminated
 * string.
 *
 * The returned pointer is valid only until the next modifying operation on the
 * string (e.g., `string_append`). It should be used for read-only
 * access.
 * @param sb A pointer to the constant string.
 * @return A `const char*` to the internal string. Returns an empty string "" if
 * sb is NULL.
 */
const char* dstring_get_cstr(const struct dstring* sb);

/**
 * @brief Converts the string into a heap-allocated string, transferring
 * ownership.
 *
 * This function creates a new, correctly-sized C-string on the heap with the
 * buffer's contents. The caller is responsible for freeing this new string.
 * The string is reset to an empty state after this operation.
 * @param sb A pointer to the string.
 * @return A new heap-allocated string that the caller must free, or NULL on
 * allocation failure.
 */
char* dstring_to_cstr(struct dstring* sb);

/**
 * @brief Gets the current length of the string in the string.
 * @param sb A pointer to the constant string.
 * @return The length of the string in characters, excluding the null
 * terminator.
 */
size_t dstring_get_len(const struct dstring* sb);

#endif
