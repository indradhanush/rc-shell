#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>


int main() {
    int r = rand();
    printf("Program started: %d\n", r);
    sleep(1000);
    printf("Program finished: %d\n", r);
}
