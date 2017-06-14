#ifndef job_control_h
#define job_control_h

#include <unistd.h>

struct parent {
    pid_t pid;
    pid_t pgid;
    pid_t fgid;
};

struct parent *make_parent();
int setup_terminal(struct parent *);

#endif  /* job_control */
