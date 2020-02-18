// CMPS 3350
// Author: Andrew Mccuan
// Final Project

#include <GL/glx.h>
#include "fonts.h"

/*
Image andrewImg = "./images/andrewImg.png";

GLuint andrewCreditTex;

void andrew_generateCreditTex()
{
	glGenTextures(1, &andrewCreditTex;
	int w = andrewImg.width;
	int h = andrewImg.height;
	//int w = pic.width;
	//int h = pic.height;
	//
	glBindTexture(GL_TEXTURE_2D, andrewCreditTex);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, andrewImg.data); //pic.data
}
*/

void andrew_creditPic() 
{
    glColor3ub(255, 255, 255);
	glBindTexture(GL_TEXTURE_2D, andrewCreditTex);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(400.0f, 400.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(500.0f, 400.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(500.0f, 300.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(400.0f, 300.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void andrew_credit_text()
{
    Rect r;
	glClear(GL_COLOR_BUFFER_BIT);
	r.bot = gl.yres - 20;
	r.left = 10;
	r.center = 0;
    ggprint8b(&r, 75, 0x0000ffff, "Andrew Mccuan");
}