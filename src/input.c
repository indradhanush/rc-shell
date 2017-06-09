#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "input.h"


struct input *make_input(struct input *inp_ptr, char *line) {
    inp_ptr = tokenize(inp_ptr, line);
    if (inp_ptr == NULL) {
        return NULL;
    }

    inp_ptr->is_background_command = is_background_command(inp_ptr);

    if (create_command(inp_ptr) == -1) {
        free(inp_ptr->argv);
        free(inp_ptr);
        return NULL;
    }

    return inp_ptr;
}

/*
 * tokenize parses the command input and initializes it into a NULL
 * padded array of strings.
 */
struct input *tokenize(struct input *inp_ptr, char *line) {
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
        inp_ptr->argv = realloc(inp_ptr->argv,
                                inp_ptr->size_argv + sizeof(char *));
        if (inp_ptr->argv == NULL) {
            return NULL;
        }
    }
    inp_ptr->argv[word_count++] = (char *)0;
    inp_ptr->size_argv += sizeof(char *);
    inp_ptr->len_argv = word_count;

    return inp_ptr;
}

struct input *expand_argv(struct input *inp_ptr) {
    int new_size_argv;

    if (inp_ptr == NULL) {
        inp_ptr = emalloc(sizeof(struct input ));
        inp_ptr->argv = emalloc(MEMORY_CHUNK);
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

/*
 * A command is a background command if the last character is an
 * ampersand (&).
 * The last element is NULL, so we want to check for
 * "&" at (size - 2)
 * 0     1     2     3
 * foo  arg1   &   NULL
 */
int is_background_command(struct input *inp_ptr) {
    int index = inp_ptr->len_argv - 2;
    if (strcmp(inp_ptr->argv[index], "&") == 0) {
        return 1;
    }

    return 0;
}

int create_command(struct input *inp_ptr) {
    int bytes;
    int size = inp_ptr->len_argv;
    if (inp_ptr->is_background_command) {
        size -= 1;
    } /* Remove the "&" from a background command */

    bytes = (sizeof(char *) * size);
    inp_ptr->command = emalloc(bytes);

    inp_ptr->command = memcpy(inp_ptr->command, inp_ptr->argv, bytes);
    if (!inp_ptr->command) {
        return -1;
    }

    inp_ptr->command[size-1] = NULL;

    return 0;
}
