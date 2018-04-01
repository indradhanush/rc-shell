#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <setjmp.h>

char **get_input(char *);
int cd(char *);
int sigint_handler(int);

jmp_buf env;

int main() {
    char **command;
    char *input;
    pid_t child_pid;
    int stat_loc;

    signal(SIGINT, SIG_IGN);

    while (1) {
        if (setjmp(env) == 1) {
            continue;
        }

        input = readline("unixsh> ");
        command = get_input(input);


        if (strcmp(command[0], "cd") == 0) {
            if (cd(command[1]) < 0) {
                perror(command[1]);
            }

            /* Skip the fork */
            continue;
        }

        child_pid = fork();
        if (child_pid < 0) {
            perror("Fork failed");
            exit(1);
        }

        if (child_pid == 0) {
            signal(SIGINT, SIG_DFL);

            /* Never returns if the call is successful */
            if (execvp(command[0], command) < 0) {
                perror(command[0]);
                exit(1);
            }
        } else {
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }

        if (!input)
            free(input);
        if (!command)
            free(command);
    }

    return 0;
}

char **get_input(char *input) {
    char **command = malloc(8 * sizeof(char *));
    if (command == NULL) {
        perror("malloc failed");
        exit(1);
    }

    char *separator = " ";
    char *parsed;
    int index = 0;

    parsed = strtok(input, separator);
    while (parsed != NULL) {
        command[index] = parsed;
        index++;

        parsed = strtok(NULL, separator);
    }

    command[index] = NULL;
    return command;
}

int cd(char *path) {
    return chdir(path);
}

int sigint_handler(int signo) {
    printf("SIGINT\n");
    longjmp(env, 1);
}
