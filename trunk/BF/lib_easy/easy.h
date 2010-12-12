#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

#define BIT0 0x01 // 00000001b
#define BIT1 0x02 // 00000010b
#define BIT2 0x04 // 00000100b
#define BIT3 0x08 // 00001000b
#define BIT4 0x10 // 00010000b
#define BIT5 0x20 // 00100000b
#define BIT6 0x40 // 01000000b
#define BIT7 0x80 // 10000000b

typedef unsigned char byte;
typedef unsigned int u__int;

struct space{
	byte *p;
	u__int size;
};

extern struct space *loadfile(char *name);
extern void unloadfile(struct space *sp);
extern byte *alloc_mem(u__int size);
extern void free_mem(byte *bp);

