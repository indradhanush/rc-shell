#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>


struct input{
    char *command;
    char **argv;
};

struct input make_input(char *);

void * expand_argv(char **, int, int);

int main() {
    char *command;
    pid_t child_pid;
    pid_t wait_result;
    int stat_loc;
    while (1) {
        command = readline("rcsh> ");
        struct input input = make_input(command);

        child_pid = fork();
        
        // The child process
        if (child_pid == 0) {
            execvp(input.command, input.argv);
            exit(1);
        } else {
            wait_result = waitpid(child_pid, &stat_loc, WUNTRACED);
        }
    }
    return 0;
}

struct input make_input(char *line) {
    int word_count = 0;
    int block = 1;
    static char *separator = " ";
    struct input input;

    char *parsed = strtok(line, separator);

    if (parsed == NULL) {
        // TODO: Return an Error
    }

    input.command = parsed;
    input.argv = NULL;

    while (parsed != NULL) {
        if (word_count % block == 0) {
            input.argv = expand_argv(input.argv, word_count, block);
        }

        input.argv[word_count] = parsed;
        parsed = strtok(NULL, separator);
        word_count++;
    }
    
    input.argv[word_count] = (char *)0;
    return input;
}

void * expand_argv(char **ptr, int count, int block) {
    int memory_chunk = sizeof(char *) * block;

    if (!ptr) {
        ptr = malloc(memory_chunk);
        return ptr;
    }

    memory_chunk = pow(2, count) * memory_chunk;
    ptr = realloc(ptr, memory_chunk);
    return ptr;
}
