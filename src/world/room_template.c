#include "game/world/room_template.h"

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#include "game/ds/vector.h"

static struct vector room_templates;
static bool is_initialized = false;

static uint8_t parse_mask_from_filename(const char* filename);

int room_template_load_all(const char* directory_path) {
    if (is_initialized) {
        TraceLog(LOG_WARNING, "ROOM_TEMPLATE: Templates already initialized.");
        return (int)vector_len(&room_templates);
    }

    if (vector_init(&room_templates) != 0) {
        TraceLog(LOG_ERROR,
                 "ROOM_TEMPLATE: Failed to initialize vector (out of memory).");
        return -ENOMEM;
    }

    int ret = 0;
    FilePathList files = LoadDirectoryFiles(directory_path);

    if (!files.paths) {
        TraceLog(LOG_ERROR, "ROOM_TEMPLATE: Failed to load directory: %s",
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
        uint8_t mask = parse_mask_from_filename(filename);
        if (mask == 0) {
            continue;
        }

        struct room_template* template = malloc(sizeof(struct room_template));
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

        template->door_mask = mask;
        if (vector_push(&room_templates, template) != 0) {
            free(template->model_path);
            free(template);
            ret = -ENOMEM;
            goto cleanup_files;
        }
    }

    is_initialized = true;
    TraceLog(LOG_INFO, "ROOM_TEMPLATE: Loaded %zu room templates.",
             vector_len(&room_templates));
    ret = (int)vector_len(&room_templates);

cleanup_files:
    UnloadDirectoryFiles(files);

cleanup_vector:
    if (ret < 0) {
        room_template_unload_all();
    }

    return ret;
}

void room_template_unload_all() {
    if (!is_initialized) {
        return;
    }

    for (size_t i = 0; i < vector_len(&room_templates); ++i) {
        struct room_template* template = vector_get(&room_templates, i);
        free(template->model_path);
        free(template);
    }

    vector_destroy(&room_templates);
    is_initialized = false;
    TraceLog(LOG_INFO, "ROOM_TEMPLATE: Unloaded all room templates.");
}

size_t room_template_get_count(void) {
    return (int)is_initialized ? vector_len(&room_templates) : 0;
}

const struct room_template* room_template_get_by_index(size_t index) {
    if (!is_initialized) {
        return nullptr;
    }

    return (const struct room_template*)vector_get(&room_templates, index);
}

const struct room_template* room_template_find_matching(
    uint8_t required_doors) {
    if (!is_initialized) {
        return nullptr;
    }

    struct vector matches;
    if (vector_init(&matches) != 0) {
        return nullptr;
    }

    for (size_t i = 0; i < vector_len(&room_templates); ++i) {
        const struct room_template* template = vector_get(&room_templates, i);
        if (template->door_mask == required_doors) {
            vector_push(&matches, (void*)template);
        }
    }

    const struct room_template* result = nullptr;
    if (!vector_is_empty(&matches)) {
        size_t rand_index = GetRandomValue(0, (int)vector_len(&matches) - 1);
        result = (const struct room_template*)vector_get(&matches, rand_index);
    }

    vector_destroy(&matches);
    return result;
}

static uint8_t parse_mask_from_filename(const char* filename) {
    if (strstr(filename, "hallway_0")) {
        return DOOR_EAST | DOOR_WEST;
    }
    if (strstr(filename, "hallway_90")) {
        return DOOR_NORTH | DOOR_SOUTH;
    }
    if (strstr(filename, "thickhallway_0")) {
        return DOOR_EAST | DOOR_WEST;
    }
    if (strstr(filename, "thickhallway_90")) {
        return DOOR_NORTH | DOOR_SOUTH;
    }
    if (strstr(filename, "L_room_0")) {
        return DOOR_NORTH | DOOR_EAST;
    }
    if (strstr(filename, "L_room_90")) {
        return DOOR_NORTH | DOOR_WEST;
    }
    if (strstr(filename, "L_room_180")) {
        return DOOR_SOUTH | DOOR_WEST;
    }
    if (strstr(filename, "L_room_270")) {
        return DOOR_SOUTH | DOOR_EAST;
    }
    if (strstr(filename, "deadend_0")) {
        return DOOR_EAST;
    }
    if (strstr(filename, "deadend_90")) {
        return DOOR_NORTH;
    }
    if (strstr(filename, "deadend_180")) {
        return DOOR_WEST;
    }
    if (strstr(filename, "deadend_270")) {
        return DOOR_SOUTH;
    }
    if (strstr(filename, "cross_room")) {
        return DOOR_NORTH | DOOR_SOUTH | DOOR_EAST | DOOR_WEST;
    }
    if (strstr(filename, "cube_room")) {
        return DOOR_NORTH | DOOR_SOUTH | DOOR_EAST | DOOR_WEST;
    }
    if (strstr(filename, "starting_room")) {
        return DOOR_NORTH | DOOR_SOUTH | DOOR_EAST | DOOR_WEST;
    }

    TraceLog(LOG_WARNING, "ROOM_TEMPLATE: Unrecognized room filename '%s'",
             filename);
    return 0;
}
