#ifndef input_h
#define input_h

#define MEMORY_CHUNK 64

struct input{
    char **argv;
    int size_argv;
};

struct input *make_input(struct input *, char *);

struct input *expand_argv(struct input *);

#endif // input_h
