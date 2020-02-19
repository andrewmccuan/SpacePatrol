// Created Raag Patel
// 02/17/2020


#include <GL/glx.h>
#include "fonts.h"

void raag_text(int yres, int xres)
{
    Rect r;
    r.bot = yres-65;
    r.left = xres/2;
    r.center = 0;
    ggprint8b(&r,75, 0x4b3ecf, "Raag Patel");
}
