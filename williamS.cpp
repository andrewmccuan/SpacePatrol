// William Source File
// Created February 2020
// Some work from friday was added

#include <GL/glx.h>
#include "fonts.h"
#include "asteroid.h"
//extern class Asteroid { };

void draw_will_text(int yres, int xres)
{
	Rect r;
	r.bot = yres - 35;
	r.left = xres/2;
	r.center = 0;
	ggprint8b(&r, 20, 0x00ff0000, "William Sparks - Space Patrol");
}

void det_coll(int yres, int xres) 
{
	Rect r;
	r.bot = yres - 75;
	r.left = xres/2;
	r.center = 0;
	ggprint8b(&r, 25, 0x00ff0000, "Collision Detected!");
}

int movement(int flag) 
{
	int arr[2] = {-2, 2};
	return arr[flag];
}

int destroy_ship(float xval, float yval, Asteroid *a)
{
	while (a) {
		if ((((xval) - (a->pos[0] - a->radius)) >= 0) &&
			(((a->pos[0] + a->radius)) - xval >= 0) &&  	
			(((yval) - (a->pos[1] - a->radius)) >= 0) &&
			(((a->pos[1] + a->radius)) - yval >= 0)) {

			return 1;
		}
		a = a->next;
	}
	return 0;
}
