// William Source File
// Created February 2020
// Some work from friday was added
#define PORT 443
#define USERAGENT "CMPS-3350"
#include <algorithm>
#include <GL/glx.h>
#include "fonts.h"
#include "asteroid.h"
#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>

using namespace std;
//extern class Asteroid { };

void draw_will_text(int yres, int xres)
{
	Rect r;
	r.bot = yres - 35;
	r.left = xres/2;
	r.center = 0;
	ggprint8b(&r, 20, 0x00ff0000, "William Sparks - Space Patrol");
}

void det_coll(int yres, int xres)
{
	Rect r;
	r.bot = yres - 75;
	r.left = xres/2;
	r.center = 0;
	ggprint8b(&r, 25, 0x00ff0000, "Collision Detected!");
}

int movement(int flag)
{
	int arr[2] = {-2, 2};
	return arr[flag];
}

int destroy_ship(float xval, float yval, Asteroid *a)
{
	while (a) {
		if ((((xval) - (a->pos[0] - a->radius)) >= 0) &&
			(((a->pos[0] + a->radius)) - xval >= 0) &&  	
			(((yval) - (a->pos[1] - a->radius)) >= 0) &&
			(((a->pos[1] + a->radius)) - yval >= 0)) {

			return 1;
		}
		a = a->next;
	}
	return 0;
}

void new_ship(Ship *my_enemy, int countr) {
	int xpos = 0;
	int v2;
	bool ypos = 0;
	if (countr == 0) {
		srand (time(NULL));
	}
	my_enemy->angle = 90;
	my_enemy->valid_enemy = 1;
	my_enemy->color[0] = my_enemy->color[1] = my_enemy->color[2] = 1.0;
	xpos = (Flt)(rand() % 1250);
	my_enemy->pos[0] = xpos;
	my_enemy->numbullets = 0;
	ypos = rand()%2;
	
	v2 = rand() % 2 + 1;

	if (ypos == 0) {
		my_enemy->pos[1] = ypos;
		my_enemy->vel[1] = 3*v2;
	} else {
		my_enemy->pos[1] = 900;
		my_enemy->vel[1] = -3*v2;
	}

	v2 = rand() % 2 + 1;
	
	if (xpos < 400) {
		my_enemy->vel[0] = 2*v2;
	} else {
		my_enemy->vel[0] = -2*v2;
	}

	my_enemy->pos[2] = 0.0f;
}



