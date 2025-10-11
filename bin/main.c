/*******************************************************************************************
 *
 *   raylib [core] example - basic window
 *
 *   Example complexity rating: [★☆☆☆] 1/4
 *
 *   Welcome to raylib!
 *
 *   To test examples, just press F6 and execute 'raylib_compile_execute' script
 *   Note that compiled executable is placed in the same folder as .c file
 *
 *   To test the examples on Web, press F6 and execute
 *'raylib_compile_execute_web' script Web version of the program is generated in
 *the same folder as .c file
 *
 *   You can find all basic examples on C:\raylib\raylib\examples folder or
 *   raylib official webpage: www.raylib.com
 *
 *   Enjoy using raylib. :)
 *
 *   Example originally created with raylib 1.0, last time updated with
 *raylib 1.0
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an
 *OSI-certified, BSD-like license that allows static linking with closed source
 *software
 *
 *   Copyright (c) 2013-2025 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

#include <raylib.h>
#include <stdlib.h>

#include "game/camera.h"
#include "game/player.h"
#include "game/room.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight,
               "raylib [core] example - basic window");
    struct camera camera;
    init_camera(&camera);

    struct player player;
    init_player(&player, (Vector3){0, 0.1F, 0}, 0.1F, 100);

    struct room room1;
    struct room room2;
    struct room room3;
    struct room* neighbors1[6] = {&room2, NULL, NULL, NULL, &room3, NULL};
    struct room* neighbors2[6] = {NULL, &room1, NULL, NULL, NULL, NULL};
    struct room* neighbors3[6] = {NULL, NULL, NULL, NULL, NULL, &room1};

    init_room(&room1, (Vector3){0, 0, 0}, 6, neighbors1,
              "assets/models/rooms/cross_room_0.glb", NULL);

    BoundingBox box = get_world_box(&room1);
    float roomLength = box.max.x - box.min.x;

    init_room(&room2,
              (Vector3){room1.position.x + roomLength, room1.position.y,
                        room1.position.z},
              6, neighbors2, "assets/models/rooms/cross_room_0.glb", NULL);

    init_room(&room3,
              (Vector3){room1.position.x, room1.position.y,
                        room1.position.z + roomLength},
              6, neighbors3, "assets/models/rooms/cross_room_0.glb", NULL);

    struct room* current_room = &room1;

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        update_player(&player);
        update_room(&current_room, &player);
        update_camera(&camera, current_room);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera.camera_m);

        draw_player(&player, camera.camera_m);
        DrawModel(room1.model, room1.position, room1.scale, RED);
        DrawModel(room2.model, room2.position, room2.scale, BLACK);
        DrawModel(room3.model, room3.position, room3.scale, BLUE);
        DrawBoundingBox(room1.bounds, GREEN);
        DrawBoundingBox(get_world_box(&room2), GREEN);

        DrawGrid(20, 10.0F);  // Draw a grid

        EndMode3D();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // UnloadTexture(player.texture);
    unload_player(&player);
    UnloadModel(room1.model);
    UnloadModel(room2.model);
    UnloadModel(room3.model);
    

    CloseWindow();  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
