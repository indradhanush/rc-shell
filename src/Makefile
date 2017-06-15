EXEC = rcshell

LIBS = -lreadline

SRC = \
	common.c \
	signal_handling.c \
	builtin.c \
	input.c \
	job_control.c \
	helpers.c \
	main.c \

build:
	gcc -g -Wall -Wpedantic $(LIBS) $(SRC) -o $(EXEC) && chmod +x $(EXEC)

run: build
	./$(EXEC)

debug: build
	gdb $(EXEC)

valgrind: build
	valgrind -v --leak-check=full ./$(EXEC)

docker:
	docker run --workdir $(PWD) --rm --volume $(PWD):$(PWD) -it gcc bash
