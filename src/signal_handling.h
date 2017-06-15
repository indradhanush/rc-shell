#ifndef signal_handling_h
#define signal_handling_h

#include <setjmp.h>


#define CODE_SIGINT 1002

extern sigjmp_buf jmpbuf;

void sigint_handler(int);

int setup_parent_signals();
int setup_child_signals();

#endif // signal_handling_h
