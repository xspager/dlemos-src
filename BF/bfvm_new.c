/*
 * BrainFuck Interpreter that runs a BYTECODE
 *
 */

/*
 * TODO: Fix the Verbosity to say how may times
 * a intruction was done in the fast way (see the code in op + in function run())
 *
 */

#include "lib_easy/easy.h"

#define VERBOSE
#define MEM_BLOCK_SIZE 65536		// 64kb

struct bf_machine{
	struct space *bf_program;
	byte *memory;
	byte *stack[255];
	byte stack_n[255];	// don't ask! :))
	byte *stack_p;
	int stack_i;
	byte *bf_program_p;
	byte b;
	byte nible;
	int times;
	byte *mem_pos_p;
};

void inC(byte *c);
void outC(byte *c);
int run(struct bf_machine *machine);
struct bf_machine *alloc_bf_machine();
void next_byte(struct bf_machine *m);
void next_nible(struct bf_machine *m);
void init_bf_machine(struct bf_machine *m);
void free_bf_machine(struct bf_machine *m);

int main(int argc, char *argv[])
{
	struct bf_machine *machine;
	int i,v;
	byte b;

	if(argc == 3){
		i = 2;
		if(argv[1][0] == '-' && argv[1][1] == 'v') v = 1;
	}
	else i = 1;

	machine = alloc_bf_machine();
	machine->bf_program = loadfile(argv[i]);
	init_bf_machine(machine);

	while(machine->b)	// machine->b ou machine->bf_program_p
	{
		if(machine->b & BIT7)	// if machine->b = 1XXXXXXXb, isn't a char,...
		{
			if(machine->nible)
			{
				if(machine->b & BIT3)	// if machine->b = 1XXX1XXXb, second nible means a number of times to repeat the first plus one
					machine->times = numberoftimes(&machine->b);
				else{
					machine->times = 1;
					/* Could be substituted by something like secondToFirstNible */
					machine->b <<= 4;
					machine->b |= 0x80;
					/*****************/
				}
			}
			else machine->times = 1;
			if(machine->times) run(machine); // to avoid call run when machine->times == 0
			next_nible(machine);
		}
		else{	// is a char jump to next byte
			machine->nible = 0; // point to first
			next_byte(machine);
		}
	}

	free_mem(machine->memory);
	free_bf_machine(machine);

	printf("\n");
	return(0);
}

void inC(byte *c)
{	*c = getchar();
}

void outC(byte *c)
{	putchar(*c);
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
    long mem_size = MEM_BLOCK_SIZE;
   
	for(;machine->times > 0; machine->times--){
		switch(machine->b & 0x70){ // 01110000b
			case 0x00: // ,
#ifdef VERBOSE
				printf(",");
#endif
				inC(machine->mem_pos_p);
				break;
			case 0x10: // +
				if(machine->times > 1){
					*machine->mem_pos_p += machine->times;
#ifdef VERBOSE
					printf("%ix+", machine->times);
#endif
					machine->times = 0;
				}
				else{
					(*machine->mem_pos_p)++;
#ifdef VERBOSE
					printf("+");					
#endif
				}
				break;
			case 0x20: // >
#ifdef VERBOSE
				printf(">");
#endif
				if(*machine->mem_pos_p >= mem_size){
					printf("too many >\n");
					exit(1);
				}
				machine->mem_pos_p++;
				break;
			case 0x30: // [
#ifdef VERBOSE
				printf("[");
#endif
				if(*machine->mem_pos_p){
					*machine->stack_p++ = machine->bf_program_p;
					machine->stack_n[machine->stack_i++]= machine->nible;
					/************************/
					/*						*/
					/* Store what nible		*/
					/*						*/
					/************************/
					/*
					 * i = 200
					 *	
					 *
					 * int i = stack_p - &stack[0]; // to know what position of stack stack_p is ponting
					 * stack_n[i/8] &= 
					 *
					 *
					 */
                }
				else{ // jump the [] block content
					do{
						next_nible(machine);
					}while(!(machine->b & 0xF0));	// ]
				}
				break;
			case 0x40: // .
#ifdef VERBOSE
				printf(".");
#endif
				outC(machine->mem_pos_p);
				break;
			case 0x50: // -
				if(machine->times > 1){
					*machine->mem_pos_p -= machine->times;
					machine->times = 0;
				}
				else{
					(*machine->mem_pos_p)--;
#ifdef VERBOSE
					printf("-");
#endif
				}
				break;
			case 0x60: // <
				if(machine->mem_pos_p < machine->memory){
					printf("Stop! Your program move left it's memory pointer out of BrainFuck's memory.\n");
					exit(1);
				}
				else{
					if(machine->times > 1){
						machine->mem_pos_p += machine->times;
#ifdef VERBOSE
						printf("%ix<", machine->times);
#endif
						machine->times = 0;
					}
					else{
						machine->mem_pos_p--;
#ifdef VERBOSE
						printf("<");
#endif
					}
				}
				break;
			case 0x70: // ]
#ifdef VERBOSE
				printf("]");
#endif
				machine->bf_program_p = *machine->stack_p; // to back to the nible in the byte SHEET!
				machine->nible = machine->stack_n[machine->stack_i--];
				break;
		}
	}
}

struct bf_machine *alloc_bf_machine()
{
	struct machine *m;

	if((m = malloc(sizeof(struct bf_machine))) == NULL){
		printf("Fail allocking a machine structure!");
		exit(1);
	}
	else
		return(m);
}

void next_byte(struct bf_machine *m)
{
	m->b = *m->bf_program_p++;
}

void next_nible(struct bf_machine *m)
{
	if(m->nible){ 		// if in the last nible of byte
		next_byte(m);
		m->nible = 0;			// now in first nible of this byte
	}
	else{
		m->b <<= 4;				// pass the last nible to first
		m->b |= BIT7;
		m->nible = 1;			// now in the last nible
	}
}

void free_bf_machine(struct bf_machine *m)
{
	free((void *) m);
}

void init_bf_machine(struct bf_machine *m)
{
	m->memory = alloc_mem(MEM_BLOCK_SIZE);
	m->mem_pos_p = m->memory;
	//m->stack_p = &m->stack[0];
	m->stack_i = 0;
	m->nible = 0;
	m->times = 1;
	m->bf_program_p = m->bf_program;
	m->b = *m->bf_program_p;
}

