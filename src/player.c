#include "game/player.h"
#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "raymath.h"

#include "game/camera.h"

enum { FILEPATH_SIZE = 128 };

// Helper function to safely format a file path
static void format_filepath(char* buffer,
                            size_t size,
                            const char* pattern,
                            int angle) {
    int written = snprintf(buffer, size, pattern, angle);
    if (written < 0 || written >= (int)size) {
        printf("Error formatting filepath: %s\n", pattern);
    }
}

void init_player(struct player* player,
                 Vector3 position,
                 float speed,
                 int health) {
    player->position = position;
    player->speed = speed;
    player->health = health;
    player->direction = EAST;

    char filepath[FILEPATH_SIZE];

    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        format_filepath(filepath, sizeof(filepath),
                        "assets/gifs/player/idle/idle_%d.gif", i * 45);
        init_anim(&player->idle_anim[i], filepath, 8);

        format_filepath(filepath, sizeof(filepath),
                        "assets/gifs/player/run/run_%d.gif", i * 45);
        init_anim(&player->run_anim[i], filepath, 4);

        format_filepath(filepath, sizeof(filepath),
                        "assets/gifs/player/death/death_%d.gif", i * 45);
        init_anim(&player->death_anim[i], filepath, 6);

        format_filepath(filepath, sizeof(filepath),
                        "assets/gifs/player/attack/attack_%d.gif", i * 45);
        init_anim(&player->attack_anim[i], filepath, 6);

        format_filepath(filepath, sizeof(filepath),
                        "assets/gifs/player/reload/reload_%d.gif", i * 45);
        init_anim(&player->reload_anim[i], filepath, 6);
    }
}

enum direction get_direction(struct player* player, int dx, int dz) {
    if (dz < 0 && dx == 0) {
        return NORTH;
    }
    if (dz < 0 && dx > 0) {
        return NORTH_EAST;
    }
    if (dz == 0 && dx > 0) {
        return EAST;
    }
    if (dz > 0 && dx > 0) {
        return SOUTH_EAST;
    }
    if (dz > 0 && dx == 0) {
        return SOUTH;
    }
    if (dz > 0 && dx < 0) {
        return SOUTH_WEST;
    }
    if (dz == 0 && dx < 0) {
        return WEST;
    }
    if (dz < 0 && dx < 0) {
        return NORTH_WEST;
    }
    return player->direction;
}

void update_player(struct player* player, const struct camera* camera) {
    if (player->state == STATE_DEAD) {
        update_anim(&player->death_anim[player->direction]);
        return;
    }

    Vector3 cam_forward =
        Vector3Subtract(camera->camera_m.target, camera->camera_m.position);
    cam_forward.y = 0;
    cam_forward = Vector3Normalize(cam_forward);

    Vector3 cam_right =
        Vector3CrossProduct(cam_forward, (Vector3){0.0f, 1.0f, 0.0f});

    Vector3 move_dir = {0};
    if (IsKeyDown(KEY_W)) {
        move_dir = Vector3Add(move_dir, cam_forward);
    }
    if (IsKeyDown(KEY_S)) {
        move_dir = Vector3Subtract(move_dir, cam_forward);
    }
    if (IsKeyDown(KEY_A)) {
        move_dir = Vector3Subtract(move_dir, cam_right);
    }
    if (IsKeyDown(KEY_D)) {
        move_dir = Vector3Add(move_dir, cam_right);
    }

    if (Vector3LengthSqr(move_dir) > 0) {
        move_dir = Vector3Normalize(move_dir);

        Vector3 next_position =
            Vector3Add(player->position, Vector3Scale(move_dir, player->speed));

        player->position = next_position;

        player->state = STATE_RUNNING;
    } else if (player->state == STATE_RUNNING) {
        player->state = STATE_IDLE;
    }

    if (IsKeyPressed(KEY_SPACE)) {
        player->state = STATE_ATTACKING;
    }
    if (IsKeyPressed(KEY_R)) {
        player->state = STATE_RELOADING;
    }
    if (player->health <= 0) {
        player->state = STATE_DEAD;
    }

    switch (player->state) {
        case STATE_IDLE:
            update_anim(&player->idle_anim[player->direction]);
            break;
        case STATE_RUNNING:
            update_anim(&player->run_anim[player->direction]);
            break;
        case STATE_ATTACKING:
            update_anim(&player->attack_anim[player->direction]);
            break;
        case STATE_RELOADING:
            update_anim(&player->reload_anim[player->direction]);
            break;
        case STATE_DEAD:
            update_anim(&player->death_anim[player->direction]);
            break;
    }
}

void draw_player(struct player* player, Camera3D camera) {
    struct anim* current_anim = NULL;

    switch (player->state) {
        case STATE_RUNNING:
            current_anim = &player->run_anim[player->direction];
            break;
        case STATE_ATTACKING:
            current_anim = &player->attack_anim[player->direction];
            break;
        case STATE_RELOADING:
            current_anim = &player->reload_anim[player->direction];
            break;
        case STATE_DEAD:
            current_anim = &player->death_anim[player->direction];
            break;
        case STATE_IDLE:
        default:
            current_anim = &player->idle_anim[player->direction];
            break;
    }

    if (current_anim != NULL) {
        DrawBillboard(camera, current_anim->tex_anim, player->position, 10.0F,
                      WHITE);
    }
}

void unload_player(struct player* player) {
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        unload_anim(&player->idle_anim[i]);
        unload_anim(&player->run_anim[i]);
        unload_anim(&player->death_anim[i]);
        unload_anim(&player->attack_anim[i]);
        unload_anim(&player->reload_anim[i]);
    }
}
