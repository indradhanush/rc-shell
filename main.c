#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>


#define MEMORY_CHUNK 64


struct input{
    char **argv;
    int size_argv;
};

struct input *make_input(struct input *, char *);

struct input *expand_argv(struct input *);

int main() {
    char *command;
    pid_t child_pid;
    pid_t wait_result;
    int stat_loc;
    int result_execvp;
    struct input *inp_ptr = NULL;

    while (1) {
        command = readline("rcsh> ");
        inp_ptr = make_input(inp_ptr, command);

        if (inp_ptr == NULL) {
            continue;
        }

        child_pid = fork();
        
        // The child process
        if (child_pid == 0) {
            result_execvp = execvp(inp_ptr->argv[0], inp_ptr->argv);
            if (result_execvp == -1) {
                perror(inp_ptr->argv[0]);
                exit(1);
            }
            exit(0);
        } else {
            wait_result = waitpid(child_pid, &stat_loc, WUNTRACED);
        }

        if (inp_ptr != NULL) {
            free(inp_ptr->argv);
            free(inp_ptr);
            inp_ptr = NULL;
        }
    }

    return 0;
}

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
        }

        inp_ptr->argv[word_count] = parsed;
        parsed = strtok(NULL, separator);
        word_count++;
    }
    
    if (inp_ptr->size_argv == word_count) {
        inp_ptr->argv = realloc(inp_ptr->argv, inp_ptr->size_argv + sizeof(char *));
    }
    inp_ptr->argv[word_count] = (char *)0;
    return inp_ptr;
}

struct input *expand_argv(struct input *inp_ptr) {
    int new_size_argv;

    if (inp_ptr == NULL) {
        inp_ptr = (struct input *) malloc(sizeof(struct input ));
        // TODO: handle error from malloc

        inp_ptr->argv = (char **) malloc(MEMORY_CHUNK);
        // TODO: handle error from malloc

        inp_ptr->size_argv = MEMORY_CHUNK;
        return inp_ptr;
    }

    new_size_argv = 2 * inp_ptr->size_argv;

    inp_ptr->argv = realloc(inp_ptr->argv, new_size_argv);
    // TODO: handle error from realloc

    inp_ptr->size_argv = new_size_argv;

    return inp_ptr;
}
