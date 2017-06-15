#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "signal_handling.h"


sigjmp_buf jmpbuf;


void sigint_handler(int signo __attribute__((unused))) {
    siglongjmp(jmpbuf, CODE_SIGINT);
}

static int setup_sigint() {
    struct sigaction s;
    s.sa_handler = sigint_handler;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;
    return sigaction(SIGINT, &s, NULL);
}

static int setup_sigtstp() {
    struct sigaction s;
    s.sa_handler = SIG_IGN;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;
    return sigaction(SIGTSTP, &s, NULL);
}

static int setup_sigttin() {
    struct sigaction s;
    s.sa_handler = SIG_IGN;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;
    return sigaction(SIGTTIN, &s, NULL);
}

static int setup_sigttou() {
    struct sigaction s;
    s.sa_handler = SIG_IGN;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;
    return sigaction(SIGTTOU, &s, NULL);
}

static int setup_sigchld() {
    struct sigaction s;
    s.sa_handler = SIG_IGN;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;
    return sigaction(SIGTTOU, &s, NULL);
}

static int setup_sigquit() {
    struct sigaction s;
    s.sa_handler = SIG_IGN;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;
    return sigaction(SIGQUIT, &s, NULL);
}

int setup_parent_signals() {
    int result;

    if ((result = setup_sigint()) < 0) {
        perror("Error in SIGINT");
        return result;
    }

    if ((result = setup_sigtstp()) < 0) {
        perror("Error in SIGTSTP");
        return result;
    }

    if ((result = setup_sigttin()) < 0) {
        perror("Error in SIGTTIN");
        return result;
    }

    if ((result = setup_sigttou()) < 0) {
        perror("Error in SIGTTOU");
        return result;
    }

    if ((result = setup_sigchld()) < 0) {
        perror("Error in SIGCHLD");
        return result;
    }

    if ((result = setup_sigquit()) < 0) {
        perror("Error in SIGQUIT");
        return result;
    }

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
