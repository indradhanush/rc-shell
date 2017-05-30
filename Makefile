build:
	gcc -lreadline main.c -o rcshell

run: build
	./rcshell

llvm: main.c
	clang main.c -emit-llvm -S -o hello.ll

docker:
	docker run --workdir $(PWD) --rm --volume $(PWD):$(PWD) -it gcc bash
