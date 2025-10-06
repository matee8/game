#include "../include/game/camera.h"
#include "../include/game/room.h"

void init_camera(struct camera* camera) {
    camera->camera_m.position = (Vector3){0.0F, 15.0F, 15.0F};
    camera->camera_m.target = (Vector3){0.0F, 0.0F, 0.0F};
    camera->camera_m.up = (Vector3){0.0F, 1.0F, 0.0F};
    camera->camera_m.fovy = 45.0F;
    camera->camera_m.projection = CAMERA_PERSPECTIVE;
}

void update_camera(struct camera* camera, struct room* current_room) {
    if (camera->prev_current_room != current_room) {
        Vector3 center = get_center(current_room);

        camera->camera_m.position =
            (Vector3){center.x + 0.0F,  // oldalról + kicsit felülről
                      center.y + 15.0F, center.z + 15.0F};

        camera->camera_m.target = center;
        camera->prev_current_room = current_room;
    }
}
