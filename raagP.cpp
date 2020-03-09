// Created Raag Patel
// 02/17/2020

#include <GL/glx.h>
#include "fonts.h"
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>


void raag_text(int yres, int xres)
{
    Rect r;
    r.bot = yres-65;
    r.left = xres/2;
    r.center = 0;
    ggprint8b(&r,75, 0xFFFF3C, "Raag Patel");
}


/*// Friday
//------------------------------------------------------------------------------------------------------
void raag_text(int yres, int xres)
{
    Rect r;
    r.bot = yres-65;
    r.left = xres/2;
    r.center = 0;
    ggprint8b(&r,75, 0xFFFF3C, "Friday --> The quick brown fox jumps over the lazy dog --> Raag Patel ");
    
}
//------------------------------------------------------------------------------------------------------
*/
void set_to_non_blocking(const int sock){
    //Set a socket to be non-blocking.
    int opts;
    opts = fcntl(sock, F_GETFL);
    if (opts < 0)
    {
        perror("ERROR: fcntl(F_GETFL)");
        exit(EXIT_FAILURE);
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(sock, F_SETFL, opts) < 0)
    {
        perror("ERROR: fcntl(O_NONBLOCK)");
        exit(EXIT_FAILURE);
    }
}


