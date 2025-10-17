#include <stdlib.h>

#include "raylib.h"

#include "game/camera.h"
#include "game/player.h"
#include "game/world/world.h"

int main(void) {
    const int screen_width = 1280;
    const int screen_height = 720;

    InitWindow(screen_width, screen_height, "Varázspuli");

    struct camera camera;
    init_camera(&camera);

    Model room1 = LoadModel("assets/test/chiproom0.gltf");
    room1.materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;
    room1.materials[0].maps[MATERIAL_MAP_ALBEDO].value = 0.5f;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera.camera_m);

        DrawModel(room1, (Vector3){0.0F, 0.0F, 0.0F}, 6.0F, WHITE);

        DrawGrid(100, 10.0F);

        EndMode3D();

        DrawFPS(10, 10);

        EndDrawing();
    }

    world_destroy();

    CloseWindow();

    return EXIT_SUCCESS;
}
