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
    struct sigaction s;
    s.sa_handler = SIG_DFL;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;

    exit_on_error(sigaction(SIGINT, &s, NULL), "Error in setting action for SIGINT");
    exit_on_error(sigaction(SIGTSTP, &s, NULL), "Error in setting action for SIGTSTP");
    exit_on_error(sigaction(SIGTTIN, &s, NULL), "Error in setting action for SIGTTIN");
    /* exit_on_error(sigaction(SIGTTOU, &s, NULL), "Error in setting action for SIGTTOU"); */
    exit_on_error(sigaction(SIGCHLD, &s, NULL), "Error in setting action for SIGCHLD");
    exit_on_error(sigaction(SIGQUIT, &s, NULL), "Error in setting action for SIGQUIT");

    return 0;
}
