#include "../include/game/camera.h"
#include "../include/game/room.h"

#include <stdlib.h>

void init_camera(struct camera* camera) {
    camera->camera_m.position = (Vector3){0.0F, 15.0F, 15.0F};
    camera->camera_m.target = (Vector3){0.0F, 0.0F, 0.0F};
    camera->camera_m.up = (Vector3){0.0F, 1.0F, 0.0F};
    camera->camera_m.fovy = 45.0F;
    camera->camera_m.projection = CAMERA_PERSPECTIVE;

    camera->prev_current_room = NULL;
    camera->prev_position = camera->camera_m.position;
    camera->target_position = camera->camera_m.position;
    camera->prev_center = camera->camera_m.target;
    camera->target_center = camera->camera_m.target;
}

void update_camera(struct camera* camera, struct room* current_room) {
    if (camera->prev_current_room != current_room) {
        camera->prev_center = camera->target_center;
        camera->target_center = get_center(current_room);

        camera->target_position = (Vector3){
            camera->target_center.x + 0.0F,  // oldalról + kicsit felülről
            camera->target_center.y + 15.0F, camera->target_center.z + 15.0F};

        camera->prev_current_room = current_room;
    }

    float lerp = 0.1F;

    camera->prev_position.x +=
        (camera->target_position.x - camera->prev_position.x) * lerp;
    camera->prev_position.y +=
        (camera->target_position.y - camera->prev_position.y) * lerp;
    camera->prev_position.z +=
        (camera->target_position.z - camera->prev_position.z) * lerp;

    camera->prev_center.x +=
        (camera->target_center.x - camera->prev_center.x) * lerp;
    camera->prev_center.y +=
        (camera->target_center.y - camera->prev_center.y) * lerp;
    camera->prev_center.z +=
        (camera->target_center.z - camera->prev_center.z) * lerp;

    camera->camera_m.position = camera->prev_position;
    camera->camera_m.target = camera->prev_center;
}
