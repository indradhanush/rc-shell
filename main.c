#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>


#define MEMORY_CHUNK 64

/* We use 1000 as the base and add the signal no. to it to represent
   our signal code.
 */
#define CODE_SIGINT 1002


static sigjmp_buf jmpbuf;


struct input{
    char **argv;
    int size_argv;
};

struct input *make_input(struct input *, char *);

struct input *expand_argv(struct input *);

void sigint_handler(int);


int main() {
    char *command;
    pid_t child_pid;
    pid_t wait_result;
    int stat_loc;
    int result_execvp;

    struct input *inp_ptr = NULL;

    struct sigaction ignore;
    struct sigaction default_s;
    struct sigaction parent_sigint;

    ignore.sa_handler = SIG_IGN;
    sigemptyset(&ignore.sa_mask);
    ignore.sa_flags = SA_RESTART;

    default_s.sa_handler = SIG_DFL;
    sigemptyset(&default_s.sa_mask);
    default_s.sa_flags = SA_RESTART;

    parent_sigint.sa_handler = sigint_handler;
    sigemptyset(&parent_sigint.sa_mask);
    parent_sigint.sa_flags = SA_RESTART;

    while (1) {
        if (sigsetjmp(jmpbuf, 1) == CODE_SIGINT) {
            printf("\n");
            continue;
        }

        /* Handle SIGINT in the parent, so that it doesn't interrupt
           the process
        */
        if (sigaction(SIGINT, &parent_sigint, NULL) == -1) {
            perror("Error in setting action for SIGINT");
            exit(1);
        }

        command = readline("rcsh> ");
        inp_ptr = make_input(inp_ptr, command);

        if (inp_ptr == NULL) {
            // TODO: This also masks errors from functions lower down
            // in the invocation chain. Need to fix this.
            continue;
        }

        child_pid = fork();
        
        // The child process
        if (child_pid == 0) {
            /* Set the default action for SIGINT in the child */
            if (sigaction(SIGINT, &default_s, NULL) == -1) {
                perror("Error in setting action for SGIINT");
                exit(1);
            }

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


void sigint_handler(int signo __attribute__((unused))) {
    siglongjmp(jmpbuf, CODE_SIGINT);
}
