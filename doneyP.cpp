// 
// CreatedBy: Doney Peters
// Date: 2/14/2020
//

#include <GL/glx.h>
#include "fonts.h"
#include "image.h"
#include "asteroid.h"
#include <stdlib.h>
#include <cmath>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

// Friday changed text color
void renderDoneyTextCredits(int yres, int xres)
{
    Rect r;
    r.bot = yres - 50;
    r.left = xres/2;
    r.center = 0;
    ggprint8b(&r, 75, 0x8926FF, "Doney Peters");
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

void renderDoneyImage(GLuint texture, int yres, int xres)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glColor3f(1.0,1.0,1.0);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(0,0);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(0,yres);
    glTexCoord2f(1.0f, 0.0f); glVertex2i(xres,yres);
    glTexCoord2f(1.0f, 1.0f); glVertex2i(xres,0);
    glEnd();
}

BIO *ssl_setup_bio(void)
{
    //Setup the ssl BIO, basic I/O abstraction.
    //https://www.openssl.org/docs/man1.1.0/man3/bio.html
    BIO *bio = NULL;
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    SSL_load_error_strings();
    bio = BIO_new(BIO_s_file());
    bio = BIO_new_fp(stdout, BIO_NOCLOSE);
    return bio;
}

void renderPowerUps(PowerUp *ahead)
{
	PowerUp *a = ahead;
	while (a) {
		//Log("draw Power Up...\n");
		glColor3fv(a->color);
		glPushMatrix();
		glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
		glBegin(GL_LINE_LOOP);
		glEnd();
		glPopMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2f(a->pos[0], a->pos[1]);
		glEnd();
		a = a->next;
	}
}

void buildPowerUp(PowerUp *p, int yres, float random)
{
	srand(time(NULL));
	p->radius = 40.0/2;
	p->pos[0] = 1500;
	p->pos[1] = (Flt)(rand() % yres);
	p->pos[2] = 0.5f;
	p->angle = 0.0;
	p->rotate = 0.0;
	p->color[0] = 0.6;
	p->color[1] = 0.6;
	p->color[2] = 0.6;
	p->vel[0] = (Flt)(abs(random) * 2.0);
	p->vel[1] = (Flt)(abs(random) * 2.0);

}

void updatePowerUpPosition (PowerUp *p, int yres, int xres)
{
	while (p) {
		p->pos[0] -= p->vel[0];
		//p->pos[1] -= p->vel[1];
		//Check for collision with window edges
		if (p->pos[0] < -100.0) {
			p->pos[0] += (float)xres+200;
		}
		else if (p->pos[0] > (float)xres+100) {
			p->pos[0] -= (float)xres+200;
		}
		else if (p->pos[1] < -100.0) {
			p->pos[1] += (float)yres+200;
		}
		else if (p->pos[1] > (float)yres+100) {
			p->pos[1] -= (float)yres+200;
		}
		p = p->next;
	}
}
void renderPowerUpImage(GLuint texture, float* pos)
{
	glColor3f(1.0,1.0,1.0);
	int width = 20;
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
    	glTexCoord2f(0.0f, 1.0f); glVertex2i(-width,-width);
    	glTexCoord2f(0.0f, 0.0f); glVertex2i(-width, width);
    	glTexCoord2f(1.0f, 0.0f); glVertex2i(width, width);
    	glTexCoord2f(1.0f, 1.0f); glVertex2i(width,-width);
    glEnd();
	glDisable(GL_ALPHA_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

int detectPowerUpCollisionWithShip(float xval, float yval, PowerUp *p)
{
	while (p) {
		if ((((xval) - (p->pos[0] - p->radius)) >= 0) &&
			(((p->pos[0] + p->radius)) - xval >= 0) &&  	
			(((yval) - (p->pos[1] - p->radius)) >= 0) &&
			(((p->pos[1] + p->radius)) - yval >= 0)) {

			return 1;
		}
		p = p->next;
	}
	return 0;
}

// Weapon Upgrade One
void powerUpOne(Bullet *barr, int nbullets, int MAX_BULLETS, Ship ship, 
				double rand, double PI)
{
	if (nbullets < MAX_BULLETS) {
		Bullet *b = &barr[nbullets];
		b->pos[0] = ship.pos[0];
		b->pos[1] = ship.pos[1];
		b->vel[0] = ship.vel[0];
		b->vel[1] = ship.vel[1];
		//convert ship angle to radians
		Flt rad = ((ship.angle+135.0) / 360.0f) * PI * 2.0;
		//convert angle to a vector
		Flt xdir = cos(rad);
		Flt ydir = sin(rad);
		b->pos[0] += xdir*20.0f;
		b->pos[1] += ydir*20.0f;
		b->vel[0] += xdir*6.0f + rand*0.1;
		b->vel[1] += ydir*6.0f + rand*0.1;
		b->color[0] = 1.0f;
		b->color[1] = 1.0f;
		b->color[2] = 1.0f;
	}
}

//Weapon Upgrade Two
void powerUpTwo(Bullet *barr, int nbullets, int MAX_BULLETS, Ship ship, 
				double rand, double PI)
{
	if (nbullets < MAX_BULLETS) {
		Bullet *b = &barr[nbullets];
		b->pos[0] = ship.pos[0];
		b->pos[1] = ship.pos[1];
		b->vel[0] = ship.vel[0];
		b->vel[1] = ship.vel[1];
		//convert ship angle to radians
		Flt rad = ((ship.angle+45.0) / 360.0f) * PI * 2.0;
		//convert angle to a vector
		Flt xdir = cos(rad);
		Flt ydir = sin(rad);
		b->pos[0] += xdir*20.0f;
		b->pos[1] += ydir*20.0f;
		b->vel[0] += xdir*6.0f + rand*0.1;
		b->vel[1] += ydir*6.0f + rand*0.1;
		b->color[0] = 1.0f;
		b->color[1] = 1.0f;
		b->color[2] = 1.0f;
	}
}
