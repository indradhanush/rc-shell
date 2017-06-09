EXEC = rcshell

LIBS = -lreadline

SRC = \
	common.c \
	signal_handling.c \
	builtin.c \
	input.c \
	main.c \

build:
	gcc -g $(LIBS) $(SRC) -o $(EXEC)

run: build
	./$(EXEC)

debug: build
	gdb $(EXEC)

valgrind: build
	valgrind -v --leak-check=full ./$(EXEC)

docker:
	docker run --workdir $(PWD) --rm --volume $(PWD):$(PWD) -it gcc bash
