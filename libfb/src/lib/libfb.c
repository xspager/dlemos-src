#include "libfb.h"

int fb;
byte *scr;

void lfb_memset8(void *, unsigned int, size_t);
void lfb_memset32(void *, unsigned int, size_t);
void lfb_set_pixel8(int, Color);
void lfb_set_pixel32(int, Color);
	
void lfb_fill_scr(Color);
void lfb_draw_line(Point, Point, int, Color);
void lfb_exit_error(char *);
void lfb_fill_box(int x, int y, int w, int h, Color);
void lfb_draw_polygon(Point *, int, Color);
void lfb_refresh();

struct fb_var_screeninfo fb_var_info;
struct fb_fix_screeninfo fb_fix_info;

void lfb_init()
{	
	fb = open("/dev/fb0", O_RDWR);
	if(fb < 0){
		printf("Can`t open /dev/fb0\n");
		fb = open("/dev/graphics/fb0", O_RDWR);
		if(fb < 0)
			lfb_exit_error("Can`t open /dev/graphics/fb0\n");
	}
	
	ioctl(fb, FBIOGET_VSCREENINFO, &fb_var_info);
	ioctl(fb, FBIOGET_FSCREENINFO, &fb_fix_info);
	
	// set some constants
	lfb.width = fb_var_info.xres;
	lfb.height = fb_var_info.yres;
	lfb.bpp = fb_var_info.bits_per_pixel;
	
	lfb.fillbox = &lfb_fill_box;
	lfb.drawpolygon = &lfb_draw_polygon;
	lfb.fillscr = &lfb_fill_scr;
	lfb.drawline = &lfb_draw_line;
	lfb.refresh = &lfb_refresh;
	
	switch(lfb.bpp){
		case 8:
			lfb.memset = &lfb_memset8;
			lfb.setpixel = &lfb_set_pixel8;
			break;
		case 16:
		case 24:
		case 32:
			lfb.memset = &lfb_memset32;
			lfb.setpixel = &lfb_set_pixel32;
			break;
	}
	
	//printf("Screen size %dx%d\n", lfb.width, lfb.height);

	scr = (unsigned char *) mmap(0, fb_fix_info.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
	scr+= (fb_var_info.xoffset + fb_var_info.yoffset * fb_var_info.xres_virtual) * (fb_var_info.bits_per_pixel >> 3);
}

void lfb_fill_scr(Color c)
{
	lfb.memset(scr, c, lfb.width * lfb.height );
}

void lfb_fill_box(int x, int y, int w, int h, Color color)
{
	int cy,cx;

	if(x+w > lfb.width) w = lfb.width - x;
	if(y+h > lfb.height) h = lfb.height - y;

	for(cy = y; cy < y + h; cy++)
		lfb.memset(((unsigned int *)scr) + (lfb.width * cy + x), color, w);
}

void lfb_draw_line(Point a, Point b, int w, Color color)
{
	int x,y,xl,yl,i,xt,yt;
	float d,B;
	int dx, dy;

//	printf("%d,%d -> %d,%d\n", a.x, a.y, b.x, b.y);

	if(a.y == b.y)		// horizontal
		if(b.x < a.x)
			lfb_fill_box(b.x, b.y-(w>>1), a.x - b.x, w, color);
		else
			lfb_fill_box(a.x, a.y-(w>>1), b.x - a.x, w, color);
	else if(a.x == b.x)	// vertical		
		if(b.y < a.y)
			lfb_fill_box(b.x-(w>>1), b.y, w, a.y - b.y, color);
		else
			lfb_fill_box(a.x-(w>>1), a.y, w, b.y - a.y, color);
	else{
		dx = (b.x > a.x) ? b.x - a.x : (a.x - b.x);
		dy = (b.y > a.y) ? b.y - a.y : (a.y - b.y);
		
		if(dx && dy){
			if(dx > dy){	// we will run trough x
	//			printf("dx > dy\n");
				d = (float) dy / (float) dx;
				if(b.x > a.x){
	//				printf("b.x > a.x\n");
					x = a.x;
					xl = b.x;
					y = a.y;
					i = ((b.y - y) < 0) ? -1 : 1;
				}
				else{
	//				printf("b.x < a.x\n");
					x = b.x;
					xl = a.x;
					y = b.y;
					i = ((a.y - y) < 0) ? -1 : 1;
				}
				B = rint(y - d * x);
				while(x < xl){
					x++;
					y = rint(d * x) + B * i;
					lfb.setpixel(lfb.width * y + x, color);
				}
			}
			else{		// we will run though y
	//			printf("dx < dy\n");
				d = (float) dx / (float) dy;
				if(b.y > a.y){
	//				printf("b.y > a.y\n");
					y = a.y;
					yl = b.y;
					x = a.x;
					i = ((b.x - x) < 0) ? -1 : 1;
				}
				else{
	//				printf("b.y < a.y\n");
					y = b.y;
					yl = a.y;
					x = b.x;
					i = ((a.x - x) < 0) ? -1 : 1;
				}
				B = rint(x - d * y);
				while(y < yl){
					y++;
					x = rint(d * y) + B * i;
					lfb.setpixel(lfb.width * y + x, color);
				}
			}
		}
	}
}

/*
 * Pa     
 *       Pb 
 */
//void draw_box(Point )

void lfb_draw_polygon(Point *points, int w, Color c)
{
	Point *p = points;
	
	while(p->x > 0 && p->y > 0 && (p+1)->x > 0 && (p+1)->y > 0){
		lfb.drawline(*p, *(p+1),  w, c);
		p++;
	}
}

void lfb_set_box(int x, int y, int w, int h)
{
	int i;

	if(x + w > lfb.width && y + h > lfb.height){
		for(i = y; i < h; i++);
		//	lfb_set_line(
	} 
}

int lfb_comp_point(Point *pa, Point *pb)
{
	if(pa->x == pb->x && pa->y == pb->y) return 0;
}

void lfb_add_point(Point *pa, Point *pb, Point *pr)
{
	pr->x = pa->x + pb->x;
	pr->y = pa->y + pb->y;
}

void lfb_exit_error(char *str)
{
	write(1, str, strlen(str));
	exit(1);
}

void lfb_memset8(void *dst, unsigned int b, size_t len){
	memset(dst, b, len);
}

void lfb_memset32(void *dst, unsigned int b, size_t len){
	wmemset(dst, b, len);
}

void lfb_set_pixel8(int offset, Color c){
	if(offset < 0) offset = 0;
	if(offset > lfb.width * lfb.height) offset = lfb.width * lfb.height;
	*(scr + offset) = (char) c;
}

void lfb_set_pixel32(int offset, Color c){
	if(offset < 0) return;
	if(offset > lfb.width * lfb.height) return;
	*(((unsigned int *) scr) + offset) = c;
}

void lfb_refresh()
{
	fb_var_info.activate |= FB_ACTIVATE_NOW | FB_ACTIVATE_FORCE;
	fb_var_info.yres_virtual = fb_var_info.yres * 2;
	fb_var_info.yoffset = fb_var_info.yres;
	ioctl(fb, FBIOGET_VSCREENINFO, &fb_var_info);	
}

void openKB()
{
}	
