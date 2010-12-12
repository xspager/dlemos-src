#include "../lib/libfb.h"

int main()
{
	int i, w, h;
	int x = 0, y = 0;

	lfb_init();
	w = lfb.width; h = lfb.height;
	w = 1000; h = 600;

	for(i = 15; i >= 0; i--){
		lfb.fillbox(x, y, w, h, i);
		x += 25; y += 20;
		w = lfb.width - (x + x); h = lfb.height - (y + y);
	}
	
	return 0;
}
