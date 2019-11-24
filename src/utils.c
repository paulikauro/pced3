#include <stdio.h>

#include "utils.h"

_Noreturn void error_std(const char *message) {
    perror(message);
    abort();
}

void *allocate(size_t size) {
    // Zero-sized allocations are not allowed here,
    // because malloc is free to return either NULL
    // or a valid pointer for it, and NULL will cause
    // a crash here.
    assert(size > 0);
    void *ptr;
    if ((ptr = malloc(size)) == NULL) {
        error_std("malloc");
    }
    return ptr;
}
