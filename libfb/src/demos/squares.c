#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#define	GREEN		10
#define YELLOW		14
#define LIGHT_WHITE	15

unsigned char map[80][60];
int fb;
int width, height;
unsigned char *scr;

typedef struct{
	int x;
	int y;
} point;

void init(void);
void fill_scr(unsigned char);
void fill_box(int,int,int,int, unsigned char);
void exit_error(char *);

main()
{
	int i, x, p;
	int x1, y1, x2, y2;

	init();
	
	fill_scr(0);
	
	srand(time(NULL));
	
	for(i=0; i<1000; i++){
		x = (double) rand() * (15 + 1) / RAND_MAX;
		p = (double) rand() * (100 + 1) / RAND_MAX;
		x1 = x % 15 (800 / 15);
		x2 = x % 15 (800 / 15);
		y1 = x * (600 / 15);
		y2 = x * (600 / 15);
		//fill_box(x1,y1,x2,y2,x);
		printf("x = %i, x1 = %i, y1 = %i, x2 = %i, y2 = %i\n", x,x1,y1,x2,y2);
		usleep(60000);
	}

	close(fb);
}

void init()
{
	struct fb_var_screeninfo fbinfo;

	fb = open("/dev/fb0", O_RDWR);
	if(!fb) exit_error("Can`t open /dev/fb0\n");
	
	ioctl(fb, FBIOGET_VSCREENINFO, &fbinfo);
	
	width = fbinfo.xres;
	height = fbinfo.yres;
	
	if(fbinfo.bits_per_pixel != 8)
		exit_error("Only works at 8bpp\n");

	scr = (unsigned char *) mmap(0, width * height * 3, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
}

void fill_scr(unsigned char color)
{
	memset(scr, color, 800*600);	
}

void fill_box(int x, int y, int w, int h, unsigned char color)
{
	int cy,cx;

	for(cy = y; cy < y+h; cy++)
		memset(scr + (width * cy + x), color, w);
}

void draw_line(int x1, int y1, int x2, int y2, int w, unsigned char color)
{
	int cx,cy;
	point a,b;
	point p1 = {x1,y1}, p2 = {x2,y2};
	
	//if(!comp_point(p1,p2)) return;
	
	if(y1 == y2){ // special case horizontal
		fill_box(x1, y1-(w/2), x2, y1+(w/2), color);
	}
	if(x1 == x2){ // special case vertical
		fill_box(x1-(w/1), y1, x1+(w/1), y2, color);
	}
}

int comp_point(point *pa, point *pb)
{
	if(pa->x == pb->x && pa->y == pb->y) return 0;
}

void add_point(point *pa, point *pb, point *pr)
{
	pr->x = pa->x + pb->x;
	pr->y = pa->y + pb->y;
}

void exit_error(char *str)
{
	write(1, str, strlen(str));
	exit(1);
}
