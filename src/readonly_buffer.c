/* A read-only buffer implementation. */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "utils.h"

long file_size(FILE *file);
void populate_line_indices(Buffer *buffer);
void count_lines(Buffer *buffer);

void buffer_from_file(Buffer *buffer, FILE *file) {
    assert(buffer != NULL && file != NULL);
    long size = file_size(file);
    char *data = allocate(size);
    fread(data, 1, size, file);
    if (ferror(file)) {
        error_std("fread");
    }
    buffer->capacity = size;
    buffer->data = data;
    populate_line_indices(buffer);
}

void buffer_free(Buffer *buffer) {
    assert(buffer != NULL);
    free(buffer->data);
    memset(buffer, 0, sizeof(*buffer));
}

long file_size(FILE *file) {
    long size;
    if (fseek(file, 0, SEEK_END) == -1) {
        error_std("fseek");
    }
    if ((size = ftell(file)) == -1) {
        error_std("ftell");
    }
    if (fseek(file, 0, SEEK_SET) == -1) {
        error_std("fseek");
    }
    return size;
}

void populate_line_indices(Buffer *buffer) {
    /* TODO: this is inefficient and ugly */
    count_lines(buffer);
    buffer->line_indices = allocate(buffer->number_of_lines * sizeof(size_t));
    buffer->line_indices[0] = 0;
    size_t line = 0;
    for (size_t i = 0; i < buffer->capacity; i++) {
        char c = buffer->data[i];
        if (c == '\n') {
            line++;
            /* TODO: might point past the buffer */
            buffer->line_indices[line] = i + 1;
        }
    }
}

void count_lines(Buffer *buffer) {
    /* There is always at least one line. */
    buffer->number_of_lines = 1;
    for (size_t i = 0; i < buffer->capacity; i++) {
        char c = buffer->data[i];
        if (c == '\n') {
            buffer->number_of_lines++;
        }
    }
}
