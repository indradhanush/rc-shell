#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "builtin.h"


struct builtin *make_builtin() {
    static struct builtin *s;
    s = emalloc(2 * sizeof(struct builtin));
    s[0].command = "cd";
    s[0].function = cd;

    s[1].command = NULL;
    s[1].function = NULL;

    return s;
}

struct builtin *is_builtin(struct builtin *builtins_ptr, char **command) {
    while(builtins_ptr->command != NULL) {
        if (strcmp(builtins_ptr->command, *command) == 0) {
            return builtins_ptr;
        }

        builtins_ptr++;
    }

    return NULL;
}

void run_builtin(struct builtin *builtin_ptr, char **command) {
    if (builtin_ptr->function(command) < 0) {
        perror(builtin_ptr->command);
    }
}

int cd(char **command) {
    return chdir(command[1]);
}
