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
    free(buffer->line_indices);
    memset(buffer, 0, sizeof(*buffer));
}

size_t buffer_line_length(Buffer *buffer, size_t line) {
    assert(buffer != NULL);
    assert(1 <= line && line <= buffer->number_of_lines);
    /* This works because line_indices[0] = 0 and
     * line_indices[number_of_lines] = capacity.
     */
    return buffer->line_indices[line + 1] - buffer->line_indices[line];
}

uint32_t buffer_get(Buffer *buffer, size_t line, size_t column) {
    assert(buffer != NULL);
    assert(1 <= line && line <= buffer->number_of_lines);
    /* this is an expensive assert */
    assert(1 <= column && column <= buffer_line_length(buffer, line));
    /* We subtract 1 from column since it uses 1-based indexing. */
    size_t location = buffer->line_indices[line] + (column - 1);
    return buffer->data[location];
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
    /* Indices start from one and are padded by one in each side. */
    size_t index_buffer_size = buffer->number_of_lines + 2;
    buffer->line_indices = allocate(index_buffer_size * sizeof(size_t));
    /* padding */
    buffer->line_indices[0] = 0;
    /* first actual line */
    buffer->line_indices[1] = 0;
    size_t line = 1;
    for (size_t i = 0; i < buffer->capacity; i++) {
        char c = buffer->data[i];
        if (c == '\n') {
            line++;
            /* TODO: might point past the buffer */
            buffer->line_indices[line] = i + 1;
        }
    }
    buffer->line_indices[buffer->number_of_lines + 1] = buffer->capacity;
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
