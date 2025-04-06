all:
	gcc -lpthread multithreads.c -o multithreads.o

run:
	./multithreads.o -p 20 -c 5
