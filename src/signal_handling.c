#include "signal_handling.h"


sigjmp_buf jmpbuf;


void sigint_handler(int signo __attribute__((unused))) {
    siglongjmp(jmpbuf, CODE_SIGINT);
}
