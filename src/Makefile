all: build clean

build:
	gcc -c hashtable.c cache.c command.c logfile.c main.c
	gcc -o kavs hashtable.o cache.o command.o logfile.o main.o

clean:
	rm -f *.o
