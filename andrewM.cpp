// CMPS 3350
// Author: Andrew Mccuan
// Final Project

#include <GL/glx.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include "fonts.h"
#include "image.h"

//from lab3get.cpp
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 100
#define BUTTON_BACK 50

static int ylocation = 0;

void closeTheGame();

void andrewShowCredits(GLuint texture1, GLuint texture2, int xres, int yres)
{
	glColor3f(1.0,1.0,1.0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture1);
    glBegin(GL_QUADS);
    	glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
   		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
    	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
    	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
    glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Back Button
	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture2);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex2i(BUTTON_BACK, BUTTON_BACK);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex2i(BUTTON_BACK, BUTTON_BACK + BUTTON_HEIGHT);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex2i(BUTTON_BACK + (BUTTON_WIDTH * 2),
				BUTTON_BACK + BUTTON_HEIGHT);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex2i(BUTTON_BACK + (BUTTON_WIDTH * 2), BUTTON_BACK);
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void andrewShowHighscore(GLuint texture1, GLuint texture2, int xres, int yres)
{
	//Background Image
	glColor3f(1.0,1.0,1.0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture1);
    glBegin(GL_QUADS);
    	glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
   		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
    	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
    	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
    glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Back Button
	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture2);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex2i(BUTTON_BACK, BUTTON_BACK);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex2i(BUTTON_BACK, BUTTON_BACK + BUTTON_HEIGHT);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex2i(BUTTON_BACK + (BUTTON_WIDTH * 2),
				BUTTON_BACK + BUTTON_HEIGHT);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex2i(BUTTON_BACK + (BUTTON_WIDTH * 2), BUTTON_BACK);
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

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

void andrewDrawMouse(int mouseX, int mouseY)
{
	glColor3f(0.0, 1.0, 0.0);
		glPushMatrix();
		glTranslatef(mouseX, mouseY, 0.0);
		glBegin(GL_TRIANGLES);
			glVertex2f(-12.0f, -10.0f);
			glVertex2f(  0.0f, 20.0f);
			glVertex2f(  0.0f, -6.0f);
			glVertex2f(  0.0f, -6.0f);
			glVertex2f(  0.0f, 20.0f);
			glVertex2f( 12.0f, -10.0f);
		glEnd();
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2f(0.0f, 0.0f);
		glEnd();
		glPopMatrix();
}

//From my lab12.cpp in 3480 folder
void rotate_square(int sqr[4][2], int sqr2[4][2], float ang)
{
	//Find the centroid of the square
	int centroid[2];
	centroid[0] = (sqr[0][0] + sqr[1][0] + sqr[2][0] + sqr[3][0]) / 4;
	centroid[1] = (sqr[0][1] + sqr[1][1] + sqr[2][1] + sqr[3][1]) / 4;
	int xdiff = 0 - centroid[0];
	int ydiff = 0 - centroid[1];
	for (int i = 0; i < 4; i++) {
		sqr[i][0] += xdiff;
		sqr[i][1] += ydiff;
	}
	//Rotation Matrix
	double matrix[2][2];
	matrix[0][0] = cos(ang);
	matrix[0][1] = -sin(ang);
	matrix[1][0] = sin(ang);
	matrix[1][1] = cos(ang);
	for (int i = 0; i < 4; i++) {
		sqr2[i][0] = sqr[i][0] * matrix[0][0] + sqr[i][1] * matrix[1][0];
		sqr2[i][1] = sqr[i][0] * matrix[0][1] + sqr[i][1] * matrix[1][1];
	}
}


void andrewShowMenu(GLuint texture1, GLuint texture2, int xres, int yres)
{
	static float ang = 0.0;
	static int sqr[4][2];
	int sqr2[4][2];
	static int firsttime = 1;
	if (firsttime == 1) {
		sqr[0][0] = 0;
		sqr[0][1] = 0;
		sqr[1][0] = 0;
		sqr[1][1] = 400*5; //yres;
		sqr[2][0] = 400*5; //xres;
		sqr[2][1] = 400*5; //yres;
		sqr[3][0] = 400*5; //xres;
		sqr[3][1] = 0;
		firsttime = 0;
	}
	//Back Image
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture1);
    glBegin(GL_QUADS);
    	glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
    	glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
    	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
    glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Front moving Image
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(float(xres/2), float(yres/2),0.0f);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	rotate_square(sqr, sqr2, ang);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(sqr2[0][0], sqr2[0][1]);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(sqr2[1][0], sqr2[1][1]);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(sqr2[2][0], sqr2[2][1]);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(sqr2[3][0], sqr2[3][1]);
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	ang += 0.02;
}

