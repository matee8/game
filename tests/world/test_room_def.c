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
        setup_mock_assets();                    \
        test();                                 \
        teardown_mock_assets();                 \
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

    (void)snprintf(path, sizeof(path), "%s/%s/hallway_0.glb", TEST_DIR,
                   ROOMS_SUBDIR);
    create_dummy_file(path);
    (void)snprintf(path, sizeof(path), "%s/%s/L_room_270.glb", TEST_DIR,
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
    (void)snprintf(path, sizeof(path), "%s/%s/hallway_0.glb", TEST_DIR,
                   ROOMS_SUBDIR);
    (void)remove(path);
    (void)snprintf(path, sizeof(path), "%s/%s/L_room_270.glb", TEST_DIR,
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
        if (strstr(temp->model_path, "L_room_270.glb")) {
            found = true;
            assert(temp->door_mask == (DOOR_WEST | DOOR_SOUTH));
            break;
        }
    }
    assert(found);

    room_def_unload_all();
    assert(room_def_get_count() == 0);
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

void test_find_constrained(void) {
    char full_path[256];
    (void)snprintf(full_path, sizeof(full_path), "%s/%s", TEST_DIR,
                   ROOMS_SUBDIR);
    room_def_load_all(full_path);
    rng_init(42);

    const struct room_def* match = nullptr;

    uint8_t required1 = DOOR_SOUTH;
    uint8_t forbidden1 = DOOR_NORTH;
    match = room_def_find_constrained(required1, forbidden1);
    assert(match != nullptr);
    assert(strstr(match->model_path, "L_room_270.glb"));
    assert((match->door_mask & required1) == required1);
    assert((match->door_mask & forbidden1) == 0);

    uint8_t required2 = DOOR_SOUTH;
    uint8_t forbidden2 = DOOR_WEST;
    match = room_def_find_constrained(required2, forbidden2);
    assert(match != nullptr);
    assert(strstr(match->model_path, "hallway_0.glb"));
    assert((match->door_mask & required2) == required2);
    assert((match->door_mask & forbidden2) == 0);

    uint8_t required3 = DOOR_NORTH;
    uint8_t forbidden3 = DOOR_SOUTH;
    match = room_def_find_constrained(required3, forbidden3);
    assert(match == nullptr);

    uint8_t required4 = DOOR_EAST;
    uint8_t forbidden4 = 0;
    match = room_def_find_constrained(required4, forbidden4);
    assert(match == nullptr);

    room_def_unload_all();
}

void test_remove_room_def(void) {
    char full_path[256];
    (void)snprintf(full_path, sizeof(full_path), "%s/%s", TEST_DIR,
                   ROOMS_SUBDIR);

    room_def_load_all(full_path);
    assert(room_def_get_count() == 3);

    const struct room_def* to_remove = nullptr;
    for (size_t i = 0; i < room_def_get_count(); i++) {
        const struct room_def* temp = room_def_get_by_index(i);
        if (strstr(temp->model_path, "L_room_270.glb")) {
            to_remove = temp;
            break;
        }
    }

    assert(to_remove != nullptr);

    room_def_remove(to_remove);

    assert(room_def_get_count() == 2);

    bool was_found_after_remove = false;
    for (size_t i = 0; i < room_def_get_count(); i++) {
        if (room_def_get_by_index(i) == to_remove) {
            was_found_after_remove = true;
            break;
        }
    }
    assert(was_found_after_remove == false);

    room_def_remove(to_remove);
    assert(room_def_get_count() == 2);

    room_def_remove(nullptr);
    assert(room_def_get_count() == 2);

    room_def_unload_all();
}

int main(void) {
    puts("Starting room_def tests.\n");

    RUN_TEST(test_load_and_unload);
    RUN_TEST(test_double_load_and_unload);
    RUN_TEST(test_find_constrained);
    RUN_TEST(test_remove_room_def);

    puts("\nAll room_def tests passed successfully!");
    return EXIT_SUCCESS;
}
