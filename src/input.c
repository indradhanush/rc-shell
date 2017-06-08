#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

struct input *make_input(struct input *inp_ptr, char *line) {
    int word_count = 0;
    int block = 4;
    static char *separator = " ";

    char *parsed = strtok(line, separator);

    if (parsed == NULL) {
        return NULL;
    }

    while (parsed != NULL) {
        if (word_count % block == 0) {
            inp_ptr = expand_argv(inp_ptr);
            if (inp_ptr == NULL) {
                return NULL;
            }
        }

        inp_ptr->argv[word_count] = parsed;
        parsed = strtok(NULL, separator);
        word_count++;
    }
    
    if (inp_ptr->size_argv == word_count) {
        inp_ptr->argv = realloc(inp_ptr->argv, inp_ptr->size_argv + sizeof(char *));
        if (inp_ptr->argv == NULL) {
            return NULL;
        }
    }
    inp_ptr->argv[word_count] = (char *)0;
    return inp_ptr;
}

struct input *expand_argv(struct input *inp_ptr) {
    int new_size_argv;

    if (inp_ptr == NULL) {
        inp_ptr = (struct input *) malloc(sizeof(struct input ));
        if (inp_ptr == NULL) {
            return NULL;
        }

        inp_ptr->argv = (char **) malloc(MEMORY_CHUNK);
        if (inp_ptr == NULL) {
            return NULL;
        }

        inp_ptr->size_argv = MEMORY_CHUNK;
        return inp_ptr;
    }

    new_size_argv = 2 * inp_ptr->size_argv;

    inp_ptr->argv = realloc(inp_ptr->argv, new_size_argv);
    if (inp_ptr->argv == NULL) {
        return NULL;
    }

    inp_ptr->size_argv = new_size_argv;
    return inp_ptr;
}
