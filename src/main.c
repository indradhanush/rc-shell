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


int main() {
    char *command;
    pid_t child_pid;
    pid_t wait_result;
    int stat_loc;
    int result_execvp;
    int builtin_index;

    struct input *inp_ptr = NULL;

    /* Setup signal handling */

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

    /* Setup builtins */
    struct builtin *builtin_ptr = make_builtin();

    while (1) {
        if (sigsetjmp(jmpbuf, 1) == CODE_SIGINT) {
            printf("\n");
            continue;
        }

        /* Handle SIGINT in the parent, so that it doesn't interrupt
           the process
        */
        if (sigaction(SIGINT, &parent_sigint, NULL) < 0) {
            perror("Error in setting action for SIGINT");
            exit(1);
        }

        command = readline("rcsh> ");
        inp_ptr = make_input(inp_ptr, command);

        if (command == NULL) {
            printf("\n");
            exit(0);
        }

        if (inp_ptr == NULL) {
            // TODO: This also masks errors from functions lower down
            // in the invocation chain. Need to fix this.
            continue;
        }

        if ((builtin_index = is_builtin(builtin_ptr, inp_ptr->command[0])) > -1) {
            if (builtin_ptr->functions[builtin_index](inp_ptr->command[1]) < 0) {
                perror(inp_ptr->command[0]);
            }
            continue;
        }

        if ((child_pid = fork()) < 0) {
            perror("Error in fork");
            exit(1);
        }

        
        // The child process
        if (child_pid == 0) {
            /* Set the default action for SIGINT in the child */
            if (sigaction(SIGINT, &default_s, NULL) < 0) {
                perror("Error in setting action for SIGINT");
                exit(1);
            }

            result_execvp = execvp(inp_ptr->command[0], inp_ptr->command);
            if (result_execvp < 0) {
                perror(inp_ptr->command[0]);
                exit(1);
            }
            exit(0);
        } else {
            if (!inp_ptr->is_background_command) {
                wait_result = waitpid(child_pid, &stat_loc, WUNTRACED);
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
