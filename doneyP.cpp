// 
// CreatedBy: Doney Peters
// Date: 2/14/2020
//

#include <GL/glx.h>
#include "fonts.h"
#include "image.h"


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

//Image doneyImg = "./images/doneyImg.png";

void renderDoneyImage(GLuint texture, int yres, int xres)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
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


