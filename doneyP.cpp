// 
// CreatedBy: Doney Peters
// Date: 2/14/2020
//

#include <GL/glx.h>
#include "fonts.h"

void renderDoneyTextCredits (int yres, int xres)
{
    Rect r;
    r.bot = yres;
    r.left = xres;
    r.center = 0
    ggprint8b(&r, 75, 0x0000ffff, "Doney P.");
}
