#include "game/ds/hashmap.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define RUN_TEST(test)                          \
    do {                                        \
        printf("Running test: %s...\n", #test); \
        test();                                 \
    } while (0)

void test_init_and_destroy(void) {
    struct hashmap map;
    assert(hashmap_init(&map) == 0);
    assert(hashmap_len(&map) == 0);
    assert(hashmap_capacity(&map) > 0);
    hashmap_destroy(&map);
    assert(map.entries == nullptr);
}

void test_set_and_get_single_entry(void) {
    struct hashmap map;
    int value = 42;
    assert(hashmap_init(&map) == 0);
    assert(hashmap_set(&map, 100, &value, nullptr) == 0);
    assert(hashmap_len(&map) == 1);

    void* result = hashmap_get(&map, 100);
    assert(result == &value);
    assert(*(int*)result == 42);

    hashmap_destroy(&map);
}

void test_get_non_existent_key(void) {
    struct hashmap map;
    assert(hashmap_init(&map) == 0);
    assert(hashmap_get(&map, 12345) == nullptr);
    hashmap_destroy(&map);
}

void test_update_existing_key(void) {
    struct hashmap map;
    int val1 = 1;
    int val2 = 2;
    void* old_val = nullptr;

    assert(hashmap_init(&map) == 0);
    assert(hashmap_set(&map, 1, &val1, nullptr) == 0);
    assert(hashmap_get(&map, 1) == &val1);

    assert(hashmap_set(&map, 1, &val2, &old_val) == 0);
    assert(hashmap_len(&map) == 1);
    assert(hashmap_get(&map, 1) == &val2);
    assert(old_val == &val1);

    hashmap_destroy(&map);
}

void test_remove_entry(void) {
    struct hashmap map;
    int val = 123;
    assert(hashmap_init(&map) == 0);
    assert(hashmap_set(&map, 1, &val, nullptr) == 0);

    void* removed = hashmap_remove(&map, 1);
    assert(removed == &val);
    assert(hashmap_len(&map) == 0);
    assert(hashmap_get(&map, 1) == nullptr);

    assert(hashmap_remove(&map, 1) == nullptr);

    hashmap_destroy(&map);
}

void test_collision_handling(void) {
    struct hashmap map;
    int values[] = {1, 2, 3};
    assert(hashmap_init(&map) == 0);

    uint64_t key1 = 1;
    uint64_t key2 = key1 + hashmap_capacity(&map);
    uint64_t key3 = key2 + hashmap_capacity(&map);

    assert(hashmap_set(&map, key1, &values[0], nullptr) == 0);
    assert(hashmap_set(&map, key2, &values[1], nullptr) == 0);
    assert(hashmap_set(&map, key3, &values[2], nullptr) == 0);

    assert(hashmap_len(&map) == 3);
    assert(hashmap_get(&map, key1) == &values[0]);
    assert(hashmap_get(&map, key2) == &values[1]);
    assert(hashmap_get(&map, key3) == &values[2]);

    assert(hashmap_remove(&map, key2) == &values[1]);
    assert(hashmap_len(&map) == 2);
    assert(hashmap_get(&map, key1) == &values[0]);
    assert(hashmap_get(&map, key3) == &values[2]);
    assert(hashmap_get(&map, key2) == nullptr);

    hashmap_destroy(&map);
}

void test_resizing(void) {
    struct hashmap map;
    assert(hashmap_init(&map) == 0);
    size_t initial_capacity = hashmap_capacity(&map);

    int values[30];
    for (int i = 0; i < 30; i++) {
        values[i] = i;
        assert(hashmap_set(&map, i, &values[i], nullptr) == 0);
    }

    assert(hashmap_len(&map) == 30);
    assert(hashmap_capacity(&map) > initial_capacity);

    for (int i = 0; i < 30; i++) {
        assert(hashmap_get(&map, i) == &values[i]);
    }

    hashmap_destroy(&map);
}

void test_null_args(void) {
    int val = 1;
    assert(hashmap_init(nullptr) == -EINVAL);
    hashmap_destroy(nullptr);
    assert(hashmap_set(nullptr, 1, &val, nullptr) == -EINVAL);
    assert(hashmap_get(nullptr, 1) == nullptr);
    assert(hashmap_remove(nullptr, 1) == nullptr);
    assert(hashmap_len(nullptr) == 0);
    assert(hashmap_capacity(nullptr) == 0);
}

void test_iterator(void) {
    struct hashmap map;
    int values[] = {10, 20, 30, 40, 50};
    assert(hashmap_init(&map) == 0);

    size_t iter = 0;
    uint64_t key = 0;
    void* value = nullptr;
    assert(hashmap_iter(&map, &iter, &key, &value) == false);

    hashmap_set(&map, 1, &values[0], nullptr);
    hashmap_set(&map, 2, &values[1], nullptr);
    hashmap_set(&map, 3, &values[2], nullptr);
    hashmap_set(&map, 4, &values[3], nullptr);

    hashmap_remove(&map, 2);

    iter = 0;
    int found_count = 0;
    bool found_keys[5] = {false};

    while (hashmap_iter(&map, &iter, &key, &value)) {
        found_count++;
        assert(key >= 1 && key <= 4);
        found_keys[key] = true;

        int* val_ptr = (int*)value;
        assert(*val_ptr == (int)key * 10);
    }

    assert(found_count == 3);
    assert(found_keys[1] == true);
    assert(found_keys[2] == false);
    assert(found_keys[3] == true);
    assert(found_keys[4] == true);

    hashmap_destroy(&map);
}

int main(void) {
    puts("Starting hashmap tests.\n");

    RUN_TEST(test_init_and_destroy);
    RUN_TEST(test_set_and_get_single_entry);
    RUN_TEST(test_get_non_existent_key);
    RUN_TEST(test_update_existing_key);
    RUN_TEST(test_remove_entry);
    RUN_TEST(test_collision_handling);
    RUN_TEST(test_resizing);
    RUN_TEST(test_null_args);
    RUN_TEST(test_iterator);

    puts("\nAll hashmap tests passed successfully!");

    return EXIT_SUCCESS;
}
