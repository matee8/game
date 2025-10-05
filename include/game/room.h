#ifndef ROOM_H
#define ROOM_H

#include <raylib.h>
#include "player.h"

enum neighbor {
    RIGHT,
    LEFT,
    UP,
    DOWN,
    FORWARD,
    BACKWARD
};

typedef struct room {
    Model model;
    BoundingBox bounds;
    float scale;
    Vector3 position;
    struct room* neighbors[6];
}room;

void init_room(room* room, Vector3 position, float scale, struct room* neighbors[], char* model_path, char* texture_path);

BoundingBox get_world_box(room* room);

Vector3 get_center(room* room);

void update_room(room** room, player* player);

#endif
