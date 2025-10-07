#include "game/world/room_def.h"

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#include "game/ds/vector.h"
#include "game/rng.h"

static struct vector room_defs;
static bool is_initialized = false;

struct room_attributes {
    uint8_t door_mask;
    int weight;
};

static struct room_attributes parse_attributes_from_filename(
    const char* filename);
static const struct room_def* select_weighted_random_from_matches(
    const struct vector* matches);

int room_def_load_all(const char* directory_path) {
    if (is_initialized) {
        TraceLog(LOG_WARNING, "ROOM_DEF: Templates already initialized.");
        return (int)vector_len(&room_defs);
    }

    if (vector_init(&room_defs) != 0) {
        TraceLog(LOG_ERROR,
                 "ROOM_DEF: Failed to initialize vector (out of memory).");
        return -ENOMEM;
    }

    int ret = 0;
    FilePathList files = LoadDirectoryFiles(directory_path);

    if (!files.paths) {
        TraceLog(LOG_ERROR, "ROOM_DEF: Failed to load directory: %s",
                 directory_path);
        ret = -EIO;
        goto cleanup_vector;
    }

    for (unsigned int i = 0; i < files.count; ++i) {
        const char* path = files.paths[i];
        if (!IsFileExtension(path, ".glb")) {
            continue;
        }

        const char* filename = GetFileName(path);
        struct room_attributes attr = parse_attributes_from_filename(filename);
        if (attr.door_mask == 0) {
            continue;
        }

        struct room_def* template = malloc(sizeof(struct room_def));
        if (!template) {
            ret = -ENOMEM;
            goto cleanup_files;
        }

        template->model_path = strdup(path);
        if (!template->model_path) {
            free(template);
            ret = -ENOMEM;
            goto cleanup_files;
        }

        template->door_mask = attr.door_mask;
        template->weight = attr.weight;
        if (vector_push(&room_defs, template) != 0) {
            free(template->model_path);
            free(template);
            ret = -ENOMEM;
            goto cleanup_files;
        }
    }

    is_initialized = true;
    TraceLog(LOG_INFO, "ROOM_DEF: Loaded %zu room templates.",
             vector_len(&room_defs));
    ret = (int)vector_len(&room_defs);

cleanup_files:
    UnloadDirectoryFiles(files);

cleanup_vector:
    if (ret < 0) {
        room_def_unload_all();
    }

    return ret;
}

void room_def_unload_all() {
    if (!is_initialized) {
        return;
    }

    for (size_t i = 0; i < vector_len(&room_defs); ++i) {
        struct room_def* template = vector_get(&room_defs, i);
        free(template->model_path);
        free(template);
    }

    vector_destroy(&room_defs);
    is_initialized = false;
    TraceLog(LOG_INFO, "ROOM_DEF: Unloaded all room templates.");
}

size_t room_def_get_count(void) {
    return (int)is_initialized ? vector_len(&room_defs) : 0;
}

const struct room_def* room_def_get_by_index(size_t index) {
    if (!is_initialized) {
        return nullptr;
    }

    return (const struct room_def*)vector_get(&room_defs, index);
}

const struct room_def* room_def_find_constrained(uint8_t required_doors,
                                                 uint8_t forbidden_doors) {
    if (!is_initialized) {
        return nullptr;
    }

    struct vector matches;
    if (vector_init(&matches) != 0) {
        TraceLog(LOG_ERROR,
                 "ROOM_TEMPLATE: Failed to initialize matches vector.");
        return nullptr;
    }

    for (size_t i = 0; i < vector_len(&room_defs); i++) {
        const struct room_def* template = vector_get(&room_defs, i);

        bool has_required =
            (template->door_mask & required_doors) == required_doors;
        bool has_no_forbidden = (template->door_mask & forbidden_doors) == 0;

        if ((int)has_required && (int)has_no_forbidden) {
            if (vector_push(&matches, (void*)template) != 0) {
                TraceLog(LOG_WARNING,
                         "ROOM_TEMPLATE: Failed to push match to vector.");
            }
        }
    }

    const struct room_def* result =
        select_weighted_random_from_matches(&matches);

    vector_destroy(&matches);
    return result;
}

void room_def_remove(const struct room_def* room_to_remove) {
    if (!is_initialized || !room_to_remove) {
        return;
    }

    for (size_t i = 0; i < vector_len(&room_defs); ++i) {
        if (vector_get(&room_defs, i) == room_to_remove) {
            void* last_element = vector_pop(&room_defs);

            if (i < vector_len(&room_defs)) {
                vector_set(&room_defs, i, last_element);
            }

            TraceLog(LOG_DEBUG,
                     "ROOM_DEF: Removed '%s' from the generation pool.",
                     room_to_remove->model_path);
            return;
        }
    }
}

static struct room_attributes parse_attributes_from_filename(
    const char* filename) {
    struct room_attributes attr = {0};

    if (strstr(filename, "cross_room_0")) {
        attr.door_mask = DOOR_SOUTH | DOOR_EAST | DOOR_NORTH | DOOR_WEST;
        attr.weight = 5;
    } else if (strstr(filename, "deadend_0")) {
        attr.door_mask = DOOR_SOUTH;
        attr.weight = 2;
    } else if (strstr(filename, "deadend_90")) {
        attr.door_mask = DOOR_EAST;
        attr.weight = 2;
    } else if (strstr(filename, "deadend_180")) {
        attr.door_mask = DOOR_NORTH;
        attr.weight = 2;
    } else if (strstr(filename, "deadend_270")) {
        attr.door_mask = DOOR_WEST;
        attr.weight = 2;
    } else if (strstr(filename, "hallway_0")) {
        attr.door_mask = DOOR_NORTH | DOOR_SOUTH;
        attr.weight = 10;
    } else if (strstr(filename, "hallway_90")) {
        attr.door_mask = DOOR_EAST | DOOR_WEST;
        attr.weight = 10;
    } else if (strstr(filename, "L_room_0")) {
        attr.door_mask = DOOR_SOUTH | DOOR_EAST;
        attr.weight = 8;  // Medium weight
    } else if (strstr(filename, "L_room_90")) {
        attr.door_mask = DOOR_EAST | DOOR_NORTH;
        attr.weight = 8;
    } else if (strstr(filename, "L_room_180")) {
        attr.door_mask = DOOR_WEST | DOOR_NORTH;
        attr.weight = 8;
    } else if (strstr(filename, "L_room_270")) {
        attr.door_mask = DOOR_WEST | DOOR_SOUTH;
        attr.weight = 8;
    } else if (strstr(filename, "starting_room")) {
        attr.door_mask = DOOR_SOUTH;
        attr.weight = 0;
    } else {
        TraceLog(LOG_WARNING, "ROOM_DEF: Unrecognized room filename '%s'",
                 filename);
    }

    return attr;
}

static const struct room_def* select_weighted_random_from_matches(
    const struct vector* matches) {
    if (vector_is_empty(matches)) {
        return nullptr;
    }

    int total_weight = 0;
    for (size_t i = 0; i < vector_len(matches); ++i) {
        const struct room_def* def = vector_get(matches, i);
        total_weight += def->weight;
    }

    if (total_weight <= 0) {
        int rand_index = rng_get_range(0, (int)vector_len(matches) - 1);
        return vector_get(matches, rand_index);
    }

    int roll = rng_get_range(0, total_weight - 1);

    for (size_t i = 0; i < vector_len(matches); ++i) {
        const struct room_def* def = vector_get(matches, i);
        roll -= def->weight;
        if (roll < 0) {
            return def;
        }
    }

    return vector_get(matches, vector_len(matches) - 1);
}
