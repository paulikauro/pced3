#include "editor.h"

static const char *editor_mode_strings[] = {
    [EM_NORMAL] = "NORMAL",
};

const char *editor_mode_to_string(EditorMode mode) {
    return editor_mode_strings[mode];
}