#include "game/ds/dstring.h"

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static const size_t SSO_SIZE = sizeof(((struct dstring*)0)->data.sso);

static char* get_content_ptr(struct dstring* sb);
static size_t get_capacity(const struct dstring* sb);
static int ensure_capacity(struct dstring* sb, size_t additional_len);

int dstring_init(struct dstring* sb) {
    if (!sb) {
        return -EINVAL;
    }

    sb->len = 0;
    sb->on_heap = false;
    sb->data.sso[0] = '\0';

    return 0;
}

void dstring_destroy(struct dstring* sb) {
    if (!sb) {
        return;
    }

    if (sb->on_heap) {
        free(sb->data.heap.ptr);
        sb->data.heap.ptr = nullptr;
    }

    dstring_init(sb);
}

int dstring_append(struct dstring* sb, const char* str, size_t len) {
    if (!sb || (!str && len > 0)) {
        return -EINVAL;
    }

    if (len == 0) {
        return 0;
    }

    int result = ensure_capacity(sb, len);
    if (result != 0) {
        return result;
    }

    char* dest = get_content_ptr(sb);
    memcpy(dest + sb->len, str, len);

    sb->len += len;
    dest[sb->len] = '\0';

    return 0;
}

int dstring_append_cstr(struct dstring* sb, const char* str) {
    if (!str) {
        return -EINVAL;
    }

    return dstring_append(sb, str, strlen(str));
}

const char* dstring_get_cstr(const struct dstring* sb) {
    if (!sb) {
        return "";
    }
    if (sb->on_heap) {
        return sb->data.heap.ptr;
    }
    return sb->data.sso;
}

char* dstring_to_cstr(struct dstring* sb) {
    if (!sb) {
        return nullptr;
    }

    char* result_ptr = nullptr;

    if (sb->on_heap) {
        result_ptr = realloc(sb->data.heap.ptr, sb->len + 1);
        if (!result_ptr) {
            return nullptr;
        }
    } else {
        result_ptr = strdup(sb->data.sso);
        if (!result_ptr) {
            return nullptr;
        }
    }

    (void)dstring_init(sb);

    return result_ptr;
}

size_t dstring_get_len(const struct dstring* sb) {
    if (!sb) {
        return 0;
    }

    return sb->len;
}

static char* get_content_ptr(struct dstring* sb) {
    if (sb->on_heap) {
        return sb->data.heap.ptr;
    }

    return sb->data.sso;
}

static size_t get_capacity(const struct dstring* sb) {
    if (sb->on_heap) {
        return sb->data.heap.capacity;
    }

    return SSO_SIZE - 1;
}

static int ensure_capacity(struct dstring* sb, size_t additional_len) {
    const size_t required_len = sb->len + additional_len;
    const size_t current_capacity = get_capacity(sb);

    if (required_len <= current_capacity) {
        return 0;
    }

    size_t new_capacity = current_capacity * 2;
    if (new_capacity < required_len) {
        new_capacity = required_len;
    }

    char* new_ptr = malloc(new_capacity + 1);
    if (!new_ptr) {
        return -ENOMEM;
    }

    const char* old_ptr = get_content_ptr(sb);
    memcpy(new_ptr, old_ptr, sb->len);

    if (sb->on_heap) {
        free(sb->data.heap.ptr);
    }

    sb->on_heap = true;
    sb->data.heap.ptr = new_ptr;
    sb->data.heap.capacity = new_capacity;

    return 0;
}
