#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

#include "signal_handling.h"
#include "builtin.h"
#include "input.h"
#include "job_control.h"
#include "helpers.h"
#include "shell.h"


int main() {
    char *command;
    pid_t child_pid;
    int stat_loc;

    struct parent *parent_ptr;

    struct builtin *builtins_ptr;
    struct builtin *builtin_found;

    struct input *inp_ptr = NULL;

    struct shell *shell_ptr = make_shell();

    /* Check if the terminal is in interactive mode */
    if (shell_ptr->is_interactive) {
        parent_ptr = make_parent();
        exit_on_error(setup_parent_signals(), NULL);
        exit_on_error(setup_job_control(parent_ptr), NULL);
    } else {
        perror("Shell is not interactive");
        exit(1);
    }

    /* Setup builtins */
    builtins_ptr = make_builtin();

    while (1) {
        if (sigsetjmp(jmpbuf, 1) == CODE_SIGINT) {
            printf("\n");
            continue;
        }

        command = readline("rcsh> ");
        if (command == NULL) {  /* Exit on Ctrl-D */
            printf("\n");
            exit(0);
        }

        add_history(command);
        inp_ptr = make_input(inp_ptr, command);
        if (inp_ptr == NULL) {
            // TODO: This also masks errors from functions lower down
            // in the invocation chain. Need to fix this.
            continue;
        }

        builtin_found = is_builtin(builtins_ptr, inp_ptr->command);
        if (builtin_found != NULL) { /* If we found an index */
            run_builtin(builtin_found, inp_ptr->command);
            continue;
        }

        child_pid = fork();
        exit_on_error(child_pid, "Error in fork");

        if (child_pid == 0) {   /* child */
            /* Setup signal handling in the child */
            setpgid(0, parent_ptr->pgid);

            exit_on_error(setup_child_signals(), NULL);

            tcsetpgrp(STDIN_FILENO, parent_ptr->fgid);

            exit_on_error(
                execvp(inp_ptr->command[0], inp_ptr->command),
                inp_ptr->command[0]
            );
        } else {                /* parent */
            setpgid(child_pid, parent_ptr->pgid);

            if (!inp_ptr->is_background_command) {
                waitpid(child_pid, &stat_loc, WUNTRACED);
            }
        }

        if (inp_ptr != NULL) {
            free(inp_ptr->argv);
            free(inp_ptr->command);
            free(inp_ptr);
            inp_ptr = NULL;
            free(command);
        }
    }

    return 0;
}
