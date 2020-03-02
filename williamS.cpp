// William Source File
// Created February 2020
// Some work from friday was added

#include <GL/glx.h>
#include "fonts.h"
#include "asteroid.h"
#include <cstdlib>
#include <time.h> 

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

void new_ship(Ship *my_enemy, int countr) {
	int xpos = 0;
	int v2;
	bool ypos = 0;
	if (countr == 0) {
		srand (time(NULL));
	}
	my_enemy->angle = 90;
	my_enemy->valid_enemy = 1;
	my_enemy->color[0] = my_enemy->color[1] = my_enemy->color[2] = 1.0;
	xpos = (Flt)(rand() % 1250);
	my_enemy->pos[0] = xpos;
	my_enemy->numbullets = 0;
	ypos = rand()%2;
	
	v2 = rand() % 2 + 1;

	if (ypos == 0) {
		my_enemy->pos[1] = ypos;
		my_enemy->vel[1] = 3*v2;
	} else {
		my_enemy->pos[1] = 1250;
		my_enemy->vel[1] = -3*v2;
	}

	v2 = rand() % 2 + 1;
	
	if (xpos < 400) {
		my_enemy->vel[0] = 2*v2;
	} else {
		my_enemy->vel[0] = -2*v2;
	}

	my_enemy->pos[2] = 0.0f;
}
 
