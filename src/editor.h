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

typedef enum Direction {
    DIR_LEFT,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN,
} Direction;

typedef struct Position {
    size_t line;
    size_t column;
} Position;

typedef struct Editor {
    EditorMode current_mode;
    /* nullable */
    Buffer *current_buffer;
    Position position;
} Editor;

void editor_init(Editor *editor);
void editor_free(Editor *editor);
void editor_load_file(Editor *editor, char *filename);

/* editor commands */
void editor_move(Editor *editor, Direction direction);
