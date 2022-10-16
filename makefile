CC=g++

CFLAGS= -c

all: mp3

prog: mp3.o
	$(CC) mp3.o -o mp3

mp3.o: mp3.cpp
	$(CC) $(CFLAGS) mp3.cpp

clean:
	rm -rf *.o