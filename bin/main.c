#include <stdlib.h>

#include "raylib.h"

#include "game/camera.h"
#include "game/player.h"
#include "game/world/world.h"

int main(void) {
    const int screen_width = 1280;
    const int screen_height = 720;

    InitWindow(screen_width, screen_height, "Varázspuli");

    if (world_init(40, "assets/models/rooms") != 0) {
        TraceLog(LOG_ERROR, "Failed to initialize game world. Exiting.");
        CloseWindow();
        return -1;
    }

    struct player player;
    init_player(&player, (Vector3){0, 0.1F, 0}, 0.1F, 100);

    struct camera camera;
    init_camera(&camera);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        update_player(&player);
        world_update(player.position);

        Vector3 current_room_center = world_get_room_center(player.position);

        camera_focus_on(&camera, current_room_center);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera.camera_m);

        world_draw();
        DrawModel(player.model, player.position, 0.1F, WHITE);

        DrawGrid(100, 10.0F);

        EndMode3D();

        DrawFPS(10, 10);

        EndDrawing();
    }

    world_destroy();
    UnloadModel(player.model);

    CloseWindow();

    return EXIT_SUCCESS;
}
