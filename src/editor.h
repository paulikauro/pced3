#pragma once

#include "buffer.h"

typedef enum EditorMode {
    EM_NORMAL,
} EditorMode;

const char *editor_mode_to_string(EditorMode mode);

typedef struct Editor {
    EditorMode current_mode;
    Buffer *current_buffer;
} Editor;
