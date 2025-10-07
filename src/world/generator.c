#include "game/world/generator.h"

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#include "game/ds/vector.h"
#include "game/rng.h"
#include "game/world/grid.h"
#include "game/world/room_def.h"

struct frontier_cell {
    int x;
    int y;
};

int generator_init(unsigned int seed) {
    rng_init(seed);

    const struct room_def* start = nullptr;

    for (size_t i = 0; i < room_def_get_count(); i++) {
        const struct room_def* temp = room_def_get_by_index(i);
        if (strstr(temp->model_path, "starting_room.glb")) {
            start = temp;
            break;
        }
    }

    if (start == nullptr) {
        TraceLog(LOG_ERROR,
                 "GENERATOR: 'starting_room.glb' not found in templates.");
        return -ENOENT;
    }

    room_def_remove(start);

    grid_place_room(0, 0, start);
    if (generator_create_chunk(0, 0) != 0) {
        TraceLog(LOG_ERROR, "GENERATOR: Initial chunk generation failed.");
        return -1;
    }

    return 0;
}

int generator_create_chunk(int center_x, int center_y) {
    struct vector frontiers;

    if (vector_init(&frontiers) != 0) {
        TraceLog(LOG_ERROR,
                 "GENERATOR: Failed to initialize frontiers vector.");
        return -ENOMEM;
    }

    int ret = 0;

    for (int y = center_y - 2; y <= center_y + 2; ++y) {
        for (int x = center_x - 2; x <= center_x + 2; ++x) {
            if (grid_get_cell(x, y) != nullptr) {
                continue;
            }

            if (grid_get_cell(x, y + 1) || grid_get_cell(x, y - 1) ||
                grid_get_cell(x + 1, y) || grid_get_cell(x - 1, y)) {
                struct frontier_cell* cell =
                    malloc(sizeof(struct frontier_cell));
                if (!cell) {
                    TraceLog(LOG_ERROR,
                             "GENERATOR: Failed to allocate memory for "
                             "frontier_cell.");
                    ret = -ENOMEM;
                    goto cleanup;
                }
                cell->x = x;
                cell->y = y;

                if (vector_push(&frontiers, cell) != 0) {
                    TraceLog(
                        LOG_ERROR,
                        "GENERATOR: Failed ot push frontier cell to vector.");
                    free(cell);
                    ret = -ENOMEM;
                    goto cleanup;
                }
            }
        }
    }

    for (int i = (int)vector_len(&frontiers) - 1; i > 0; i--) {
        size_t j = rng_get_range(0, i);
        void* temp = vector_get(&frontiers, i);

        vector_set(&frontiers, i, vector_get(&frontiers, j));
        vector_set(&frontiers, j, temp);
    }

    for (size_t i = 0; i < vector_len(&frontiers); ++i) {
        struct frontier_cell* cell_pos = vector_get(&frontiers, i);

        uint8_t required_doors = 0;
        uint8_t forbidden_doors = 0;

        struct world_cell* north = grid_get_cell(cell_pos->x, cell_pos->y + 1);
        if (north && (north->template->door_mask & DOOR_SOUTH)) {
            required_doors |= DOOR_NORTH;
        } else if (north) {
            forbidden_doors |= DOOR_NORTH;
        }

        struct world_cell* south = grid_get_cell(cell_pos->x, cell_pos->y - 1);
        if (south && (south->template->door_mask & DOOR_NORTH)) {
            required_doors |= DOOR_SOUTH;
        } else if (south) {
            forbidden_doors |= DOOR_SOUTH;
        }

        struct world_cell* east = grid_get_cell(cell_pos->x + 1, cell_pos->y);
        if (east && (east->template->door_mask & DOOR_WEST)) {
            required_doors |= DOOR_EAST;
        } else if (east) {
            forbidden_doors |= DOOR_EAST;
        }

        struct world_cell* west = grid_get_cell(cell_pos->x - 1, cell_pos->y);
        if (west && (west->template->door_mask & DOOR_EAST)) {
            required_doors |= DOOR_WEST;
        } else if (west) {
            forbidden_doors |= DOOR_WEST;
        }

        if (required_doors == 0) {
            continue;
        }

        const struct room_def* compatible =
            room_def_find_constrained(required_doors, forbidden_doors);
        if (compatible) {
            grid_place_room(cell_pos->x, cell_pos->y, compatible);
        }
    }

cleanup:
    for (size_t i = 0; i < vector_len(&frontiers); ++i) {
        free(vector_get(&frontiers, i));
    }
    vector_destroy(&frontiers);

    return ret;
}
