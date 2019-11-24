/* The buffer interface */
#pragma once

#include <stdint.h>
#include <stdio.h>

typedef struct Buffer {
    size_t capacity;
    char *data;
    size_t number_of_lines;
    size_t *line_indices;
} Buffer;

void buffer_from_file(Buffer *buffer, FILE *file);
void buffer_free(Buffer *buffer);
/* `line` is in range [1, number_of_lines]. */
size_t buffer_line_length(Buffer *buffer, size_t line);
uint32_t buffer_get(Buffer *buffer, size_t line, size_t column);
