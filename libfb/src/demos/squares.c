#include "../lib/libfb.h" 

#define	GREEN		10
#define YELLOW		14
#define LIGHT_WHITE	15

void main()
{
	int i, x, p;
	int x1, y1, x2, y2;

	lfb_init();
	
	lfb.fillscr(0);
	
	srand(time(NULL));
	
	for(i=0; i<1000; i++){
		x = (double) rand() * (15 + 1) / RAND_MAX;
		p = (double) rand() * (100 + 1) / RAND_MAX;
		x1 = x % 15 * (800 / 15);
		x2 = x % 15 * (800 / 15);
		y1 = x * (600 / 15);
		y2 = x * (600 / 15);
		lfb.fillbox(x1,y1,x2,y2,x);
		printf("x = %i, x1 = %i, y1 = %i, x2 = %i, y2 = %i\n", x,x1,y1,x2,y2);
		usleep(60000);
	}
}

