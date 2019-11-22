#include <stdio.h>

#include "buffer.h"
#include "utils.h"
#include "ui.h"
#include "editor.h"

char *parse_command_line(int argc, char *argv[]);

/* Temporary kludge */
int main(int argc, char *argv[]) {
    char *filename = parse_command_line(argc, argv);
    Editor editor;
    editor_init(&editor);
    editor_load_file(&editor, filename);

    ui_init();
    do {
        ui_draw(&editor);
    } while (ui_input() != 1);
    ui_free();

    editor_free(&editor);
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
