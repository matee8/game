#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

typedef struct Player{
    Vector3 position;
    float speed;
    int health;
    Model model;
} Player;

void InitPlayer(Player* player, Vector3 position, float speed, int health, Model model) {}
void UpdatePlayer(Player* player) {}

#endif

