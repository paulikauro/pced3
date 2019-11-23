#pragma once

#include <stdbool.h>

#include "editor.h"

void ui_init();
void ui_free();
void ui_draw(Editor *editor);
bool ui_input(Editor *editor);
