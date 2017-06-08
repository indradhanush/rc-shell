#ifndef builtin_h
#define builtin_h

struct builtin {
    char **commands;
    int (*functions[]) (char *);
};

struct builtin *make_builtin();
int is_builtin(struct builtin *, char *);
int cd(char *);

#endif // builtin_h
