#include "game/world/room_template.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#if defined(_WIN32)
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#define RMDIR(path) _rmdir(path)
#else
#include <sys/stat.h>
#include <unistd.h>
#define MKDIR(path) mkdir(path, 0755)
#define RMDIR(path) rmdir(path)
#endif

#define RUN_TEST(test)                          \
    do {                                        \
        printf("Running test: %s...\n", #test); \
        test();                                 \
    } while (0)

static const char* TEST_DIR = "test_template_assets";
static const char* ROOMS_SUBDIR = "rooms";

void create_dummy_file(const char* path) {
    FILE* f = fopen(path, "w");
    if (f) {
        (void)fclose(f);
    }
}

void setup_mock_assets(void) {
    MKDIR(TEST_DIR);
    char path[256];
    (void)snprintf(path, sizeof(path), "%s/%s", TEST_DIR, ROOMS_SUBDIR);
    MKDIR(path);

    (void)snprintf(path, sizeof(path), "%s/%s/hallway_90.glb", TEST_DIR,
                   ROOMS_SUBDIR);
    create_dummy_file(path);
    (void)snprintf(path, sizeof(path), "%s/%s/L_room_180.glb", TEST_DIR,
                   ROOMS_SUBDIR);
    create_dummy_file(path);
    (void)snprintf(path, sizeof(path), "%s/%s/deadend_0.glb", TEST_DIR,
                   ROOMS_SUBDIR);
    create_dummy_file(path);
    (void)snprintf(path, sizeof(path), "%s/%s/notes.txt", TEST_DIR,
                   ROOMS_SUBDIR);
    create_dummy_file(path);
}

void teardown_mock_assets(void) {
    char path[256];
    (void)snprintf(path, sizeof(path), "%s/%s/hallway_90.glb", TEST_DIR,
                   ROOMS_SUBDIR);
    (void)remove(path);
    (void)snprintf(path, sizeof(path), "%s/%s/L_room_180.glb", TEST_DIR,
                   ROOMS_SUBDIR);
    (void)remove(path);
    (void)snprintf(path, sizeof(path), "%s/%s/deadend_0.glb", TEST_DIR,
                   ROOMS_SUBDIR);
    (void)remove(path);
    (void)snprintf(path, sizeof(path), "%s/%s/notes.txt", TEST_DIR,
                   ROOMS_SUBDIR);
    (void)remove(path);
    (void)snprintf(path, sizeof(path), "%s/%s", TEST_DIR, ROOMS_SUBDIR);
    RMDIR(path);
    RMDIR(TEST_DIR);
}

void test_load_and_unload(void) {
    char full_path[256];
    (void)snprintf(full_path, sizeof(full_path), "%s/%s", TEST_DIR,
                   ROOMS_SUBDIR);

    int count = room_template_load_all(full_path);
    assert(count == 3);
    assert(room_template_get_count() == 3);

    bool found = false;
    for (size_t i = 0; i < room_template_get_count(); i++) {
        const struct room_template* temp = room_template_get_by_index(i);
        if (strstr(temp->model_path, "L_room_180.glb")) {
            found = true;
            assert(temp->door_mask == (DOOR_SOUTH | DOOR_WEST));
            break;
        }
    }
    assert(found);

    room_template_unload_all();
    assert(room_template_get_count() == 0);
}

void test_find_matching(void) {
    char full_path[256];
    (void)snprintf(full_path, sizeof(full_path), "%s/%s", TEST_DIR,
                   ROOMS_SUBDIR);
    room_template_load_all(full_path);

    const uint8_t deadend_mask = DOOR_EAST;
    const struct room_template* match =
        room_template_find_matching(deadend_mask);
    assert(match != NULL);
    assert(match->door_mask == deadend_mask);
    assert(strstr(match->model_path, "deadend_0.glb"));

    const uint8_t no_match_mask = DOOR_NORTH | DOOR_EAST | DOOR_WEST;
    match = room_template_find_matching(no_match_mask);
    assert(match == NULL);

    room_template_unload_all();
}

void test_double_load_and_unload(void) {
    char full_path[256];
    (void)snprintf(full_path, sizeof(full_path), "%s/%s", TEST_DIR,
                   ROOMS_SUBDIR);

    assert(room_template_load_all(full_path) == 3);
    assert(room_template_load_all(full_path) == 3);

    room_template_unload_all();

    room_template_unload_all();
    assert(room_template_get_count() == 0);
}

int main(void) {
    puts("Starting room_template tests.\n");
    setup_mock_assets();

    RUN_TEST(test_load_and_unload);
    RUN_TEST(test_find_matching);
    RUN_TEST(test_double_load_and_unload);

    teardown_mock_assets();

    puts("\nAll room_template tests passed successfully!");
    return EXIT_SUCCESS;
}
