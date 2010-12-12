/*

*/
#include "lib_easy/easy.h"

//#define VERBOSE
#define MEM_SIZE 65534

struct bf_machine{
	byte *mem;			// memory of this BF machine
	byte *mem_p;		// pointer to BF`s machine actual memory mem_pition;
	byte (*stack)[255];	// stack that store the start mem_pition of the [ of a [] loop to back faste to this point after found a ]
	byte *stack_p;		// pointer // one or two dereference?
	struct space *prog;	// a space struct pointer to a space struct that store the BF program that is been executed by this machine
	byte *b;			// pointe to the byte that is been executed
	byte nible;			// if 0 this BF machine is runing the most significant nible of b, else the less one
	int times;			// how many times actual b first nible should be repeated after be executed once
};

void inC(byte *c);
void outC(byte *c);
int run(struct bf_machine *machine);

int main(int argc, char *argv[])
{
	struct bf_machine *machine,m;
	int i,v;

	if(argc == 3){
		i = 2;
		if(argv[1][0] == '-' && argv[1][1] == 'v') v = 1;
	}
	else i = 1;

	machine = &m;
	machine->prog = loadfile(argv[i]);
	machine->mem = alloc_mem(MEM_SIZE);
	machine->mem_p = machine->mem;
	machine->stack_p = &machine->stack;
	machine->nible = 0; // starting by nible 0
	machine->times = 1; // run a command, at least once
	machine->b = machine->prog->p;

	while(*machine->b){											// while isn`t a null byte
		if(*machine->b & BIT7){									// is like 1XXXXXXXb
			if(machine->nible){									// most significant nible
				if(*machine->b & BIT3){ // it is XXXX1XXXb?
					machine->times = numberoftimes(machine->b);
					if(machine->times == 0){}
				}
				else{
					machine->times = 1;
					*machine->b <<= 4;
					*machine->b |= 0x80; // set first bt
				}
			}
			else
				machine->times = 1;
/*******************************************************/
			run(machine);
/*******************************************************/
			if(!machine->nible) machine->nible = 1;
			else{
				machine->b++;
				machine->nible = 0;
			}
		}
		else{
			machine->nible = 0;
			machine->b++;
		}
	}
	printf("\n");
	return(0);
}

void inC(byte *c)
{
	*c = getchar();
}

void outC(byte *c)
{
	//putchar(*c);
	printf("%c", *c);
}

int numberoftimes(byte *b)
{
	int times;

	switch(*b & 0x07){ // 00000111b
		case 0: times = 0;  break;
		case 1: times = 2;	break;
		case 2: times = 4;	break;
		case 3: times = 8;	break;
		case 4: times = 16;	break;
		case 5: times = 32;	break;
		case 6: times = 64;	break;
		case 7: times = 128;	break;
	}
	return(times+1);
}

int run(struct bf_machine *machine)
{
    long mem_size = MEM_SIZE;
   
	for(;machine->times > 0; machine->times--){
		switch(*machine->b & 0x70){ // 01110000b
			case 0x00: // ,
#ifdef VERBOSE
				printf(",");
#endif
				inC(machine->mem_p);
				break;
			case 0x10: // +
#ifdef VERBOSE
				printf("+");
#endif
				if(machine->times > 1){
					*machine->mem_p += machine->times;
					machine->times = 0;
				}
				else
					*machine->mem_p++;
				break;
			case 0x20: // >
#ifdef VERBOSE
				printf(">");
#endif
				if(*machine->mem_p >= mem_size){
					printf("Fatal programer stupid!\nStop! traing to acess mem after end of BF machine\n");
					exit(1);
				}
				machine->mem_p++;
				break;
			case 0x30: // [
#ifdef VERBOSE
				printf("[");
#endif
				if(*machine->mem_p){
					machine->stack_p++;
					*machine->stack_p = machine->b;
                }
				else
					while(*machine->b != ']') machine->b++;
				break;
			case 0x40: // .
#ifdef VERBOSE
				printf(".");
#endif
				outC(machine->mem_p);
				break;
			case 0x50: // -
#ifdef VERBOSE
				printf("-");
#endif
				if(machine->times > 1){
					*machine->mem_p -= machine->times;
					machine->times = 0;
				}
				else
					*machine->mem_p--;
				break;
			case 0x60: // <
#ifdef VERBOSE
				printf("<");
#endif
				if(machine->mem_p < machine->mem){
					printf("Fatal programer stupid!\nStop! traing to acess mem before star of BF machine\n");
					exit(1);
				}
				machine->mem_p--;
				break;
			case 0x70: // ]
#ifdef VERBOSE
				printf("]");
#endif
				machine->b = *machine->stack_p--;
				break;
		}
	}
}

