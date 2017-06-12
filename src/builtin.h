#ifndef builtin_h
#define builtin_h

struct builtin {
    char *command;
    int (*function) (char **);
};

struct builtin *make_builtin();
struct builtin *is_builtin(struct builtin *, char **);
void run_builtin(struct builtin *, char **);
int cd(char **);

#endif // builtin_h
