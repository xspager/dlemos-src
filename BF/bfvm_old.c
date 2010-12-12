/*

*/
#include "lib_easy/easy.h"

#define MEM_SIZE 65534

#define MACRO_VARS int MACRO_i = 0;\
		int v = 0;

#define GETB() loaded_file[MACRO_i];MACRO_i++
#define PUTB() loaded_file[MACRO_i];MACRO_i++

byte inC();
void outC(byte c);

int main(int argc, char *argv[])
{
	MACRO_VARS // :.
	int times,f_name_i = 1;
	byte b,nible,*loaded_file,stack_i;
	struct space *f;
	uint stack[256];
	/*	BF VM vars	*/
	byte *mem;
	uint pos = 0;
	/*	end	*/
	
	if(argc == 3){
		f_name_i = 2;
		if(argv[1][0] == '-' && argv[1][1] == 'v')
			v = 1;
	}

	f = loadfile(argv[f_name_i]);
	loaded_file = f->p;

	mem = alloc_mem(MEM_SIZE);

	nible = 0; // starting by nible 0
	times = 1; // run a command, at least once

	b = GETB();
	if(v) printf("b = 0x%x\n", b);
	while(b){
		if(b & BIT7){
			if(v) printf("Isn't a char\n");
			if(nible){
				if(v) printf("Second nible\n");
				if(b & BIT3){ // it is XXXX1XXXb?
					if(v) printf("Is a number of times to repeat\n");
					switch(b & 0x07){ // 00000111b
						case 0: times = 0;  break;
						case 1: times = 2;	break;
						case 2: times = 4;	break;
						case 3: times = 8;	break;
						case 4: times = 16;	break;
						case 5: times = 32;	break;
						case 6: times = 64;	break;
						case 7: times = 128;	break;
					}
					if(times == 0){				
					}
					if(v) printf("Repeat %i\n", times);
				}
				else{
					if(v) printf("Is a command\n");
					times = 1;
					b <<= 4;
					b |= 0x80; // set first bt
				}
			}
			else{
				if(v) printf("First nible\n");
				times = 1;
			}
			for(; times >= 1; times--){
				if(v) printf("Executando a %i_a vez\n", times);
				switch(b & 0x70){ // 01110000b
					case 0x00: // ,
						//if(v) printf("Digite um caracter:\n");
						mem[pos] = inC();
						break;
					case 0x10: // +
						if(times > 1){
							mem[pos] += times;
							if(v) printf("%i x +\n", times);
							times = 0;
						}
						else{
							if(v) printf("+\n");
							mem[pos]++;
						}
						break;
					case 0x20: // >
						if(v) printf(">\n");
						if(pos >= MEM_SIZE){
							printf("Fatal programer stupid!\nStop! traing to acess mem after end of BF machine\n");
							exit(1);
						}
						pos++;
						break;
					case 0x30: // [
						if(v) printf("[\n");
						if(mem[pos])
							stack[stack_i++] = MACRO_i;
						else
							while(b != ']') b = GETB();
						break;
					case 0x40: // .
						outC(mem[pos]);
						if(v) printf(" = 0x%x\n", mem[pos]);
						break;
					case 0x50: // -
						if(times > 1){
							mem[pos] -= times;
							if(v) printf("%i x -\n", times);
							times = 0;
						}
						else{
							if(v) printf("-\n");
							mem[pos]--;
						}
						break;
					case 0x60: // <
						if(v) printf("<\n");
						if(pos <= 0){
							printf("Fatal programer stupid!\nStop! traing to acess mem before star of BF machine\n");
							exit(1);
						}
						pos--;
						break;
					case 0x70: // ]
						if(v) printf("]\n");
						MACRO_i = stack[stack_i--];
						break;
				}
			}
			if(!nible) nible = 1;
			else{
				b = GETB();
				if(v) printf("b = 0x%x\n", b);
				nible = 0;
			}
		}
		else{
			if(v) printf("%c", b); //printf("Is a char: %c\n", b);
			nible = 0;
			b = GETB();
		}
	}
	//printf("%s", mem);
	printf("\n");
	return(0);
}

byte inC(){ return(getc(stdin)); }
void outC(byte c){ putchar(c); }

