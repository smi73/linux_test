CC=gcc
CFLAGS=-I.

ifspeed: ifspeed.o
	$(CC) -o ifspeed ifspeed.o


