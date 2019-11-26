#include <stdbool.h>
#include <ncurses.h>

#include "ui.h"
#include "buffer.h"
#include "editor.h"

static void draw_status(Editor *editor);
static void draw_buffer(Buffer *buf, Position *pos);
static void draw_line(size_t line, size_t *rows_left, Buffer *buf, Position *pos);
static void draw_line_number(size_t line, int y);
// start inclusive, end exclusive
static void draw_row(Buffer *buffer, int y, size_t line, size_t start, size_t end, Position *pos);
static void position_cursor();

// TODO: struct this up
static int rows, cols;
static size_t first_line = 1;
// for draw
static size_t real_row, real_col;
// TODO: determine this dynamically
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
    draw_buffer(editor->current_buffer, &(editor->position));
    draw_status(editor);
    position_cursor();
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

static void position_cursor() {
    // TODO
    move(real_row, real_col);
    // move(first_line - pos.line, left_pad + pos.column - 1);
}

static void draw_buffer(Buffer *buf, Position *pos) {
    if (buf == NULL) {
        mvprintw(0, 0, "No buffer");
        return;
    }
    size_t rows_left = rows;
    for (size_t line = first_line; line <= buf->number_of_lines; line++) {
        draw_line(line, &rows_left, buf, pos);
        if (rows_left == 0) {
            return;
        }
    }
}

static void draw_line(size_t line, size_t *rows_left, Buffer *buffer, Position *pos) {
    draw_line_number(line, rows - *rows_left);
    size_t available_width = cols - left_pad;
    size_t line_length = buffer_line_length(buffer, line);
    size_t start_column = 1;
    // TODO: get rid of this
    size_t cols_left = line_length;

    while (*rows_left > 0 && start_column < line_length) {
        size_t row_length = cols_left > available_width ? available_width : cols_left;
        int y = rows - *rows_left;
        draw_row(buffer, y, line, start_column, start_column + row_length, pos);
        cols_left -= row_length;
        start_column += row_length;
        (*rows_left)--;
    }
}

static void draw_row(Buffer *buffer, int y, size_t line, size_t start, size_t end, Position *pos) {
    int x = left_pad;
    size_t column = start;
    for (; column < end; x++, column++) {
        uint32_t c = buffer_get(buffer, line, column);
        mvaddch(y, x, c);
        // TODO: this is inefficient
        if (column == pos->column && line == pos->line) {
            real_row = y;
            real_col = x;
        }
    }
}

static void draw_line_number(size_t line, int y) {
    char line_number[3];
    snprintf(line_number, 3, "%ld", line);
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
