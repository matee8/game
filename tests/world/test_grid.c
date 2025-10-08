#include "game/world/grid.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define RUN_TEST(test)                          \
    do {                                        \
        printf("Running test: %s...\n", #test); \
        test();                                 \
    } while (0)

static struct room_def mock_template_1 = {
    .model_path = "assets/models/rooms/hallway_0.glb",
    .door_mask = 0};
static struct room_def mock_template_2 = {
    .model_path = "assets/models/rooms/cube_room_0.glb",
    .door_mask = 0};

void test_init_and_destroy(void) {
    grid_init();
    grid_destroy();
}

void test_place_and_get_room(void) {
    assert(grid_init() == 0);

    assert(grid_place_room(0, 0, &mock_template_1) == 0);
    struct world_cell* cell = grid_get_cell(0, 0);

    assert(cell != nullptr);
    assert(cell->template == &mock_template_1);

    grid_destroy();
}

void test_get_non_existent_cell(void) {
    grid_init();
    assert(grid_get_cell(10, -10) == nullptr);
    grid_destroy();
}

void test_place_on_existing_cell_is_ignored(void) {
    assert(grid_init() == 0);

    assert(grid_place_room(5, 5, &mock_template_1) == 0);

    assert(grid_place_room(5, 5, &mock_template_2) == 0);

    struct world_cell* cell = grid_get_cell(5, 5);
    assert(cell != nullptr);
    assert(cell->template == &mock_template_1);

    grid_destroy();
}

void test_model_loading_and_unloading(void) {
    grid_init();
    grid_place_room(1, 1, &mock_template_2);
    struct world_cell* cell = grid_get_cell(1, 1);

    assert(cell->is_model_loaded == false);

    grid_load_model(cell);
    assert(cell->is_model_loaded == true);
    assert(cell->model.meshCount > 0);

    grid_unload_model(cell);
    assert(cell->is_model_loaded == false);
    assert(cell->model.meshCount == 0);

    grid_destroy();
}

void test_destroy_unloads_models(void) {
    grid_init();
    grid_place_room(-2, -2, &mock_template_1);
    struct world_cell* cell = grid_get_cell(-2, -2);

    grid_load_model(cell);
    assert(cell->is_model_loaded == true);

    grid_destroy();
}

void test_invalid_arguments(void) {
    assert(grid_init() == 0);

    assert(grid_place_room(1, 1, nullptr) == -EINVAL);

    assert(grid_load_model(nullptr) == -EINVAL);
    assert(grid_unload_model(nullptr) == -EINVAL);

    grid_destroy();
}

int main(void) {
    InitWindow(100, 100, "World Grid Test");

    puts("Starting grid tests.\n");

    RUN_TEST(test_init_and_destroy);
    RUN_TEST(test_place_and_get_room);
    RUN_TEST(test_get_non_existent_cell);
    RUN_TEST(test_place_on_existing_cell_is_ignored);
    RUN_TEST(test_model_loading_and_unloading);
    RUN_TEST(test_destroy_unloads_models);
    RUN_TEST(test_invalid_arguments);

    puts("\nAll grid tests passed successfully!");

    CloseWindow();
    return EXIT_SUCCESS;
}
