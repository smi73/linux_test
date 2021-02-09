CC      = gcc
CFLAGS  = -g
RM      = rm -f


default: all

all: ifspeed

ifspeed: ifspeed.c
    $(CC) $(CFLAGS) -o ifspeed ifspeed.c

clean veryclean:
    $(RM) ifspeed
    