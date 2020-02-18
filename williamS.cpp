// William Source File
// Created February 2020
//
void draw_will_text(int yres, int xres)
{
        Rect r;
        glClear(GL_COLOR_BUFFER_BIT);
        //
        r.bot = yres - 50;
        r.left = 10;
        r.center = 0;
        ggprint8b(&r, 20, 0x00ff0000, "William Sparks - Space Patrol");
        glPushMatrix();
}

