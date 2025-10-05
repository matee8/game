#include "../include/game/player.h"

void InitPlayer(Player* player, Vector3 position, float speed, int health, Model model) {
    player->position = position;
    player->speed = speed;
    player->health = health;
    player->direction = 0; // Default direction
    player->model = model;
}

void UpdatePlayer(Player* player) {
    // Update player position based on input
    if (IsKeyDown(KEY_W)) player->position.z -= player->speed;
    if (IsKeyDown(KEY_S)) player->position.z += player->speed;
    if (IsKeyDown(KEY_A)) player->position.x -= player->speed;
    if (IsKeyDown(KEY_D)) player->position.x += player->speed;

    // Update player model position
    //player->model.transform.position = player->position;
}