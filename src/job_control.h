#ifndef job_control_h
#define job_control_h

#include <unistd.h>

struct process {
    pid_t pid;
    pid_t pgid;
    pid_t fgid;
};

struct process *make_process();
int setup_job_control(struct process *);

#endif  /* job_control */
