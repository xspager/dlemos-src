#include "../lib/libfb.h"

void main()
{
	lfb.init();
	int width = lfb.width;
	int height = lfb.height;
	int color, row, col;

	for(color = 0; color <= 15; color++){
		for(row = 0; row < height ; row++)
			for(col = 0; col < (width - color * 50); col++)
				*(src + col + row * width) = color;
		//usleep(60000);
		sleep(1);
	}
}
