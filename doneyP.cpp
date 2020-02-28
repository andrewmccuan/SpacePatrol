// 
// CreatedBy: Doney Peters
// Date: 2/14/2020
//

#include <GL/glx.h>
#include "fonts.h"

// Friday changed text color
void renderDoneyTextCredits(int yres, int xres)
{
    Rect r;
    r.bot = yres - 50;
    r.left = xres/2;
    r.center = 0;
    ggprint8b(&r, 75, 0x8926FF, "Doney Peters - CEO (Chief Enemy Officer)");
}

//For Friday exercise 2/21/2020
void renderTGIF (int yres, int xres)
{
    Rect r;
    r.bot = yres/2;
    r.left = xres/2;
    r.center = 0;
    ggprint8b(&r, 75, 0x8926DD, "TGIF - Thank God Its Friday");
}




    
