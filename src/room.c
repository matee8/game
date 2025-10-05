#include "../include/game/room.h"
#include <raylib.h>
#include <stdlib.h>
#include <string.h>

void init_room(room* room,
               Vector3 position,
               float scale,
               struct room* neighbors[],
               char* model_path,
               char* texture_path) {
    Model model = LoadModel(model_path);

    if (texture_path) {
        Texture2D texture = LoadTexture(texture_path);
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    }

    room->model = model;
    room->position = position;
    room->scale = scale;
    room->bounds = get_world_box(room);
    if (neighbors) {
        memcpy(room->neighbors, neighbors, sizeof(room->neighbors));
    }
}

BoundingBox get_world_box(room* room) {
    BoundingBox b = GetModelBoundingBox(room->model);
    b.min.x = room->position.x + b.min.x * room->scale;
    b.min.y = room->position.y + b.min.y * room->scale;
    b.min.z = room->position.z + b.min.z * room->scale;
    b.max.x = room->position.x + b.max.x * room->scale;
    b.max.y = room->position.y + b.max.y * room->scale;
    b.max.z = room->position.z + b.max.z * room->scale;
    return b;
}

Vector3 get_center(room* room) {
    return (Vector3){
        (room->bounds.min.x + room->bounds.max.x) / 2.0f + room->position.x,
        (room->bounds.min.y + room->bounds.max.y) / 2.0f + room->position.y,
        (room->bounds.min.z + room->bounds.max.z) / 2.0f + room->position.z};
}

void update_room(room** current_room, player* player) {
    BoundingBox b = (*current_room)->bounds;
    Vector3 pos = player->position;

    // +X irány
    if (pos.x > b.max.x && (*current_room)->neighbors[0] != NULL) {
        *current_room = (*current_room)->neighbors[0];
        player->position.x =
            (*current_room)->bounds.min.x + 0.1f;  // belépési pozíció
    }
    // -X irány
    else if (pos.x < b.min.x && (*current_room)->neighbors[1] != NULL) {
        *current_room = (*current_room)->neighbors[1];
        player->position.x = (*current_room)->bounds.max.x - 0.1f;
    }
    // +Y irány
    else if (pos.y > b.max.y && (*current_room)->neighbors[2] != NULL) {
        *current_room = (*current_room)->neighbors[2];
        player->position.y = (*current_room)->bounds.min.y + 0.1f;
    }
    // -Y irány
    else if (pos.y < b.min.y && (*current_room)->neighbors[3] != NULL) {
        *current_room = (*current_room)->neighbors[3];
        player->position.y = (*current_room)->bounds.max.y - 0.1f;
    }
    // +Z irány
    else if (pos.z > b.max.z && (*current_room)->neighbors[4] != NULL) {
        *current_room = (*current_room)->neighbors[4];
        player->position.z = (*current_room)->bounds.min.z + 0.1f;
    }
    // -Z irány
    else if (pos.z < b.min.z && (*current_room)->neighbors[5] != NULL) {
        *current_room = (*current_room)->neighbors[5];
        player->position.z = (*current_room)->bounds.max.z - 0.1f;
    }
}
