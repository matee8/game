#include "../include/game/player.h"
#include <raylib.h>

void init_player(struct player* player,
                 Vector3 position,
                 float speed,
                 int health) {
    Model model = LoadModel("assets/models/player/player.glb");
    Texture2D texture = LoadTexture("assets/textures/player/player.png");
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    player->position = position;
    player->speed = speed;
    player->health = health;
    player->direction = 0;
    player->model = model;
    player->texture = texture;
}

void update_player(struct player* player) {
    if (IsKeyDown(KEY_W)) {
        player->position.z -= player->speed;
    }
    if (IsKeyDown(KEY_S)) {
        player->position.z += player->speed;
    }
    if (IsKeyDown(KEY_A)) {
        player->position.x -= player->speed;
    }
    if (IsKeyDown(KEY_D)) {
        player->position.x += player->speed;
    }
}
