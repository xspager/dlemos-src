#include <stdio.h>
#include "lib_easy/easy.h"

/*
	I think it just work in a machine with a PC like endianism!
*/

/*
	This program reads a BF program from a file, and write another file with a BYTECODE version of the file. It compact repeted BF comands, and encode it in units of multiples of two from 4 till 128, to represent a sequence of two repeated commands it just encode in 1/1 command/opcode. 

The structure of the output file bytes is:

- first a header BFC
- every next byte:
	0xYxxx xxxx		if Y = 1 it is a opcode byte
	0x1YYY xxxx		YYY is a command
		000		,
		001		+
		010		>
		011		[
		100		.
		101		-
		110		<
		111		]
	0x1xxx Yxxx		if Y = 1 the second nible of the byte means a number of times + 1 that the first nible command should be repeated
					if Y = 0 the second nible of the byte is a other command, and the other bits had the same meas as in the first nible
	0x1xxx 1YYY		
		000		0		1	that is a way to put just one command in a byte
		001		2		3		
		010		4		5
		011		8		9
		100		16		17
		101		32		33
		110		64		65
		111 	128		129

*/

int main(int argc, char *argv[])
{
	byte b,bi,*loaded_file;
	u__int fi_size, fo_i;
	int i;
	struct space *fi,*fo;
	byte bo, op = 0, nible = 0;//,t = 1;
	byte *fo_;

	fi = loadfile(argv[1]);
	loaded_file = fi->p;
	fi_size = fi->size;

	fo_ = alloc_mem(fi_size);
	fo_i = 0;
	
	for(i = 0; i <= fi_size; i++){
		/*bi =*/ b = loaded_file[i] & 0x7F; // 1XXX XXXXb => 0XXX XXXXb
		//printf("%c", b);
		//for(;t >= 1; t--){
			switch(b){
				case ',': op = 0x0; break;
				case '+': op = 0x1; break;
				case '>': op = 0x2; break;
				case '[': op = 0x3; break;
				case '.': op = 0x4; break;
				case '-': op = 0x5; break;
				case '<': op = 0x6; break;
				case ']': op = 0x7; break;
				default:
					if(!nible) bo = b;
					else{
						bo |= 0x08;	// bo |= 00001000b (if kill the meaning of the last nible)
						fo_[fo_i++] = bo;
						putchar(bo);	
						bo = b;
						//continue;
					}
					nible = 1;
					op = 0;
			}/*
			if(op != b){
				while(ci == c && eq <= 255){
					eq++;
					ci = sp->p[i];
				}
			}
		}*/
		if(!nible){
			bo = op << 4; // shift op to first nible
			bo |= 0x80; // set bit 7 of byte meaning that it is a BF bytecode
			nible = 1;
		}
		else{
			//if(op != b){
				bo = bo | op;
			//}
			//else{
			//}
			fo_[fo_i++] = bo;
			putchar(bo);
			//printf("bo[%i] = 0x%x\n", fo_i - 1, fo_[fo_i - 1]);
			nible = 0;
		}
		
	}
	//printf("%s", fo_);
}

