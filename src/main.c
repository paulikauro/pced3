#include <stdio.h>

#include "buffer.h"
#include "utils.h"
#include "ui.h"
#include "editor.h"

/* Temporary kludge */
int main() {
    Editor editor = {0};
    Buffer buf = {0};
    editor.current_buffer = &buf;
    FILE *file = fopen("testfile", "r");
    if (file == NULL) {
        error_std("fopen");
    }
    buffer_from_file(editor.current_buffer, file);
    fclose(file);

    ui_init();
    do {
        ui_draw(&editor);
    } while (ui_input() != 1);
    ui_free();

    buffer_free(editor.current_buffer);
    return 0;
}
