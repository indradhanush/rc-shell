#include <unistd.h>
#include "shell.h"

struct shell *make_shell() {
    static struct shell s;
    s.is_interactive = isatty(STDIN_FILENO);

    return &s;
}
