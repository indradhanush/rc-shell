#ifndef builtin_h
#define builtin_h

struct builtin {
    char *command;
    int (*function) (char **);
};

struct builtin *make_builtin();
int is_builtin(struct builtin *, char *);
int run_builtin(struct builtin *, int, char **);
int cd(char **);

#endif // builtin_h
