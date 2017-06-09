#ifndef input_h
#define input_h

#define MEMORY_CHUNK 64

struct input{
    char **argv;
    int size_argv;
    int len_argv;
    int is_background_command;
    char **command;
};

struct input *make_input(struct input *, char *);
struct input *tokenize(struct input *, char *);
struct input *expand_argv(struct input *);
int is_background_command(struct input *);
int create_command(struct input *);

#endif // input_h
