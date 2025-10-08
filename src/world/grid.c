#include "game/world/grid.h"

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include "raylib.h"

#include "game/ds/hashmap.h"

static struct hashmap grid;
static bool is_initialized = false;

static inline uint64_t grid_key(int x, int y);

int grid_init() {
    if (is_initialized) {
        return 0;
    }

    if (hashmap_init(&grid) != 0) {
        return -ENOMEM;
    }

    is_initialized = true;

    return 0;
}

void grid_destroy(void) {
    if (!is_initialized) {
        return;
    }

    size_t iter = 0;
    uint64_t key = 0;
    void* value = nullptr;
    while (hashmap_iter(&grid, &iter, &key, &value)) {
        struct world_cell* cell = (struct world_cell*)value;
        if (cell->is_model_loaded) {
            UnloadModel(cell->model);
        }
        free(cell);
    }

    hashmap_destroy(&grid);
    is_initialized = false;
}

int grid_place_room(int x, int y, const struct room_def* room_template) {
    if (!is_initialized) {
        TraceLog(LOG_WARNING, "GRID: Called place_room before initialization.");
        return -ECANCELED;
    }

    if (!room_template) {
        return -EINVAL;
    }

    uint64_t key = grid_key(x, y);
    if (hashmap_get(&grid, key) != nullptr) {
        return 0;
    }

    struct world_cell* cell = malloc(sizeof(struct world_cell));
    if (!cell) {
        TraceLog(LOG_ERROR, "GRID: Failed to allocate memory for world cell.");
        return -ENOMEM;
    }

    cell->template = room_template;
    cell->is_model_loaded = false;
    cell->model = (Model){0};
    cell->grid_x = x;
    cell->grid_y = y;

    if (hashmap_set(&grid, key, cell, nullptr) != 0) {
        free(cell);
        return -ENOMEM;
    }

    return 0;
}

struct world_cell* grid_get_cell(int x, int y) {
    if (!is_initialized) {
        return nullptr;
    }

    return (struct world_cell*)hashmap_get(&grid, grid_key(x, y));
}

int grid_load_model(struct world_cell* cell) {
    if (!cell) {
        return -EINVAL;
    }

    if (cell->is_model_loaded) {
        return 0;
    }

    cell->model = LoadModel(cell->template->model_path);
    cell->is_model_loaded = true;

    return 0;
}

int grid_unload_model(struct world_cell* cell) {
    if (!cell) {
        return -EINVAL;
    }

    if (!cell->is_model_loaded) {
        return 0;
    }

    UnloadModel(cell->model);
    cell->model = (Model){0};
    cell->is_model_loaded = false;

    return 0;
}

static inline uint64_t grid_key(int x, int y) {
    return ((uint64_t)(x) << 32U) | (uint32_t)(y);
}
