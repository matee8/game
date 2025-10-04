#include "game/ds/dstring.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RUN_TEST(test)                          \
    do {                                        \
        printf("Running test: %s...\n", #test); \
        test();                                 \
    } while (0)

static const size_t SSO_CAPACITY = sizeof(((struct dstring*)0)->data.sso);

void test_init_succeeds_on_valid_struct(void) {
    struct dstring sb;
    int ret = dstring_init(&sb);
    assert(ret == 0);
    assert(sb.len == 0);
    assert(sb.on_heap == false);
    assert(dstring_get_len(&sb) == 0);
    assert(dstring_get_cstr(&sb)[0] == '\0');
}

void test_init_fails_on_null_pointer(void) {
    assert(dstring_init(nullptr) == -EINVAL);
}

void test_destroy_is_safe_on_null_pointer(void) {
    dstring_destroy(nullptr);
}

void test_destroy_is_idempotent(void) {
    struct dstring sb;
    dstring_init(&sb);
    dstring_append_cstr(&sb, "test");
    dstring_destroy(&sb);
    assert(sb.len == 0 && sb.on_heap == false);
    dstring_destroy(&sb);
    assert(sb.len == 0 && sb.on_heap == false);
}

void test_append_sso_within_capacity(void) {
    struct dstring sb;
    dstring_init(&sb);

    assert(dstring_append_cstr(&sb, "hello") == 0);
    assert(sb.len == 5);
    assert(sb.on_heap == false);
    assert(strcmp(dstring_get_cstr(&sb), "hello") == 0);

    dstring_destroy(&sb);
}

void test_append_sso_fills_capacity_exactly(void) {
    struct dstring sb;
    dstring_init(&sb);

    char content[SSO_CAPACITY];
    memset(content, 'a', SSO_CAPACITY - 1);
    content[SSO_CAPACITY - 1] = '\0';

    assert(dstring_append_cstr(&sb, content) == 0);
    assert(sb.len == SSO_CAPACITY - 1);
    assert(sb.on_heap == false);
    assert(strcmp(dstring_get_cstr(&sb), content) == 0);

    dstring_destroy(&sb);
}

void test_append_sso_overflow_triggers_heap_transition(void) {
    struct dstring sb;
    dstring_init(&sb);

    char content[SSO_CAPACITY];
    memset(content, 'a', SSO_CAPACITY - 1);
    content[SSO_CAPACITY - 1] = '\0';
    dstring_append_cstr(&sb, content);

    assert(dstring_append_cstr(&sb, "b") == 0);
    assert(sb.len == SSO_CAPACITY);
    assert(sb.on_heap == true);
    assert(dstring_get_len(&sb) == SSO_CAPACITY);

    dstring_destroy(&sb);
}

void test_append_large_string_goes_directly_to_heap(void) {
    struct dstring sb;
    dstring_init(&sb);

    char large_content[100];
    memset(large_content, 'x', 99);
    large_content[99] = '\0';

    assert(dstring_append_cstr(&sb, large_content) == 0);
    assert(sb.len == 99);
    assert(sb.on_heap == true);
    assert(strcmp(dstring_get_cstr(&sb), large_content) == 0);

    dstring_destroy(&sb);
}

void test_append_on_heap_triggers_reallocation(void) {
    struct dstring sb;
    dstring_init(&sb);

    char large_content[100];
    memset(large_content, 'x', 99);
    large_content[99] = '\0';
    dstring_append_cstr(&sb, large_content);

    const size_t initial_capacity = sb.data.heap.capacity;
    assert(sb.on_heap == true);

    char overflow_content[initial_capacity];
    memset(overflow_content, 'y', initial_capacity - 1);
    overflow_content[initial_capacity - 1] = '\0';

    assert(dstring_append_cstr(&sb, overflow_content) == 0);
    assert(sb.on_heap == true);
    assert(sb.len == 99 + initial_capacity - 1);

    dstring_destroy(&sb);
}

void test_append_fails_on_null_buffer(void) {
    assert(dstring_append(nullptr, "a", 1) == -EINVAL);
    assert(dstring_append_cstr(nullptr, "a") == -EINVAL);
}

void test_append_fails_on_null_content(void) {
    struct dstring sb;
    dstring_init(&sb);
    assert(dstring_append(&sb, nullptr, 1) == -EINVAL);
    assert(dstring_append_cstr(&sb, nullptr) == -EINVAL);
    dstring_destroy(&sb);
}

void test_append_empty_string_is_a_noop(void) {
    struct dstring sb;
    dstring_init(&sb);
    dstring_append_cstr(&sb, "content");

    assert(dstring_append(&sb, "more", 0) == 0);
    assert(sb.len == 7);
    assert(strcmp(dstring_get_cstr(&sb), "content") == 0);

    assert(dstring_append_cstr(&sb, "") == 0);
    assert(sb.len == 7);
    assert(strcmp(dstring_get_cstr(&sb), "content") == 0);

    dstring_destroy(&sb);
}

void test_to_cstr_from_sso_succeeds(void) {
    struct dstring sb;
    dstring_init(&sb);
    dstring_append_cstr(&sb, "sso_string");

    assert(sb.on_heap == false);

    char* owned_str = dstring_to_cstr(&sb);
    assert(owned_str != nullptr);
    assert(strcmp(owned_str, "sso_string") == 0);

    assert(sb.len == 0);
    assert(sb.on_heap == false);
    assert(dstring_get_cstr(&sb)[0] == '\0');

    free(owned_str);
}

void test_to_cstr_from_heap_succeeds(void) {
    struct dstring sb;
    dstring_init(&sb);

    char large_content[100];
    memset(large_content, 'z', 99);
    large_content[99] = '\0';
    dstring_append_cstr(&sb, large_content);

    assert(sb.on_heap == true);

    char* owned_str = dstring_to_cstr(&sb);
    assert(owned_str != nullptr);
    assert(strcmp(owned_str, large_content) == 0);

    assert(sb.len == 0);
    assert(sb.on_heap == false);
    assert(dstring_get_cstr(&sb)[0] == '\0');

    free(owned_str);
}

void test_to_cstr_on_null_buffer(void) {
    assert(dstring_to_cstr(nullptr) == nullptr);
}

int main(void) {
    puts("Starting dstring tests.\n");

    RUN_TEST(test_init_succeeds_on_valid_struct);
    RUN_TEST(test_init_fails_on_null_pointer);
    RUN_TEST(test_destroy_is_safe_on_null_pointer);
    RUN_TEST(test_destroy_is_idempotent);

    RUN_TEST(test_append_sso_within_capacity);
    RUN_TEST(test_append_sso_fills_capacity_exactly);

    RUN_TEST(test_append_sso_overflow_triggers_heap_transition);
    RUN_TEST(test_append_large_string_goes_directly_to_heap);
    RUN_TEST(test_append_on_heap_triggers_reallocation);

    RUN_TEST(test_append_fails_on_null_buffer);
    RUN_TEST(test_append_fails_on_null_content);
    RUN_TEST(test_append_empty_string_is_a_noop);

    RUN_TEST(test_to_cstr_from_sso_succeeds);
    RUN_TEST(test_to_cstr_from_heap_succeeds);
    RUN_TEST(test_to_cstr_on_null_buffer);

    puts("\nAll dstring tests passed successfully!");

    return EXIT_SUCCESS;
}
