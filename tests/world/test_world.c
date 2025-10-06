#include "raylib.h"

#include "game/world/world.h"

int main(void) {
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Procedural World Test");

    if (world_init(1238972181, "assets/models/rooms") != 0) {
        TraceLog(LOG_ERROR, "Failed to initialize game world. Exiting.");
        CloseWindow();
        return -1;
    }

    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0F, 10.0F, 10.0F };
    camera.target = (Vector3){ 0.0F, 0.0F, 0.0F };
    camera.up = (Vector3){ 0.0F, 1.0F, 0.0F };
    camera.fovy = 60.0F;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FREE);

        world_update(camera.position);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        world_draw();
        DrawGrid(100, 10.0F);

        EndMode3D();

        DrawFPS(10, 10);
        DrawText("Free camera controls: Move (W,A,S,D), Look (Mouse)", 10, 40, 20, DARKGRAY);

        EndDrawing();
    }

    world_destroy();
    CloseWindow();

    return 0;
}
