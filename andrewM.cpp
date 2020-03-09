// CMPS 3350
// Author: Andrew Mccuan
// Final Project

#include <GL/glx.h>
#include <unistd.h>
#include <stdio.h>
#include "fonts.h"
#include "image.h"

//from lab3get.cpp
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

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

/*
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
*/

void show_cert_data(SSL *ssl, BIO *outbio, const char *hostname) 
{
	//Display ssl certificate data here.
	//Get the remote certificate into the X509 structure
	printf("--------------------------------------------------------------\n");
	printf("Certificate data...\n");
	X509 *cert;
	X509_NAME *certname;
	printf("calling SSL_get_peer_certificate(ssl)\n");
	cert = SSL_get_peer_certificate(ssl);
	if (cert == NULL)
		printf("Error: Could not get a certificate from: %s.\n", hostname);
	else
		printf("Retrieved the server's certificate from: %s.\n", hostname);
	//extract various certificate information
	certname = X509_NAME_new();
	certname = X509_get_subject_name(cert);
	//display the cert subject here
	if (BIO_printf(outbio, "Displaying the certificate subject data:\n") < 0)
		fprintf(stderr, "ERROR: BIO_printf\n");
	X509_NAME_print_ex(outbio, certname, 0, 0);
	if (BIO_printf(outbio, "\n\n") < 0)
		fprintf(stderr, "ERROR: BIO_printf\n");
	printf("--------------------------------------------------------------\n");
}

void andrewShowMenu(GLuint texture1, GLuint texture2, int xres, int yres)
{
	static float ang = 0.0;
	//Back Image
	//*
	
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture1);
    glBegin(GL_QUADS);
    	glTexCoord2f(0.0f, 1.0f); glVertex2i(0,0);
    	glTexCoord2f(0.0f, 0.0f); glVertex2i(0,yres);
    	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres,yres);
    	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres,0);
    glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	//*/

	//Front moving Image
	//*
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	//glLoadIdentity();
	glTranslatef(0.0f,.0f,0.0f);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
	glRotatef(ang, 0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    	glTexCoord2f(0.0f, 1.0f); glVertex2i(0,0);
    	glTexCoord2f(0.0f, 0.0f); glVertex2i(0,yres);
    	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres,yres);
    	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres,0);
    glEnd();
	glDisable(GL_ALPHA_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	//*/
	ang += 4.0;
}


//Sets up and binds the background texture
void andrewBackImg(GLuint texture, int xres, int yres, float xc[], float yc[])
{
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
		glTexCoord2f(xc[0], yc[1]); glVertex2i(0, 0);
		glTexCoord2f(xc[0], yc[0]); glVertex2i(0, yres);
		glTexCoord2f(xc[1], yc[0]); glVertex2i(xres, yres);
		glTexCoord2f(xc[1], yc[1]); glVertex2i(xres, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

//Moves the Background Texture with
//every physics call
void andrewBackImgMove(float* xc)
{
	xc[0] += 0.0005;
	xc[1] += 0.0005;
}

void andrewDrawShip(GLuint texture, float* pos)
{
	glColor3f(1.0, 1.0, 0.0);
	int width=40;
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    	glTexCoord2f(0.0f, 1.0f); glVertex2i(-width,-width);
    	glTexCoord2f(0.0f, 0.0f); glVertex2i(-width, width);
    	glTexCoord2f(1.0f, 0.0f); glVertex2i( width, width);
    	glTexCoord2f(1.0f, 1.0f); glVertex2i( width,-width);
    glEnd();
	glDisable(GL_ALPHA_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

void andrewDrawEnemy(GLuint texture, float* pos)
{
	glColor3f(1.0, 1.0, 0.0);
	int width=30;
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    	glTexCoord2f(0.0f, 1.0f); glVertex2i(-width,-width);
    	glTexCoord2f(0.0f, 0.0f); glVertex2i(-width, width);
    	glTexCoord2f(1.0f, 0.0f); glVertex2i( width, width);
    	glTexCoord2f(1.0f, 1.0f); glVertex2i( width,-width);
    glEnd();
	glDisable(GL_ALPHA_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

void andrew_credit_text(int yres, int xres)
{
    Rect r;
	r.bot = yres - 20;
	r.left = xres/2;
	r.center = 0;
    ggprint8b(&r, 75, 0x0000ffff, "Andrew Mccuan");
}

void andrewHelpMenu(int yres, int xres, int bot)
{
	//Note: Words are created from the lower left up
	// yres -20 words are built on
	//glClear(GL_COLOR_BUFFER_BIT);
	//Draw Rectangle
	int x1 = 5;
	int x2 = 180;
	int y1 = 5;
	int y2 = 160;
	glColor3f(0.0, 1.0, 1.0);
	glBegin(GL_QUADS);
    	glVertex2f((xres - x1), (yres - y1));
    	glVertex2f((xres - x2), (yres - y1));
    	glVertex2f((xres - x2), (yres - (y1 + 16)));
    	glVertex2f((xres - x1), (yres - (y1 + 16)));
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
    	glVertex2f((xres - x1), (yres - y1));
    	glVertex2f((xres - x2), (yres - y1));
    	glVertex2f((xres - x2), (yres - y2));
    	glVertex2f((xres - x1), (yres - y2));
	glEnd();

	Rect r;
	r.bot = bot;
	r.bot = yres - 20;
	//r.left = 10;
	r.left = xres - 175;
	r.center = 0;
	ggprint8b(&r, 16, 0x00000000, "HELP MENU [H]");
	ggprint8b(&r, 16, 0x0000ffff, "Keys:");
	ggprint8b(&r, 16, 0x0000ffff, "[Arrow Keys] Move");
	ggprint8b(&r, 16, 0x0000ffff, "[Space] Shoot");
	ggprint8b(&r, 16, 0x0000ffff, "[TAB] Highscore (Scoreboard)");
	ggprint8b(&r, 16, 0x0000ffff, "[P] Highscore (Save Score)");
	ggprint8b(&r, 16, 0x0000ffff, "[H] Toggle Help Menu");
	ggprint8b(&r, 16, 0x0000ffff, "[C] Credits");
	ggprint8b(&r, 16, 0x0000ffff, "[D] if its Friday");
	
}

void andrewHighscoreBox(int yres, int xres, int score)
{
	int x1 = 75;
	//int x2 = 200;
	int y1 = 25;
	int y2 = 125;
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_QUADS);
    	glVertex2f(((xres / 2) + x1), (yres - y1));
    	glVertex2f(((xres / 2) - x1), (yres - y1));
    	glVertex2f(((xres / 2) - x1), (yres - (y1 + 16)));
    	glVertex2f(((xres / 2) + x1), (yres - (y1 + 16)));
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
    	glVertex2f(((xres / 2) + x1), (yres - y1));
    	glVertex2f(((xres / 2) - x1), (yres - y1));
    	glVertex2f(((xres / 2) - x1), (yres - y2));
    	glVertex2f(((xres / 2)+ x1), (yres - y2));
	glEnd();

	Rect r;
	r.bot = yres - (y1 + 15);
	//r.left = 10;
	r.left = xres/2 - 40;
	r.center = 0;
	ggprint8b(&r, 16, 0x00000000, "Scoreboard [TAB]");
	r.left = r.left - 5;
	ggprint8b(&r, 16, 0x00ffff00, "Your Score - %d", score);
	ggprint8b(&r, 16, 0x00ffff00, "Best Score - %d", score);

}