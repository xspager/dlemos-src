/* BrainFuck machine whith jumps, and repetitions assigned by a number */

#include <stdio.h>
#include <sys/stat.h>
//#include <time.h>

#define STACK_LEVEL  32
/* 32768b = 32kb */
#define MEM_SIZE     32768

typedef unsigned char byte;
typedef unsigned int uint;

byte *loadfile(char *name);
byte *alloc_mem(uint size);

int main(int argc, char *argv[])
{
	/* program vars */
	byte *m; /* Brain'sFuck Memory */
	uint p=0; /* Brain'sFuck Memory pointer */
	uint i=0, c, times = 1;
	uint stack[STACK_LEVEL];
	uint stack_l=0;
	//clock_t start, end;
	int ext = 0;

	/* BF vars */
	byte *mem;
	uint pos=0;

	if(argc == 3){
		if(argv[1][0] == '-'){
			if(argv[1][1] == 'e') ext = 1;
			else{
				printf("invalid argument %s\n", argv[1]);
				exit(1);
			}
		}
		else{
			printf("try: %s -e file\n", argv[0]);
			exit(1);
		}
	}
	if(argc <= 3){
		if(ext == 1) m = loadfile(argv[2]);
		else m = loadfile(argv[1]);
	}
	else{
		printf("try: %s [-e] file\n", argv[0]);
		exit(1);
	}

	mem = alloc_mem(MEM_SIZE);

	//start = clock();

	while(m[p]){
/*		if(ext == 1)
			if(m[p] >= '0' && m[p] <= '9'){
				if(!(m[p+1] >= '0' && m[p+1] <= '9')){
					times = m[p] - '0';
					p++;
				}
			}
		else times = 1;
		for(c=0; c < times; c++){
*/			switch(m[p]){
				case '+': mem[pos]++; break;
				case '-': mem[pos]--; break;
				case '<': pos--; break;
				case '>': pos++; break;
				case '[':
					i=0;
					if(!mem[pos])
						while(m[++p] != ']' || i)
							switch(m[p]){
								case '[': i++; break;
								case ']': i--; break;
							}
					else{
						stack_l++;
						stack[stack_l]=p;
					}
					break;
				case ']':
					if(stack_l > 0){
						p = stack[stack_l]-1;
						stack_l--;
					}
					else{
						printf("Erro\n");
						exit(1);
					}
					break;
				case '.': putchar(mem[pos]); break;
				case ',': mem[pos] = getchar();	break;
			}
//		}
		p++;
//		times = 1;
	}
	//end = clock();
	//printf("\nThe program takes %2.2f seconds\n", ((float)end - start) / CLK_TCK);
	return(0);
}

byte *loadfile(char *name)
{
	FILE *file;
	struct stat stats;
	char *loaded_file;

	if((file = fopen(name, "rb")) == NULL){
		printf("can't open: %s", name);
		exit(1);
	}
	fstat(fileno(file), &stats);
	loaded_file = alloc_mem(stats.st_size+4);
	if(fread(loaded_file, sizeof(byte) , sizeof(byte) * (int) stats.st_size, file) != stats.st_size){
		printf("erro loading file\n");
		exit(1);
	}
//	loaded_file[stats.st_size] = 0x0; // too mutch care
	return(loaded_file);
}

byte *alloc_mem(uint size)
{
	char *ptr;

	ptr = (void*) calloc(size, sizeof(byte));
	if(!ptr){
		printf("Fail Allocking %ibytes\n", size);
		exit(1);
	}
	return(ptr);
}

