// William Source File
// Created February 2020
// Some work from friday was added

#include <GL/glx.h>
#include "fonts.h"

void draw_will_text(int yres, int xres)
{
	Rect r;
	r.bot = yres - 35;
	r.left = xres/2;
	r.center = 0;
	ggprint8b(&r, 20, 0x00ff0000, "William Sparks - Space Patrol");
}


int movement(int flag) {
	int arr[2] = {-2, 2};
	return arr[flag];
}

