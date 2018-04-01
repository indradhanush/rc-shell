#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid, parent_pid, child_pgid, parent_pgid;
    char *command;
    int stat_loc;
    int wait_result;

    parent_pid = getpid();
    while (1) {
        command = readline("Press any key to continue\n");
        if (command == NULL)
            exit(0);

        struct sigaction ignore;
        ignore.sa_handler = SIG_IGN;
        sigemptyset(&ignore.sa_mask);
        ignore.sa_flags = SA_RESTART;
        sigaction(SIGINT, &ignore, NULL);
        sigaction(SIGTSTP, &ignore, NULL);
        sigaction(SIGQUIT, &ignore, NULL);
        sigaction(SIGCHLD, &ignore, NULL);
        sigaction(SIGTTIN, &ignore, NULL);
        sigaction(SIGTTOU, &ignore, NULL);

        setpgid(0, parent_pid);
        parent_pgid = getpgid(0);
        tcsetpgrp(STDIN_FILENO, parent_pgid);

        child_pid = fork();
        if (child_pid < 0) {
            exit(1);
        }

        if (child_pid == 0) {

            setpgid(0, child_pid);
            child_pgid = getpgid(0);
            tcsetpgrp(STDIN_FILENO, child_pgid);

            struct sigaction default_s;

            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
            signal(SIGCHLD, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            execlp("ls", "ls", NULL);
        } else {
            setpgid(child_pid, child_pid);
            tcsetpgrp(STDIN_FILENO, child_pgid);

            wait_result = waitpid(child_pid, &stat_loc, WUNTRACED);
            tcsetpgrp(STDIN_FILENO, parent_pgid);

            printf("Wait result: %d\n", wait_result);
        }
    }
}
