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

enum direction get_direction(int dx, int dz) {
    if (dz < 0 && dx == 0) return NORTH;
    else if (dz < 0 && dx > 0) return NORTHEAST;
    else if (dz == 0 && dx > 0) return EAST;
    else if (dz > 0 && dx > 0) return SOUTHEAST;
    else if (dz > 0 && dx == 0) return SOUTH;
    else if (dz > 0 && dx < 0) return SOUTHWEST;
    else if (dz == 0 && dx < 0) return WEST;
    else if (dz < 0 && dx < 0) return NORTHWEST;
    return player->direction;
}

void update_player(struct player* player) {
    if (player->state == STATE_DEAD) {
        update_anim(&player->death_anim[player->direction]);
        return;
    }

    if (IsKeyDown(KEY_W)) dz -= 1;
    if (IsKeyDown(KEY_S)) dz += 1;
    if (IsKeyDown(KEY_A)) dx -= 1;
    if (IsKeyDown(KEY_D)) dx += 1;

    int dx = 0, dz = 0;

    if (IsKeyDown(KEY_W)) dz -= 1;
    if (IsKeyDown(KEY_S)) dz += 1;
    if (IsKeyDown(KEY_A)) dx -= 1;
    if (IsKeyDown(KEY_D)) dx += 1;

    bool moving = (dx != 0 || dz != 0);

    // Mozgás frissítése
    if (moving) {
        float length = sqrtf((float)(dx*dx + dz*dz));
        player->position.x += (dx / length) * player->speed;
        player->position.z += (dz / length) * player->speed;
        player->direction = get_direction(dx, dz);
        player->state = STATE_RUNNING;
    } else if (player->state == STATE_RUNNING) {
        player->state = STATE_IDLE;
    }

    if (IsKeyPressed(KEY_SPACE)) player->state = STATE_ATTACKING;
    if (IsKeyPressed(KEY_R))     player->state = STATE_RELOADING;
    if (player->health <= 0)     player->state = STATE_DEAD;

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