int * high_score(int score)
{
	const int MAX_READ_ERRORS = 100;
	string myStrings[256];	
	BIO *ssl_setup_bio(void);
	void show_cert_data(SSL *ssl, BIO *outbio, const char *hostname);
	void set_to_non_blocking(const int sock);
	int sd;
	struct hostent *host;
	struct sockaddr_in addr;
	BIO *outbio = NULL;
	const SSL_METHOD *method;
	SSL_CTX *ctx;
	SSL *ssl;
	char req[1000];
	int req_len;
	char hostname[256] = "odin.cs.csub.edu";
	char pagename[256] = "/~dpeters/highScore.php?param=";
	int port = PORT;
	int flag = 0;	
	int bytes, nreads, nerrs;
	char buf[256];
	int ret;
	char buf2[1024];

	string str = to_string(score);
	strcat(pagename, str.c_str());
	cout << pagename << endl;					

	//
	//Setup the SSL BIO
	outbio = ssl_setup_bio();
	//Initialize the SSL library
	if (SSL_library_init() < 0) {
		BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
	}

	method = SSLv23_client_method();
	ctx = SSL_CTX_new(method);
	SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);
	//next 2 lines of code are not currently needed.
	//SSL_MODE_AUTO_RETRY flag of the SSL_CTX_set_mode call.
	//SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);
	//
	//Setup the socket used for connection.
	host = gethostbyname(hostname);
	sd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = *(long*)(host->h_addr);
	if (connect(sd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		BIO_printf(outbio, "Cannot connect to host %s [%s] on port %d.\n",
				hostname, inet_ntoa(addr.sin_addr), port);
	}
	//Connect using the SSL certificate.
	ssl = SSL_new(ctx);
	SSL_set_fd(ssl, sd);
	SSL_connect(ssl);
	//
	//Show the certificate data
	show_cert_data(ssl, outbio, hostname);
	//
	//A non-blocking socket will make the ssl_read() not block.
	set_to_non_blocking(sd);
	//
	//Send the http request to the host server.
	sprintf(req, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n",
			pagename, hostname, USERAGENT);
	req_len = strlen(req);
	ret = SSL_write(ssl, req, req_len);
	if (ret <= 0) {
		fprintf(stderr, "ERROR: SSL_write\n"); fflush(stderr);
	}	
	//
	//Get data returned from the server.
	//First, do priming read.
	//We can take this approach because our socket is non-blocking.
	//Start with an error condition.
	bytes = -1;
	memset(buf, '\0', sizeof(buf));
	memset(buf, '\0', sizeof(buf2));	
	int j = 0;
	int k = 0;
	while(bytes <= 0){
		flag = 0;		
		bytes = SSL_read(ssl, buf, sizeof(buf));
		//cout << "Bytes" << bytes << "Buf" << buf << endl;
		k = 0;

		for (int i = 0; i < 256; i++) {
			if (buf[i] == 'U' && buf[i + 1] == 'T' && buf[i + 2] == 'F') {
				flag = 1;
				i = i + 5;
			}
			if (flag == 1) {
				if (buf[i] == '0' || buf[i] == '1' || buf[i] == '2' ||
						buf[i] == '3' || buf[i] == '4' ||
						buf[i] == '5' || buf[i] == '6' ||
						buf[i] == '7' || buf[i] == '8' ||
						buf[i] == '9' ) {
					buf2[k] = buf[i];
					k++;	
				}

				if (i == sizeof(buf) - 1 ) {
					buf2[k] = '\0';
					myStrings[j] = buf2;
					k = 0;
					j++;

				}
				if (buf[i] == '<' ) {
					//cout << "In if: " << i << endl;
					buf2[k] = '\0';
					//cout << "Buf2 " << buf2 << endl;
					i = i + 2;
					k = 0;
					myStrings[j] = buf2;
					j++;	
				}
			}
		}
		//cout << "Buf2 " << buf2 << endl;
		//cout << "End of buf read" << endl;	
		//A slight pause can cause fewer reads to be nm,
		//A slight pause can cause fewer reads to be nm,
		usleep(10000);
	}
	//A successful priming read was accomplished.
	//Now read all the data.
	nreads = 1;
	//Allow for some read errors to happen, while getting the complete data.
	nerrs = 0;
	//cout << "BUf out" << buf << endl;	
	while(bytes >= 0 && nerrs < MAX_READ_ERRORS){
		k = 0;
		//cout << "Bytes" << bytes << "Buf" << buf << endl;	
		write(STDOUT_FILENO, buf, bytes);
		memset(buf, '\0', sizeof(buf));
		++nreads;
		bytes = SSL_read(ssl, buf, sizeof(buf));	
		for (int i = 0; i < 256; i++) {
			if (buf[i] == 'U' && buf[i + 1] == 'T' && buf[i + 2] == 'F') {
				flag = 1;
				i = i + 5;
			}
			if (flag == 1) {
				if (buf[i] == '0' || buf[i] == '1' || buf[i] == '2' ||
						buf[i] == '3' || buf[i] == '4' ||
						buf[i] == '5' || buf[i] == '6' ||
						buf[i] == '7' || buf[i] == '8' ||
						buf[i] == '9' ) {

					buf2[k] = buf[i];
					k++;	
				}

				if (i == sizeof(buf) - 1) {
					buf2[k+1] = '\0';
					myStrings[j] = buf2;
					k = 0;
					j++;

				}
				if (buf[i] == '<' ) {
					//cout << "In if: " << i << endl;
					buf2[k] = '\0';
					//cout << "Buf2 " << buf2 << endl;
					i = i + 2;
					k = 0;
					myStrings[j] = buf2;
					j++;	
				}
			}
		}

		if (bytes == 0) ++nerrs; else nerrs = 0;
		//A slight pause can cause fewer reads to be needed.
		usleep(20000);
	}

	int myVals[j];
	int * ret_arr;
	ret_arr = (int*) malloc(5*sizeof(int));
	for (int i = 0; i <= j; i++) {
		myVals[i] = atoi(myStrings[i].c_str());
	}
	sort(myVals, myVals + j+1);

	for (int i = 0; i < 5; i++) {
		ret_arr[i] = myVals[j - i];
	}	

	printf("\nn calls to ssl_read(): %i\n", nreads); fflush(stdout);
	//Cleanup.
	SSL_free(ssl);
	close(sd);
	SSL_CTX_free(ctx);

	return ret_arr;
}

