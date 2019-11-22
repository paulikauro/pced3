#pragma once

#include "buffer.h"

typedef enum EditorMode {
    EM_NORMAL,
} EditorMode;

static inline char *editor_mode_to_string(EditorMode mode) {
    extern char *editor_mode_strings[];
    return editor_mode_strings[mode];
}

typedef struct Editor {
    EditorMode current_mode;
    Buffer *current_buffer;
} Editor;
