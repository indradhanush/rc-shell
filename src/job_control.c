#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "signal_handling.h"
#include "job_control.h"


struct parent *make_parent() {
    static struct parent s;
    s.pid = getpid();

    s.pgid = getpgid(0);
    s.fgid = tcgetpgrp(STDIN_FILENO);

    if (s.pgid != s.fgid) {
        kill(- s.pgid, SIGTTIN);
    }

    return &s;
}


int setup_job_control(struct parent *ptr) {
    int result;

    /* Set process group id to the process id */
    if ((result = setpgid(ptr->pid, ptr->pid)) < 0) {
        perror("Error in setpgid");
        return result;
    }

    if ((ptr->pgid = getpgid(0)) < 0) {
        perror("Error in getpgid");
        return ptr->pgid;
    }

    /* Set the foreground group for STDIN */
    if ((result = tcsetpgrp(STDIN_FILENO, ptr->pgid)) < 0) {
        perror("Error in tcsetpgrp");
        return result;
    }

    if ((ptr->fgid = tcgetpgrp(STDIN_FILENO)) < 0) {
        perror("Error in tcgetpgrp");
        return ptr->fgid;
    }

    return 0;
}
