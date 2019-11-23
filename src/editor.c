#include <string.h>

#include "editor.h"
#include "utils.h"

/* TODO: possibly move EditorMode here */
char *editor_mode_strings[] = {
    [EM_NORMAL] = "NORMAL",
};

void editor_init(Editor *editor) {
    assert(editor != NULL);
    editor->current_buffer = NULL;
    editor->current_mode = EM_NORMAL;
    Position pos = {0};
    editor->position = pos;
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
        /* TODO: do not crash */
        error_std("fopen");
    }
    buffer_from_file(editor->current_buffer, file);
    if (fclose(file) == EOF) {
        /* TODO: do not crash */
        error_std("fclose");
    }
}

void editor_move(Editor *editor, Direction direction) {
    Position *pos = &editor->position;
    switch (direction) {
    case DIR_RIGHT:
        /* TODO: check */
        pos->column++;
        break;
    case DIR_LEFT:
        if (pos->column > 0) {
           pos->column--;
        }
        break;
    default:
        break;
    }
}
