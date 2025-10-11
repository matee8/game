#include "game/camera.h"

#include "raymath.h"

void init_camera(struct camera* camera) {
    camera->camera_m.position = (Vector3){0.0F, 15.0F, 15.0F};
    camera->camera_m.target = (Vector3){0.0F, 0.0F, 0.0F};
    camera->camera_m.up = (Vector3){0.0F, 1.0F, 0.0F};
    camera->camera_m.fovy = 45.0F;
    camera->camera_m.projection = CAMERA_PERSPECTIVE;

    camera->target_position = camera->camera_m.position;
    camera->target_center = camera->camera_m.target;
}

void camera_focus_on(struct camera* camera, Vector3 new_center) {
    if (camera->target_center.x != new_center.x ||
        camera->target_center.y != new_center.y ||
        camera->target_center.z != new_center.z) {
        camera->target_center = new_center;

        camera->target_position = (Vector3){
            .x = camera->target_center.x + 0.0F,
            .y = camera->target_center.y + 15.0F,
            .z = camera->target_center.z + 15.0F,
        };
    }

    float lerp_factor = 0.1F;

    camera->camera_m.position.x = Lerp(camera->camera_m.position.x,
                                       camera->target_position.x, lerp_factor);
    camera->camera_m.position.y = Lerp(camera->camera_m.position.y,
                                       camera->target_position.y, lerp_factor);
    camera->camera_m.position.z = Lerp(camera->camera_m.position.z,
                                       camera->target_position.z, lerp_factor);

    camera->camera_m.target.x =
        Lerp(camera->camera_m.target.x, camera->target_center.x, lerp_factor);
    camera->camera_m.target.y =
        Lerp(camera->camera_m.target.y, camera->target_center.y, lerp_factor);
    camera->camera_m.target.z =
        Lerp(camera->camera_m.target.z, camera->target_center.z, lerp_factor);
}
