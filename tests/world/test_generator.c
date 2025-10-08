#include "game/world/generator.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game/world/grid.h"
#include "game/world/room_def.h"

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

#define TEST_DIR "test_generator_assets"

void create_dummy_file(const char* path) {
    FILE* f = fopen(path, "w");
    if (f)
        fclose(f);
}

void setup_full_environment(void) {
    MKDIR(TEST_DIR);

    create_dummy_file(TEST_DIR "/starting_room.glb");
    create_dummy_file(TEST_DIR "/hallway_0.glb");
    create_dummy_file(TEST_DIR "/hallway_90.glb");
    create_dummy_file(TEST_DIR "/cross_room_0.glb");

    grid_init();
    assert(room_def_load_all(TEST_DIR) > 0);
}

void teardown_full_environment(void) {
    grid_destroy();
    room_def_unload_all();

    (void)remove(TEST_DIR "/starting_room.glb");
    (void)remove(TEST_DIR "/hallway_0.glb");
    (void)remove(TEST_DIR "/hallway_90.glb");
    (void)remove(TEST_DIR "/cross_room_0.glb");
    RMDIR(TEST_DIR);
}

#define RUN_TEST(test)                          \
    do {                                        \
        printf("Running test: %s...\n", #test); \
        setup_full_environment();               \
        test();                                 \
        teardown_full_environment();            \
    } while (0)

void test_init_places_start_room_and_chunk(void) {
    assert(generator_init(123) == 0);

    struct world_cell* center = grid_get_cell(0, 0);
    assert(center != NULL);
    assert(strstr(center->template->model_path, "starting_room.glb"));

    int room_count = 0;
    for (int y = -2; y <= 2; y++) {
        for (int x = -2; x <= 2; x++) {
            if (grid_get_cell(x, y) != nullptr) {
                room_count++;
            }
        }
    }

    assert(room_count == 2);
}

void test_generation_is_deterministic(void) {
    char path_seed42[256];
    char path_seed_random[256];

    assert(generator_init(42) == 0);
    const struct room_def* template_run1 = grid_get_cell(0, -1)->template;
    assert(template_run1 != nullptr);

    strncpy(path_seed42, template_run1->model_path, sizeof(path_seed42) - 1);
    path_seed42[sizeof(path_seed42) - 1] = '\0';

    teardown_full_environment();
    setup_full_environment();

    assert(generator_init(123213213) == 0);
    const struct room_def* template_run2 = grid_get_cell(0, -1)->template;
    assert(template_run2 != nullptr);

    strncpy(path_seed_random, template_run2->model_path,
            sizeof(path_seed_random) - 1);
    path_seed_random[sizeof(path_seed_random) - 1] = '\0';

    teardown_full_environment();
    setup_full_environment();

    assert(generator_init(42) == 0);
    const struct room_def* template_run3 = grid_get_cell(0, -1)->template;
    assert(template_run3 != nullptr);
    const char* path_seed42_run2 = template_run3->model_path;

    assert(strcmp(path_seed42, path_seed42_run2) == 0);
    assert(strcmp(path_seed42, path_seed_random) != 0);
}

int main(void) {
    puts("Starting generator tests.\n");

    RUN_TEST(test_init_places_start_room_and_chunk);
    RUN_TEST(test_generation_is_deterministic);

    puts("\nAll generator tests passed successfully!");

    return EXIT_SUCCESS;
}
