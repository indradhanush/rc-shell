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

int is_builtin(struct builtin *builtins_ptr, char *command) {
    unsigned int i = 0;

    while(builtins_ptr->command != NULL) {
        if (strcmp(builtins_ptr->command, command) == 0) {
            return i;
        }
        builtins_ptr++;
        i++;
    }
 
    return -1;
}

int run_builtin(struct builtin *builtins_ptr, int index, char **command) {
    return builtins_ptr[index].function(command);
}

int cd(char **command) {
    return chdir(command[1]);
}
