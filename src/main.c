#include <stdio.h>

#include "buffer.h"
#include "utils.h"
#include "ui.h"
#include "editor.h"

char *parse_command_line(int argc, char *argv[]);

/* Temporary kludge */
int main(int argc, char *argv[]) {
    char *filename = parse_command_line(argc, argv);
    Editor editor = {0};
    Buffer buf = {0};
    editor.current_buffer = &buf;
    FILE *file = fopen(filename, "r");
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

char *parse_command_line(int argc, char *argv[]) {
    /* TODO */
    if (argc < 2) {
        printf("usage: %s filename\n", argv[0]);
        exit(1);
    }
    return argv[1];
}
