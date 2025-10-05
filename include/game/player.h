#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

typedef struct player{
    Vector3 position;
    float speed;
    int health;
    int direction;
    Model model;
    Texture2D texture;
} player;

void init_player(player* player, Vector3 position, float speed, int health);
void update_player(player* player);

#endif

