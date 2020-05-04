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
		//Log("%i verts\n",a->nverts);
		for (int j=0; j<a->nverts; j++) {
			glVertex2f(a->vert[j][0], a->vert[j][1]);
		}
		glEnd();
		glPopMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2f(a->pos[0], a->pos[1]);
		glEnd();
		a = a->next;
	}
}

PowerUp* buildPowerUp(PowerUp *p, int yres, float random, float PI)
{
	p->nverts = 4;
	p->radius = 40.0/2;
	Flt r2 = p->radius / 2.0;
	Flt angle = 0.0f;
	Flt inc = (PI * 2.0) / (Flt)p->nverts;
	for (int i=0; i<p->nverts; i++) {
		p->vert[i][0] = sin(angle) * (r2 + random * p->radius);
		p->vert[i][1] = cos(angle) * (r2 + random * p->radius);
		angle += inc;
	}
	p->pos[0] = 1500;
	p->pos[1] = (Flt)(rand()-20 % yres);
	p->pos[2] = 0.5f;
	p->angle = 0.0;
	p->rotate = 0.0;
	p->color[0] = 0.6;
	p->color[1] = 0.6;
	p->color[2] = 0.6;
	p->vel[0] = (Flt)(random *  2.0 - 1.0);
	p->vel[1] = (Flt)(random * 2.0 - 1.0);

	return p;
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



