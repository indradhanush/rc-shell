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

struct input make_input(char *);

void expand_argv(struct input *);

int main() {
    char *command;
    pid_t child_pid;
    pid_t wait_result;
    int stat_loc;
    int result_execvp;
    while (1) {
        command = readline("rcsh> ");
        struct input inp = make_input(command);
        if (inp.argv == NULL) {
            continue;
        }

        child_pid = fork();
        
        // The child process
        if (child_pid == 0) {
            result_execvp = execvp(inp.argv[0], inp.argv);
            if (result_execvp == -1) {
                printf("%s - An error occurred.\n", inp.argv[0]);
                exit(1);
            }
            exit(0);
        } else {
            wait_result = waitpid(child_pid, &stat_loc, WUNTRACED);
        }
    }
    return 0;
}

struct input make_input(char *line) {
    int word_count = 0;
    int block = 4;
    static char *separator = " ";
    struct input inp = {NULL, 0};

    char *parsed = strtok(line, separator);

    if (parsed == NULL) {
        return inp;
    }

    while (parsed != NULL) {
        if (word_count % block == 0) {
            expand_argv(&inp);
        }

        inp.argv[word_count] = parsed;
        parsed = strtok(NULL, separator);
        word_count++;
    }
    
    if (inp.size_argv == word_count) {
        realloc(inp.argv, sizeof(char *));
    }
    inp.argv[word_count] = (char *)0;
    return inp;
}

void expand_argv(struct input *inp_ptr) {
    int new_size_argv;

    if (!inp_ptr->argv) {
        inp_ptr->argv = malloc(MEMORY_CHUNK);
        // TODO: handle error from malloc

        inp_ptr->size_argv = MEMORY_CHUNK;
        return;
    }

    new_size_argv = 2 * inp_ptr->size_argv;

    inp_ptr->argv = realloc(inp_ptr->argv, new_size_argv);
    // TODO: handle error from realloc

    inp_ptr->size_argv = new_size_argv;
}
