CC=gcc
#CC=agcc
#CC=cc
#CC=i586-mingw32msvc-cc
#CFLAGS:=-w -O3 -pedantic
CFLAGS=-w -ggdb
#CFLAGS:=-w -S

all:	bfi bfvm brainchange

lib_easy/easy.o:	lib_easy/easy.c
	$(CC) $(CFLAGS) -c -o lib_easy/easy.o lib_easy/easy.c

bfi:	bfi.c
	$(CC) $(CFLAGS) -o bfi bfi.c

bfi_old:	bfi_old.c
	$(CC) $(CFLAGS) -o bfi_old bfi_old.c

bfi_fast:	bfi_fast.c bfi
	$(CC) $(CFLAGS) -o bfi_fast bfi_fast.c

bfi_pointer:	bfi_pointer.c
	$(CC) $(CFLAGS) -o bfi_pointer bfi_pointer.c

bfvm:	bfvm.c lib_easy/easy.o
	$(CC) $(CFLAGS) -o bfvm ./lib_easy/easy.o bfvm.c

brainchange:	brainchange.c lib_easy/easy.o
	$(CC) $(CFLAGS) -o brainchange lib_easy/easy.o brainchange.c

clean:
	rm -f bfi bfvm brainchange lib_easy/easy.o 
