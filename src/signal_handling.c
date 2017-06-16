#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "signal_handling.h"
#include "helpers.h"


sigjmp_buf jmpbuf;


void sigint_handler(int signo __attribute__((unused))) {
    siglongjmp(jmpbuf, CODE_SIGINT);
}


static int setup_signal(int signal, void (*handler)(int)) {
    struct sigaction s;
    s.sa_handler = handler;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;
    return sigaction(signal, &s, NULL);
}

int setup_parent_signals() {
    exit_on_error(setup_signal(SIGINT, sigint_handler), NULL);
    exit_on_error(setup_signal(SIGTSTP, SIG_IGN), NULL);
    exit_on_error(setup_signal(SIGTTIN, SIG_IGN), NULL);
    exit_on_error(setup_signal(SIGTTOU, SIG_IGN), NULL);
    exit_on_error(setup_signal(SIGCHLD, SIG_IGN), NULL);
    exit_on_error(setup_signal(SIGQUIT, SIG_IGN), NULL);

    return 0;
}

int setup_child_signals() {
    int result;
    struct sigaction s;
    s.sa_handler = SIG_DFL;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;

    if ((result = sigaction(SIGINT, &s, NULL)) < 0) {
        perror("Error in setting action for SIGINT");
        return result;
    }

    if ((result = sigaction(SIGTSTP, &s, NULL)) < 0) {
        perror("Error in setting action for SIGTSTP");
        return result;
    }

    return 0;
}
