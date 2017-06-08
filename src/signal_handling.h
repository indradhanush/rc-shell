#include <signal.h>
#include <setjmp.h>


#define CODE_SIGINT 1002

extern sigjmp_buf jmpbuf;

void sigint_handler(int);
