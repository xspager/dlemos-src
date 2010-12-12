/*
 * Ease lib
 * by x_sp4g3r@yahoo.com.br
*/

#include "easy.h"

struct space *loadfile(char *name)
{
	FILE *file;
	struct stat stats;
	char *loaded_file;
	struct space sp;		// chage it to a pointer

	if((file = fopen(name, "rb")) == NULL){
		printf("can't open: %s\n", name);
		exit(1);
	}
	fstat(fileno(file), &stats);
	loaded_file = alloc_mem(stats.st_size+4);
	if(fread(loaded_file, sizeof(byte) , sizeof(byte) * (int) stats.st_size, file) != stats.st_size){
		printf("erro loading file\n");
		exit(1);
	}
	sp.p = loaded_file;
	sp.size = stats.st_size;
	return(&sp);
}

void unloadfile(struct space *sp)
{
	free((void *) sp->p);
}

byte *alloc_mem(u__int size)
{
	char *ptr;

	ptr = (void*) calloc(size, sizeof(byte));
	if(!ptr){
		printf("Fail Allocking %ibytes\n", size);
		exit(1);
	}
	return(ptr);
}

void free_mem(byte *bp)
{
	free((void *) bp);
}