void andrewShowButtons(GLuint tex1, GLuint tex2, GLuint tex3, GLuint tex4,
 		GLuint tex5, int xres, int yres)
{
	//Logo
	int x1 = 250;
	int y1 = 200;
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex1);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i((xres/2) - x1, yres - (50 + y1));
		glTexCoord2f(0.0f, 0.0f); glVertex2i((xres/2) - x1, yres - 50);
		glTexCoord2f(1.0f, 0.0f); glVertex2i((xres/2) + x1, yres - 50);
		glTexCoord2f(1.0f, 1.0f); glVertex2i((xres/2) + x1, yres - (50 + y1));
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Get the y value of the bottom of the logo
	//and distance the buttons equally from each other to 
	//the bottom of the screen
	static int firsttime = 1;
	if (firsttime == 1) {
		int bottomLogo = yres - (y1 + 50);
		int bufferFrame = 100; //gives 100pix buf from below the screen
		int spaceAvaliable = bottomLogo - (bufferFrame * 2); 
		ylocation = spaceAvaliable / 4; //4 is the # of buttons below the logo
		firsttime = 0;
	}

	//Play Button
	glColor3f(1.0, 1.0, 0.0);
	x1 = BUTTON_WIDTH;
	y1 = BUTTON_HEIGHT;
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex2);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex2i(((xres/2) - x1), (ylocation * 4));
		glTexCoord2f(0.0f, 0.0f); 
		glVertex2i(((xres/2) - x1), (ylocation * 4) + y1);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex2i(((xres/2) + x1), (ylocation * 4) + y1);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex2i(((xres/2) + x1), (ylocation * 4));
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//High Score Button
	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex3);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(((xres/2) - x1), (ylocation * 3));
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(((xres/2) - x1), (ylocation * 3) + y1);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i(((xres/2) + x1), (ylocation * 3) + y1);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i(((xres/2) + x1), (ylocation * 3));
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Credits Button
	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex4);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex2i(((xres/2) - x1), (ylocation * 2));
		glTexCoord2f(0.0f, 0.0f); 
		glVertex2i(((xres/2) - x1), (ylocation * 2) + y1);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex2i(((xres/2) + x1), (ylocation * 2) + y1);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex2i(((xres/2) + x1), (ylocation * 2));
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	//-------------------------------------------------
	//Exit Button
	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tex5);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex2i(((xres/2) - x1), (ylocation * 1));
		glTexCoord2f(0.0f, 0.0f); 
		glVertex2i(((xres/2) - x1), (ylocation * 1) + y1);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex2i(((xres/2) + x1), (ylocation * 1) + y1);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex2i(((xres/2) + x1), (ylocation * 1));
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void andrewButtonHighlight(int mouseX, int mouseY, int screen, int xres)
{
	int button = 0;
	int x1 = BUTTON_WIDTH;
	int y1 = BUTTON_HEIGHT;

	if (screen == 1) {
		if ((mouseX >= (xres / 2) - x1) && (mouseX <= (xres / 2) + x1)) {
			if ((mouseY >= (ylocation * 4)) && 
					(mouseY <= (ylocation * 4) + BUTTON_HEIGHT)) {
				button = 4;
			}
			if ((mouseY >= (ylocation * 3)) && 
					(mouseY <= (ylocation * 3) + BUTTON_HEIGHT)) {
				button = 3;
			}
			if ((mouseY >= (ylocation * 2)) && 
					(mouseY <= (ylocation * 2) + BUTTON_HEIGHT)) {
				button = 2;
			}
			if ((mouseY >= (ylocation)) && 
					(mouseY <= (ylocation) + BUTTON_HEIGHT)) {
				button = 1;
			}
		}
	}
	if (screen == 2) {
		if ((mouseX >= BUTTON_BACK) && 
				(mouseX <= BUTTON_BACK + (BUTTON_WIDTH * 2))) {
			if ((mouseY >= BUTTON_BACK) && 
					(mouseY <= BUTTON_BACK + BUTTON_HEIGHT)) {
				button = 10;
			}
		}
	}
	if (button != 0) {
		int widthLine = 2;
		if (button == 10) {
			glColor3f(1.0, 0.9, 0.0);
			glBegin(GL_QUADS);
				glVertex2f((BUTTON_BACK + (x1*2) + widthLine), 
						(BUTTON_BACK + y1 + widthLine));
				glVertex2f((BUTTON_BACK - widthLine), 
						(BUTTON_BACK + y1 + widthLine));
				glVertex2f((BUTTON_BACK - widthLine), 
						(BUTTON_BACK + y1 - widthLine));
				glVertex2f(BUTTON_BACK + (x1*2) + widthLine, 
						(BUTTON_BACK + y1 - widthLine));		
			glEnd();
			glBegin(GL_QUADS);
				glVertex2f((BUTTON_BACK + widthLine), 
						(BUTTON_BACK + y1 + widthLine));
				glVertex2f((BUTTON_BACK - widthLine), 
						(BUTTON_BACK + y1 + widthLine));
				glVertex2f((BUTTON_BACK - widthLine), 
						(BUTTON_BACK - widthLine));
				glVertex2f((BUTTON_BACK + widthLine), 
						(BUTTON_BACK - widthLine));
			glEnd();
			glBegin(GL_QUADS);
				glVertex2f((BUTTON_BACK + (x1*2) + widthLine), 
						(BUTTON_BACK + widthLine));
				glVertex2f((BUTTON_BACK - widthLine), 
						(BUTTON_BACK + widthLine));
				glVertex2f((BUTTON_BACK - widthLine), 
						(BUTTON_BACK - widthLine));
				glVertex2f(BUTTON_BACK + (x1*2) + widthLine, 
						(BUTTON_BACK - widthLine));
			glEnd();
			glBegin(GL_QUADS);
				glVertex2f((BUTTON_BACK + (x1*2) + widthLine), 
						(BUTTON_BACK + y1 + widthLine));
				glVertex2f((BUTTON_BACK + (x1*2) - widthLine), 
						(BUTTON_BACK + y1 + widthLine));
				glVertex2f((BUTTON_BACK + (x1*2) - widthLine), 
						(BUTTON_BACK - widthLine));
				glVertex2f((BUTTON_BACK + (x1*2) + widthLine), 
						(BUTTON_BACK - widthLine));
			glEnd();
		}
		else {
			glColor3f(1.0, 0.9, 0.0);
			glBegin(GL_QUADS);
				glVertex2f(((xres / 2) + x1 + widthLine), 
						(ylocation * button) + y1 + widthLine);
				glVertex2f(((xres / 2) - x1 - widthLine), 
						(ylocation * button) + y1 + widthLine);
				glVertex2f(((xres / 2) - x1 - widthLine), 
						(ylocation * button) + y1 - widthLine);
				glVertex2f(((xres / 2) + x1 - widthLine), 
						(ylocation * button) + y1 - widthLine);
			glEnd();
			glBegin(GL_QUADS);
				glVertex2f(((xres / 2) - x1 + widthLine), 
						(ylocation * button) + y1 + widthLine);
				glVertex2f(((xres / 2) - x1 - widthLine), 
						(ylocation * button) + y1 + widthLine);
				glVertex2f(((xres / 2) - x1 - widthLine), 
						(ylocation * button) - widthLine);
				glVertex2f(((xres / 2) - x1 + widthLine), 
						(ylocation * button) - widthLine);
			glEnd();
			glBegin(GL_QUADS);
				glVertex2f(((xres / 2) + x1 + widthLine), 
						(ylocation * button) + widthLine);
				glVertex2f(((xres / 2) - x1 - widthLine), 
						(ylocation * button) + widthLine);
				glVertex2f(((xres / 2) - x1 - widthLine), 
						(ylocation * button) - widthLine);
				glVertex2f(((xres / 2) + x1 - widthLine), 
						(ylocation * button) - widthLine);
			glEnd();
			glBegin(GL_QUADS);
				glVertex2f(((xres / 2) + x1 + widthLine), 
						(ylocation * button) + y1 + widthLine);
				glVertex2f(((xres / 2) + x1 - widthLine), 
						(ylocation * button) + y1 + widthLine);
				glVertex2f(((xres / 2) + x1 - widthLine), 
						(ylocation * button) - widthLine);
				glVertex2f(((xres / 2) + x1 + widthLine), 
						(ylocation * button) - widthLine);
			glEnd();	
		}
	}

}

