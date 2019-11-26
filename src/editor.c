#include <string.h>

#include "editor.h"
#include "utils.h"

// TODO: possibly move EditorMode here
char *editor_mode_strings[] = {
    [EM_NORMAL] = "NORMAL",
};

// TODO: do something with this
#define MIN(x, y) ((x) < (y) ? (x) : (y))

static void cap_column(Editor *editor);

void editor_init(Editor *editor) {
    assert(editor != NULL);
    editor->current_buffer = NULL;
    editor->current_mode = EM_NORMAL;
    editor->position.line = 1;
    editor->position.column = 1;
}

void editor_free(Editor *editor) {
    if (editor->current_buffer == NULL) {
        return;
    }
    buffer_free(editor->current_buffer);
    free(editor->current_buffer);
}

void editor_load_file(Editor *editor, char *filename) {
    assert(editor != NULL && filename != NULL);
    editor->current_buffer = allocate(sizeof(*(editor->current_buffer)));
    FILE *file;
    if ((file = fopen(filename, "r")) == NULL) {
        // TODO: do not crash
        error_std("fopen");
    }
    buffer_from_file(editor->current_buffer, file);
    if (fclose(file) == EOF) {
        // TODO: do not crash
        error_std("fclose");
    }
}

void editor_move(Editor *editor, Direction direction) {
    Position *pos = &editor->position;
    size_t real_line_length = buffer_line_length(editor->current_buffer, pos->line);
    // TODO: might go negative
    size_t visible_line_length = real_line_length - 1;
    switch (direction) {
    case DIR_RIGHT:
        if (pos->column < visible_line_length) {
            pos->column++;
        }
        break;
    case DIR_LEFT:
        if (pos->column > 1) {
           pos->column--;
        }
        break;
    case DIR_DOWN:
        if (pos->line < editor->current_buffer->number_of_lines) {
            pos->line++;
            cap_column(editor);
        }
        break;
    case DIR_UP:
        if (pos->line > 1) {
            pos->line--;
            cap_column(editor);
        }
        break;
    default:
        break;
    }
}

// Adjust current position's column to be within line_len.
static void cap_column(Editor *editor) {
    size_t line_len = buffer_line_length(editor->current_buffer, editor->position.line);
    editor->position.column = MIN(editor->position.column, line_len);
}
