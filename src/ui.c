#include <stdbool.h>
#include <ncurses.h>

#include "ui.h"
#include "buffer.h"
#include "editor.h"

static void draw_status(Editor *editor);
static void draw_buffer(Buffer *buf);
static void draw_line(size_t line, size_t *rows_left, Buffer *buf);
static void draw_line_number(size_t line, int y);
static void position_cursor(Position pos);

/* TODO: struct this up */
static int rows, cols;
static size_t first_line;
/* TODO: determine this dynamically */
static size_t left_pad = 3;

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
    position_cursor(editor->position);
    refresh();
}

bool ui_input(Editor *editor) {
    int key = getch();
    switch (key) {
    case 27:
        return false;
    case 'h':
        editor_move(editor, DIR_LEFT);
        break;
    case 'l':
        editor_move(editor, DIR_RIGHT);
    default:
        break;
    }
    return true;
}

static void position_cursor(Position pos) {
    move(first_line - pos.line, left_pad + pos.column);
}

static void draw_buffer(Buffer *buf) {
    if (buf == NULL) {
        mvprintw(0, 0, "No buffer");
        return;
    }
    size_t rows_left = rows;
    for (size_t line = first_line; line < buf->number_of_lines; line++) {
        draw_line(line, &rows_left, buf);
        if (rows_left == 0) {
            return;
        }
    }
}

static void draw_line(size_t line, size_t *rows_left, Buffer *buf) {
    draw_line_number(line, rows - *rows_left);
    size_t current = buf->line_indices[line];
    char current_char = '\0';
    int x = left_pad;
    while (current < buf->capacity && current_char != '\n' && *rows_left > 0) {
        current_char = buf->data[current];
        int y = rows - *rows_left;
        mvaddch(y, x, current_char);
        current++;
        x++;
        if (x > cols - 1) {
            x = left_pad;
            (*rows_left)--;
        }
    }
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
    char *mode_string = editor_mode_to_string(editor->current_mode);
    size_t num_lines =
        editor->current_buffer == NULL
        ? 0
        : editor->current_buffer->number_of_lines;
    char status_text[cols];
    snprintf(
        status_text,
        cols,
        "%s %ld lines %ld, %ld",
        mode_string,
        num_lines,
        editor->position.line,
        editor->position.column
    );
    mvprintw(rows - 1, 0, status_text);
    clrtoeol();
    attroff(A_STANDOUT);
}