int andrewButtonAction(int mouseX, int mouseY, int screen, int xres)
{
	int x1 = BUTTON_WIDTH;
	//int y1 = BUTTON_HEIGHT;
	if (screen == 1) {
		if ((mouseX >= (xres / 2) - x1) && (mouseX <= (xres / 2) + x1)) {
			if ((mouseY >= (ylocation * 4)) && 
				(mouseY <= (ylocation * 4) + BUTTON_HEIGHT)) {
				printf("PLAY BUTTON WAS PRESSED\n");
				return 1;
			}
			if ((mouseY >= (ylocation * 3)) && 
				(mouseY <= (ylocation * 3) + BUTTON_HEIGHT)) {
				printf("HIGH SCORES BUTTON WAS PRESSED\n");
				return 2;
			}
			if ((mouseY >= (ylocation * 2)) && 
				(mouseY <= (ylocation * 2) + BUTTON_HEIGHT)) {
				printf("CREDIT BUTTON WAS PRESSED\n");
				return 3;
			}
			if ((mouseY >= (ylocation)) && 
				(mouseY <= (ylocation) + BUTTON_HEIGHT)) {
				//printf("EXIT BUTTON WAS PRESSED\n");
				printf("Thank you for playing our game\n");
				//closeTheGame();
				return 4;
			}
		}
	}
	if (screen == 2) {
		if ((mouseX >= BUTTON_BACK) && 
				(mouseX <= BUTTON_BACK + (BUTTON_WIDTH * 2))) {
			if ((mouseY >= BUTTON_BACK) && 
					(mouseY <= BUTTON_BACK + BUTTON_HEIGHT)) {
				printf("MENU BUTTON WAS PRESSED\n");
				return 1;
			}
		}
	}
	return 0;
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
	r.bot = yres - 80;
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
	ggprint8b(&r, 16, 0x0000ffff, "[M] Menu");
	ggprint8b(&r, 16, 0x0000ffff, "[TAB] Highscore (Scoreboard)");
	ggprint8b(&r, 16, 0x0000ffff, "[P] Highscore (Save Score)");
	ggprint8b(&r, 16, 0x0000ffff, "[H] Toggle Help Menu");
	ggprint8b(&r, 16, 0x0000ffff, "[C] Credits");
	ggprint8b(&r, 16, 0x0000ffff, "[D] if its Friday");
	
}

