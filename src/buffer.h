/* The buffer interface */
#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef struct Buffer {
    size_t capacity;
    char *data;
    size_t number_of_lines;
    size_t *line_indices;
} Buffer;

void buffer_from_file(Buffer *buffer, FILE *file);
void buffer_free(Buffer *buffer);
