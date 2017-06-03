build:
	gcc -g -lreadline main.c -o rcshell

run: build
	./rcshell

llvm: main.c
	clang main.c -emit-llvm -S -o hello.ll

debug: build
	gdb rcshell

docker:
	docker run --workdir $(PWD) --rm --volume $(PWD):$(PWD) -it gcc bash
