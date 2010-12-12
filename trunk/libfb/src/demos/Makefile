include ../../Makefile.inc

all:	arvre colors fbdemo2 fbed game pattern squares

%:	%.c
	$(CC) -o $* $*.c -lm $(LIBFB) -ggdb -DDELAY
