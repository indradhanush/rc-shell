EXEC = rcshell

LIBS = -lreadline

SRC = \
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

docker:
	docker run --workdir $(PWD) --rm --volume $(PWD):$(PWD) -it gcc bash
