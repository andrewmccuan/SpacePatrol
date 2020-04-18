// William Source File
// Created February 2020
// Added random enemy movement after loop
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
	ggprint8b(&r, 20, 0x00ff0000, "William Sparks");
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

void new_ship(Ship *my_enemy, int countr, int yres, int xres) {
	int xpos = 0;
	int v2;
	bool ypos = 0;
	if (countr == 0) {
		srand (time(NULL));
	}
	my_enemy->angle = 90;
	my_enemy->valid_enemy = 1;
	my_enemy->rotated = 0;
	my_enemy->color[0] = my_enemy->color[1] = my_enemy->color[2] = 1.0;
	xpos = (Flt)(rand() % xres);
	my_enemy->pos[0] = xpos;
	my_enemy->numbullets = 0;
	ypos = rand()%2;
	my_enemy->first_call = 0;
	
	v2 = rand() % 2 + 1;

	if (ypos == 0) {
		my_enemy->pos[1] = ypos;
		my_enemy->vel[1] = 3*v2;
	} else {
		my_enemy->pos[1] = yres;
		my_enemy->vel[1] = -3*v2;
	}

	v2 = rand() % 2 + 1;
	
	if (xpos < xres/3) {
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
	while (bytes <= 0){
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
	while (bytes >= 0 && nerrs < MAX_READ_ERRORS){
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

int circ_mov (Ship *my_enemy)
{
	int dir_flag = 0;

	if (my_enemy->first_call == 1) {
		my_enemy->save_vel[0] = 3;
		my_enemy->save_vel[1] = 3;
		my_enemy->vel[0] = 3;
		my_enemy->vel[1] = 0;
		my_enemy->quadrant[0] = 4;
	}
	// Check to see if ship has performed a loop already
	if ((my_enemy->quadrant[0] == my_enemy->quadrant[1]) && 
			(my_enemy->first_call > 126)) {
		
		my_enemy->rotated = 0;
		return 1;
	}
	// to do: add the conditioninal to this if 
	if (1) {
		// Find which quadrant of the trajectory the ship is in 
		if (my_enemy->vel[0] >= 0 && my_enemy->vel[1] > 0) {
			if (my_enemy->first_call == 1) {
				my_enemy->quadrant[0] = 4;
			}

			if ((float)my_enemy->vel[0] < 0 ) {
				my_enemy->vel[1] = my_enemy->save_vel[1];
				my_enemy->vel[0] = 0;
				
				std::cout << "Changing velocity" << std::endl;
				//my_enemy->quadrant[1] = 1;
			} else {

				dir_flag = 1;
				my_enemy->quadrant[1] = 4;
			}
		}
		// Find which quadrant of the trajectory the ship is in 
		if ((float)my_enemy->vel[0] > 0 && (float)my_enemy->vel[1] <= 0) {
			// Determine if the ship is entering a new quadrant
			if (my_enemy->vel[1] > -0.1 && my_enemy->vel[1] < 0.1) {
				my_enemy->vel[1] = 0;
				my_enemy->vel[0] = my_enemy->save_vel[0];
				my_enemy->quadrant[1] = 4;
			} else {
				dir_flag = 2;
				my_enemy->quadrant[1] = 3;
			}
		}

		if ((float)my_enemy->vel[0] < 0 && (float)my_enemy->vel[1] >= 0) {
			//std::cout << "In 1st" << std::endl;
			if (my_enemy->vel[1] <= 0) {
				my_enemy->vel[1] = -1 * my_enemy->save_vel[1];
				my_enemy->vel[0] = 0;
				my_enemy->quadrant[1] = 2;
			} else {
				dir_flag = 3;
				my_enemy->quadrant[1] = 1;
			}
		}

		if ((float)my_enemy->vel[0] < 0 && (float)my_enemy->vel[1] <= 0) {
			//std::cout << "In 2nd" << std::endl;
			// Determine if the ship is entering a new quadrant
			if (my_enemy->vel[0] > -0.1 && my_enemy->vel[0] < 0.1) {
				my_enemy->vel[0] = 0;
				my_enemy->vel[1] = -1 * my_enemy->save_vel[1];
				my_enemy->quadrant[1] = 3;
			} else {
				dir_flag = 4;
				my_enemy->quadrant[1] = 2;
			}
		}
		// Change the ships velocity
		if (my_enemy->quadrant[1] == 4) {
			
			if (my_enemy->vel[0] > 0) {
				my_enemy->vel[0] -= .1;
			}
			if (my_enemy->vel[1] < my_enemy->save_vel[1]) {
				my_enemy->vel[1] += .1;
			}
			return 0;
		}
		// Change the ships velocity
		if (my_enemy->quadrant[1] == 3) {
			
			if (my_enemy->vel[1] < 0) {
				my_enemy->vel[1] += .1;
			}

			if (my_enemy->vel[0] < my_enemy->save_vel[0]) {
				my_enemy->vel[0] += .1;
			}
			return 0;

		}
		// Change the ships velocity
		if (my_enemy->quadrant[1] == 2) {
			
			if (my_enemy->vel[0] < 0) {
				my_enemy->vel[0] += .1;
			}
			if (my_enemy->vel[1] > -1 * my_enemy->save_vel[1]) {
				my_enemy->vel[1] -= .1;
			}
			return 0;

		}

		if (my_enemy->quadrant[1] == 1) {
			
			if (my_enemy->vel[1] > 0) {
				my_enemy->vel[1] -= .1;
			}
			if (my_enemy->vel[0] > -1 * my_enemy->save_vel[0]) {
				my_enemy->vel[0] -= .1;
			}
			return 0;
		}

		return dir_flag;

	} else {
		return 0;
	}
	
}

void change_vel(Ship *my_enemy, int num_calls, int ship_quadrant)
{
	int xvel = 0;
	int yvel = 0;
	int pos_x = 0;
	int pos_y = 0;
	if (num_calls == 1) {
		srand(time(NULL));
	}
	
	pos_x = rand() % 2;
	pos_y = rand() % 2;

	if (ship_quadrant > 0 && my_enemy->num_calls_vel == 1) {
		std::cout << "In change V" << std::endl;
		xvel = rand() % 5 + 1;
		yvel = rand() % 5 + 1;
		if (pos_x == 1) {
			my_enemy->vel[0] = xvel;
		} else {
			my_enemy->vel[0] = -1 * xvel;
		}
		
		if (pos_y == 1) {
			my_enemy->vel[1] = yvel;
		} else {
			my_enemy->vel[1] = -1 * yvel;
		}
		my_enemy->num_calls_vel += 1;
	}
}

void det_coll_enemy(int *num_enemies, int num_bullets, Ship *enemies,
	Bullet *barr)
{
	Bullet *b = barr;
	for (int i = 0; i < num_bullets; i++) {
		for (int k = 0; k < *num_enemies; k++) {
			//std::cout << "Num Enemies " << *num_enemies << std::endl;
			if ((((b[i].pos[0]) - (enemies[k].pos[0] - 30)) >= 0) &&
				(((enemies[k].pos[0] + 30)) - b[i].pos[0] >= 0) &&  	
				(((b[i].pos[1]) - (enemies[k].pos[1] - 30)) >= 0) &&
				(((enemies[k].pos[1] + 30)) - b[i].pos[1] >= 0)) {
					
				std::cout << "Collision " <<  std::endl;
				enemies[k].pos[1] = 40000;
				enemies[k].pos[0] = 40000;

			}
		}
	}
}

// Creates a ship that moves from the righ of the screen to the left
void new_ship_rightspawn(Ship *my_enemy, int countr, int yres, int xres) {
	int xpos = xres - 1;
	int v2;
	bool ypos = 0;
	if (countr == 0) {
		srand (time(NULL));
	}
	// set up the new enemies variables
	my_enemy->angle = 90;
	my_enemy->valid_enemy = 1;
	my_enemy->rotated = 1;
	my_enemy->color[0] = my_enemy->color[1] = my_enemy->color[2] = 1.0;
	ypos = (Flt)(rand() % yres);
	my_enemy->pos[0] = xpos;
	my_enemy->numbullets = 0;
	my_enemy->first_call = 0;

	// Set x velocity to a value between 1 and 3
	v2 = rand() % 2 + 1;
	my_enemy->vel[0] = v2;
	
	// set y velocity to 0
	my_enemy->vel[1] = ypos;
	my_enemy->pos[2] = 0.0f;
}

void det_coll_player(int num_bullets, Ship *player,	Bullet *barr)
{
	Bullet *b = barr;
	for (int i = 0; i < num_bullets; i++) {
		if ((((b[i].pos[0]) - (player->pos[0] - 30)) >= 0) &&
				(((player->pos[0] + 30)) - b[i].pos[0] >= 0) &&  	
				(((b[i].pos[1]) - (player->pos[1] - 30)) >= 0) &&
				(((player->pos[1] + 30)) - b[i].pos[1] >= 0)) {
					
			std::cout << "Collision " <<  std::endl;
			// Collision detected
			// Display explosion at position of ship if hp = 0
			// Otherwise decrement hp
		}
	}
}
void creditBox(int yres, int xres, int bot)
{
	int x1 = xres/2 + 5;
	int x2 = xres/2 - 160;
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
	r.left = xres - xres/2; 
	r.center = 0;
	ggprint8b(&r, 16, 0x00000000, "Credits");
}

void hit_point_box(int yres, int xres, int bot)
{
	int x1 = xres/2 + 160;
	int x2 = xres/2 - 160;
	int y1 = yres - 60;
	int y2 = yres - 5;
	int hp_box_width = 80;
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
	int up_cntr = 0;
	int down_cntr = 3;
	for (int i = 3; i >= 0; i--) {
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
			glVertex2f((xres - x1 + 1 + up_cntr * hp_box_width), 
				(yres - y1 - 17));
			glVertex2f((xres - x2 - 1 - down_cntr * hp_box_width),
				(yres - y1 - 17));
			glVertex2f((xres - x2 - 1 - down_cntr * hp_box_width),
				(yres - y2 + 1));
			glVertex2f((xres - x1 + 1 + up_cntr * hp_box_width), 
				(yres - y2 + 1));
		glEnd();
		up_cntr++;
		down_cntr--;
	}

	Rect r;
	r.bot = bot;
	r.bot = 45;
	//r.left = 10;
	r.left = xres - xres/2 - 30; 
	r.center = 0;
	ggprint8b(&r, 16, 0xffffffff, "Hit Points");
}
