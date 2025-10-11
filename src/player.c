#include "game/player.h"
#include <raylib.h>
#include <stdio.h>

void init_player(struct player* player,
                 Vector3 position,
                 float speed,
                 int health) {
    player->position = position;
    player->speed = speed;
    player->health = health;
    player->direction = EAST;
    player->moving = false;

    char filepath[128];

    for (int i = 0; i < NUM_DIRECTIONS; i++) {
    snprintf(filepath, sizeof(filepath), "assets/gifs/player/idle/idle_%d.gif", i*45);
    init_anim(&player->idle_anim[i], filepath, 8);

    snprintf(filepath, sizeof(filepath), "assets/gifs/player/run/run_%d.gif", i*45);
    init_anim(&player->run_anim[i], filepath, 4);

    snprintf(filepath, sizeof(filepath), "assets/gifs/player/death/death_%d.gif", i*45);
    init_anim(&player->death_anim[i], filepath, 6);

    snprintf(filepath, sizeof(filepath), "assets/gifs/player/attack/attack_%d.gif", i*45);
    init_anim(&player->attack_anim[i], filepath, 6);

    snprintf(filepath, sizeof(filepath), "assets/gifs/player/reload/reload_%d.gif", i*45);
    init_anim(&player->reload_anim[i], filepath, 6);
    }

}

void update_player(struct player* player) {
    bool moving = false;

    if (IsKeyDown(KEY_A)) {
        player->position.x -= player->speed;
        player->direction = WEST;
        moving = true;
    }
    if (IsKeyDown(KEY_D)) {
        player->position.x += player->speed;
        player->direction = EAST;
        moving = true;
    }
    if (IsKeyDown(KEY_W)) {
        player->position.z -= player->speed;
        player->direction = NORTH;
        moving = true;
    }
    if (IsKeyDown(KEY_S)) {
        player->position.z += player->speed;
        player->direction = SOUTH;
        moving = true;
    }

    player->moving = moving;

    if (moving) {
        update_anim(&player->run_anim[player->direction]);
    } else {
        update_anim(&player->idle_anim[player->direction]);
    }

}

void draw_player(struct player* player, Camera3D camera) {
    DrawBillboard(camera, player->moving ? player->run_anim[player->direction].tex_anim : player->idle_anim[player->direction].tex_anim,
                  player->position, 10.0F, WHITE);
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

