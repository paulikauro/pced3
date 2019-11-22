#include <stdbool.h>
#include <ncurses.h>

#include "ui.h"
#include "buffer.h"
#include "editor.h"

static void draw_status(Editor *editor);
static void draw_buffer(Buffer *buf);
static void draw_line(size_t line, size_t *rows_left, Buffer *buf);
static void draw_line_number(size_t line, int y);

static int rows, cols;

void ui_init() {
    initscr();
    raw();
    keypad(stdscr, true);
    noecho();
    getmaxyx(stdscr, rows, cols);
}

void ui_free() {
    endwin();
}

void ui_draw(Editor *editor) {
    /* TODO: draw stuff */
    draw_buffer(editor->current_buffer);
    draw_status(editor);
    refresh();
}

int ui_input() {
    int key = getch();
    switch (key) {
    case 27:
        return 1;
    default:
        break;
    }
    return 0;
}

static void draw_buffer(Buffer *buf) {
    size_t rows_left = rows - 1;
    for (size_t line = 0; line < buf->number_of_lines; line++) {
        draw_line(line, &rows_left, buf);
        if (rows_left == 0) {
            return;
        }
    }
}

static void draw_line(size_t line, size_t *rows_left, Buffer *buf) {
    draw_line_number(line, rows - *rows_left);
    size_t current = buf->line_indices[line];
    char current_char;
    int left_pad = 3;
    int x = left_pad;
    do {
        current_char = buf->data[current];
        int y = rows - *rows_left;
        mvaddch(y, x, current_char);
        current++;
        x++;
        if (x > cols) {
            x = left_pad;
            (*rows_left)--;
        }
    } while (current < buf->capacity && current_char != '\n' && *rows_left > 0);
    (*rows_left)--;
}

static void draw_line_number(size_t line, int y) {
    char line_number[3];
    snprintf(line_number, 3, "%ld", line + 1);
    mvprintw(y, 0, line_number);
}

static void draw_status(Editor *editor) {
    clrtobot();
    attron(A_STANDOUT);
    const char *mode_string = editor_mode_to_string(editor->current_mode);
    size_t num_lines = editor->current_buffer->number_of_lines;
    char status_text[cols];
    snprintf(status_text, cols, "%s %ld lines", mode_string, num_lines);
    mvprintw(rows - 1, 0, status_text);
    clrtoeol();
    attroff(A_STANDOUT);
}
