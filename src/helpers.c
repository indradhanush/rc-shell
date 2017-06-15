#include <stdio.h>
#include <stdlib.h>


void exit_on_error(int result, char *msg) {
    if (result < 0) {
        if (msg != NULL) {
            perror(msg);
        }

        exit(1);
    }
}
