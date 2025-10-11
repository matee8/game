#include "game/player.h"
#include <raylib.h>

void init_player(struct player* player,
                 Vector3 position,
                 float speed,
                 int health) {
    player->position = position;
    player->speed = speed;
    player->health = health;
    player->direction = EAST;

    init_anim(&player->idle_anim, "assets/gifs/player/idle/idle_0.gif", 8);
    init_anim(&player->run_anim, "assets/gifs/player/run/run_45.gif", 4);
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
        update_anim(&player->run_anim);
    } else {
        update_anim(&player->idle_anim);
    }

}

void draw_player(struct player* player, Camera3D camera) {
    DrawBillboard(camera, player->moving ? player->run_anim.tex_anim : player->idle_anim.tex_anim,
                  player->position, 10.0F, WHITE);
}

void unload_player(struct player* player) {
    unload_anim(&player->idle_anim);
    unload_anim(&player->run_anim);
}

