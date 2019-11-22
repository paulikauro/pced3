#pragma once

#include "buffer.h"

typedef enum EditorMode {
    EM_NORMAL,
} EditorMode;

static inline
char *editor_mode_to_string(EditorMode mode) {
    extern char *editor_mode_strings[];
    return editor_mode_strings[mode];
}

typedef struct Editor {
    EditorMode current_mode;
    /* nullable */
    Buffer *current_buffer;
} Editor;

void editor_init(Editor *editor);
void editor_free(Editor *editor);
void editor_load_file(Editor *editor, char *filename);
