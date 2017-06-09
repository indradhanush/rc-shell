#ifndef signal_handling_h
#define signal_handling_h

#include <setjmp.h>


#define CODE_SIGINT 1002

extern sigjmp_buf jmpbuf;

void sigint_handler(int);

#endif // signal_handling_h
