#include "game/ds/hashmap.h"

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static int g_tombstone_marker;
#define TOMBSTONE ((void*)&g_tombstone_marker)

#define IS_EMPTY(entry) ((entry)->value == nullptr)

#define IS_TOMBSTONE(entry) ((entry)->value == TOMBSTONE)

static const size_t HASHMAP_INITIAL_CAPACITY = 16;

static const size_t HASHMAP_LOAD_FACTOR_NUMERATOR = 3;
static const size_t HASHMAP_LOAD_FACTOR_DENOMINATOR = 4;

#define HASHMAP_NEEDS_RESIZE(len, capacity)          \
    (((len) + 1) * HASHMAP_LOAD_FACTOR_DENOMINATOR > \
     (capacity) * HASHMAP_LOAD_FACTOR_NUMERATOR)

static struct hashmap_entry* find_entry(struct hashmap_entry* entries,
                                        size_t capacity,
                                        uint64_t key);
static uint64_t hash_key(uint64_t key);
static int hashmap_resize(struct hashmap* map, size_t new_capacity);

int hashmap_init(struct hashmap* map) {
    if (!map) {
        return -EINVAL;
    }

    map->entries =
        calloc(HASHMAP_INITIAL_CAPACITY, sizeof(struct hashmap_entry));
    if (!map->entries) {
        return -ENOMEM;
    }

    map->len = 0;
    map->capacity = HASHMAP_INITIAL_CAPACITY;
    return 0;
}

void hashmap_destroy(struct hashmap* map) {
    if (!map) {
        return;
    }

    free(map->entries);

    map->entries = nullptr;
    map->len = 0;
    map->capacity = 0;
}

int hashmap_set(struct hashmap* map,
                uint64_t key,
                void* value,
                void** old_value) {
    if (!map || !value) {
        return -EINVAL;
    }

    if (HASHMAP_NEEDS_RESIZE(map->len, map->capacity)) {
        size_t new_capacity = map->capacity * 2;
        if (hashmap_resize(map, new_capacity) != 0) {
            return -ENOMEM;
        }
    }

    struct hashmap_entry* entry = find_entry(map->entries, map->capacity, key);
    bool is_new_key = IS_EMPTY(entry) || IS_TOMBSTONE(entry);

    if (old_value) {
        *old_value = (int)is_new_key ? nullptr : entry->value;
    }

    if (is_new_key) {
        map->len++;
        entry->key = key;
    }

    entry->value = value;
    return 0;
}

void* hashmap_get(const struct hashmap* map, uint64_t key) {
    if (!map || map->len == 0) {
        return nullptr;
    }

    const struct hashmap_entry* entry =
        find_entry(map->entries, map->capacity, key);
    if (IS_EMPTY(entry) || IS_TOMBSTONE(entry)) {
        return nullptr;
    }

    return entry->value;
}

void* hashmap_remove(struct hashmap* map, uint64_t key) {
    if (!map || map->len == 0) {
        return nullptr;
    }

    struct hashmap_entry* entry = find_entry(map->entries, map->capacity, key);
    if (IS_EMPTY(entry) || IS_TOMBSTONE(entry)) {
        return nullptr;
    }

    void* old_value = entry->value;
    entry->value = TOMBSTONE;
    map->len--;

    return old_value;
}

size_t hashmap_len(const struct hashmap* map) {
    return map ? map->len : 0;
}

size_t hashmap_capacity(const struct hashmap* map) {
    return map ? map->capacity : 0;
}

bool hashmap_iter(const struct hashmap* map,
                  size_t* iterator,
                  uint64_t* key,
                  void** value) {
    if (!map || !iterator || !key || !value) {
        return false;
    }

    while (*iterator < map->capacity) {
        struct hashmap_entry* entry = &map->entries[*iterator];
        (*iterator)++;

        if (!IS_EMPTY(entry) && !IS_TOMBSTONE(entry)) {
            *key = entry->key;
            *value = entry->value;
            return true;
        }
    }

    return false;
}

static int hashmap_resize(struct hashmap* map, size_t new_capacity) {
    struct hashmap_entry* new_entries =
        calloc(new_capacity, sizeof(struct hashmap_entry));
    if (!new_entries) {
        return -ENOMEM;
    }

    for (size_t i = 0; i < map->capacity; ++i) {
        struct hashmap_entry* entry = &map->entries[i];
        if (!IS_EMPTY(entry) && !IS_TOMBSTONE(entry)) {
            struct hashmap_entry* dest =
                find_entry(new_entries, new_capacity, entry->key);
            dest->key = entry->key;
            dest->value = entry->value;
        }
    }

    free(map->entries);
    map->entries = new_entries;
    map->capacity = new_capacity;

    return 0;
}

static struct hashmap_entry* find_entry(struct hashmap_entry* entries,
                                        size_t capacity,
                                        uint64_t key) {
    uint64_t index = hash_key(key) % capacity;
    struct hashmap_entry* tombstone = nullptr;

    for (;;) {
        struct hashmap_entry* entry = &entries[index];

        if (IS_EMPTY(entry)) {
            return tombstone != nullptr ? tombstone : entry;
        }

        if (IS_TOMBSTONE(entry)) {
            if (tombstone == nullptr) {
                tombstone = entry;
            }
        } else if (entry->key == key) {
            return entry;
        }

        index = (index + 1) % capacity;
    }
}

static uint64_t hash_key(uint64_t key) {
    key ^= key >> 33LU;
    key *= 0xff51afd7ed558ccd;
    key ^= key >> 33LU;
    key *= 0xc4ceb9fe1a85ec53;
    key ^= key >> 33LU;
    return key;
}
