#include "game/world/world.h"

#include <errno.h>
#include <math.h>
#include <stdlib.h>

#include "raylib.h"

#include "game/world/generator.h"
#include "game/world/grid.h"
#include "game/world/room_def.h"

static const float ROOM_SIZE = 4.0F;
static const int LOAD_RADIUS = 1;

static int player_grid_x = -9999;
static int player_grid_y = -9999;
static bool is_initialized = false;

int world_init(unsigned int seed, const char* assets_path) {
    if (is_initialized) {
        return 0;
    }

    grid_init();
    if (room_def_load_all(assets_path) <= 0) {
        return -1;
    }

    if (generator_init(seed) != 0) {
        return -1;
    }

    world_update((Vector3){0});

    is_initialized = true;
    return 0;
}

void world_destroy(void) {
    if (!is_initialized) {
        return;
    }

    grid_destroy();
    room_def_unload_all();

    is_initialized = false;
}

int world_update(Vector3 player_pos) {
    if (!is_initialized) {
        return -EINVAL;
    }

    int new_grid_x = (int)roundf(player_pos.x / ROOM_SIZE);
    int new_grid_y = (int)roundf(player_pos.z / ROOM_SIZE);

    if (new_grid_x == player_grid_x && new_grid_y == player_grid_y) {
        return 0;
    }

    player_grid_x = new_grid_x;
    player_grid_y = new_grid_y;

    generator_create_chunk(player_grid_x, player_grid_y);

    const int scan_radius = LOAD_RADIUS + 2; 
    for (int y = player_grid_y - scan_radius; y <= player_grid_y + scan_radius; y++) {
        for (int x = player_grid_x - scan_radius; x <= player_grid_x + scan_radius; x++) {
            struct world_cell* cell = grid_get_cell(x, y);
            if (!cell) {
                continue;
            }

            int dist_x = abs(x - player_grid_x);
            int dist_y = abs(y - player_grid_y);

            if (dist_x <= LOAD_RADIUS && dist_y <= LOAD_RADIUS) {
                grid_load_model(cell);
            } else {
                grid_unload_model(cell);
            }
        }
    }

    return 0;
}

int world_draw(void) {
    if (!is_initialized) {
        return -EINVAL;
    }

    for (int y = player_grid_y - LOAD_RADIUS; y <= player_grid_y + LOAD_RADIUS; y++) {
        for (int x = player_grid_x - LOAD_RADIUS; x <= player_grid_x + LOAD_RADIUS; x++) {
            struct world_cell* cell = grid_get_cell(x, y);

            if (cell && (int)cell->is_model_loaded) {
                Vector3 room_pos = {
                    .x = (float)cell->grid_x * ROOM_SIZE,
                    .y = 0.0F,
                    .z = (float)cell->grid_y * ROOM_SIZE
                };
                DrawModel(cell->model, room_pos, 1.0F, WHITE);
            }
        }
    }

    return 0;
}
