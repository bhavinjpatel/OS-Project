CC=gcc
FLAGS=-c
all: shell
program:shell.o
	$(CC) shell.o -o shell
shell.o:shell.c
	$(CC) $(FLAGS) shell.c
run:
	./shell
clean:
	rm -fr *shell