void andrewHighscoreBox(int yres, int xres, int score, int* high_score)
{
	int x1 = 100;
	//int x2 = 200;
	int y1 = 25;
	int y2 = 195;
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_QUADS);
		glVertex2f(((xres / 2) + x1), (yres - y1));
		glVertex2f(((xres / 2) - x1), (yres - y1));
		glVertex2f(((xres / 2) - x1), (yres - (y1 + 22)));
		glVertex2f(((xres / 2) + x1), (yres - (y1 + 22)));
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
		glVertex2f(((xres / 2) + x1), (yres - y1));
		glVertex2f(((xres / 2) - x1), (yres - y1));
		glVertex2f(((xres / 2) - x1), (yres - y2));
		glVertex2f(((xres / 2) + x1), (yres - y2));
	glEnd();

	glColor3f(0.3, 0.3, 0.3);
	int j = 24;
	for (int i = 1; i < 6; i++) {
		glBegin(GL_LINES);
    		glVertex2f(((xres / 2) + x1), (yres - (y1 + (23 + (i*j)))));
    		glVertex2f(((xres / 2) - x1), (yres - (y1 + (23 + (i*j)))));
		glEnd();
	}

	Rect r;
	r.bot = yres - (y1 + 20);
	//r.left = 10;
	r.left = xres/2 - 65;
	r.center = 0;
	ggprint13(&r, 25, 0x00000000, "Scoreboard [TAB]");
	r.left = r.left - 20;
	ggprint13(&r, 24, 0x00ffaa00, "Your Score - %7d", score);
	//ggprint13(&r, 22, 0x00ffff00, "Your Score - %05d", score);
	ggprint13(&r, 24, 0x00ffd500, "1ST - %012d", high_score[0]);
	ggprint13(&r, 24, 0x00ffd500, "2ND - %012d", high_score[1]);
	ggprint13(&r, 24, 0x00ffd500, "3RD - %012d", high_score[2]);
	ggprint13(&r, 24, 0x00ffd500, "4TH - %012d", high_score[3]);
	ggprint13(&r, 24, 0x00ffd500, "5TH - %012d", high_score[4]);
}
