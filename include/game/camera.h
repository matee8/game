#ifndef CAMERA_H
#define CAMERA_H

#include <raylib.h>
#include "room.h"

typedef struct camera {
    Camera3D camera_m;
    room* prev_current_room;
}camera;

void init_camera(camera* camera);

void update_camera(camera* camera, room* current_room);

#endif
