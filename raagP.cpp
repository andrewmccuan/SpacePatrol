// Created Raag Patel
// 02/17/2020

// For bullets
for (int i=0; i<g.nbullets; i++) {
        Bullets *b = &g.barr[i];
        //Log("draw bullet...\n");
        glColor3f(1.5, 1.5, 1.5);
        glBegin(GL_POINTS);
        glVertex2f(b->pos[0],      b->pos[1]);
        glVertex2f(b->pos[0]-1.0f, b->pos[1]);
        glVertex2f(b->pos[0]+1.0f, b->pos[1]);
        glVertex2f(b->pos[0],      b->pos[1]-1.0f);
        glVertex2f(b->pos[0],      b->pos[1]+1.0f);
        glColor3f(1.0, 1.0, 1.0);
        glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
        glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
        glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
        glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
        glEnd();
    }
}


void raag_text(int yres, int kes)
{
    Rect r;
    r.bot = yres-20;
    r.ley = kes/2;
    r.center = 0;
    ggprint8b(&r,75, 0c4b3ecf, "Raag Patel");
}
