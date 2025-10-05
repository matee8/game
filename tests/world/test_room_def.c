#include "game/world/room_def.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game/rng.h"

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

static const char* TEST_DIR = "test_def_assets";
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

    int count = room_def_load_all(full_path);
    assert(count == 3);
    assert(room_def_get_count() == 3);

    bool found = false;
    for (size_t i = 0; i < room_def_get_count(); i++) {
        const struct room_def* temp = room_def_get_by_index(i);
        if (strstr(temp->model_path, "L_room_180.glb")) {
            found = true;
            assert(temp->door_mask == (DOOR_SOUTH | DOOR_WEST));
            break;
        }
    }
    assert(found);

    room_def_unload_all();
    assert(room_def_get_count() == 0);
}

void test_find_matching(void) {
    char full_path[256];
    (void)snprintf(full_path, sizeof(full_path), "%s/%s", TEST_DIR,
                   ROOMS_SUBDIR);
    room_def_load_all(full_path);

    const uint8_t deadend_mask = DOOR_EAST;
    const struct room_def* match = room_def_find_matching(deadend_mask);
    assert(match != NULL);
    assert(match->door_mask == deadend_mask);
    assert(strstr(match->model_path, "deadend_0.glb"));

    const uint8_t no_match_mask = DOOR_NORTH | DOOR_EAST | DOOR_WEST;
    match = room_def_find_matching(no_match_mask);
    assert(match == NULL);

    room_def_unload_all();
}

void test_double_load_and_unload(void) {
    char full_path[256];
    (void)snprintf(full_path, sizeof(full_path), "%s/%s", TEST_DIR,
                   ROOMS_SUBDIR);

    assert(room_def_load_all(full_path) == 3);
    assert(room_def_load_all(full_path) == 3);

    room_def_unload_all();

    room_def_unload_all();
    assert(room_def_get_count() == 0);
}

void test_find_compatible(void) {
    char full_path[256];
    (void)snprintf(full_path, sizeof(full_path), "%s/%s", TEST_DIR,
                   ROOMS_SUBDIR);
    room_def_load_all(full_path);
    rng_init(123);

    const uint8_t south_door_req = DOOR_SOUTH;
    const struct room_def* match = room_def_find_compatible(south_door_req);
    assert(match != nullptr);
    assert((match->door_mask & south_door_req) == south_door_req);
    assert(strstr(match->model_path, "L_room_180.glb"));

    const uint8_t north_door_req = DOOR_NORTH;
    match = room_def_find_compatible(north_door_req);
    assert(match != nullptr);
    assert((match->door_mask & north_door_req) == north_door_req);
    assert(strstr(match->model_path, "hallway_90.glb"));

    const uint8_t impossible_req = DOOR_NORTH | DOOR_EAST;
    match = room_def_find_compatible(impossible_req);
    assert(match == nullptr);

    room_def_unload_all();
}

int main(void) {
    puts("Starting room_def tests.\n");
    setup_mock_assets();

    RUN_TEST(test_load_and_unload);
    RUN_TEST(test_find_matching);
    RUN_TEST(test_double_load_and_unload);
    RUN_TEST(test_find_compatible);

    teardown_mock_assets();

    puts("\nAll room_def tests passed successfully!");
    return EXIT_SUCCESS;
}
