#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "builtin.h"


struct builtin *make_builtin() {
    static struct builtin s;
    s.commands = emalloc(2 * sizeof(char *));
    s.commands[0] = "cd";
    s.commands[1] = NULL;

    s.functions[0] = cd;

    return &s;
}

int is_builtin(struct builtin *builtin_ptr, char *command) {
    unsigned int i = 0;

    for(i=0; builtin_ptr->commands[i] != NULL; i++) {
        if (strcmp(builtin_ptr->commands[i], command) == 0) {
            return i;
        }
    }
 
    return -1;
}

int cd(char *path) {
    return chdir(path);
}
