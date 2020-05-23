all: build clean

build:
	gcc -c datum.c lru.c hashtable.c cache.c command.c logfile.c main.c
	gcc -o kavs datum.o lru.o hashtable.o cache.o command.o logfile.o main.o

clean:
	rm -f *.o
