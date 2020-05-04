//Here is a change for march 3rd lab (From: Will)
// andrew - change made from branch andrewB
// 		  - 2nd change made from andrewB
//raag - change made from branch b1
// Doney - changed made from branch master_doney_lab7
//program: spacepatrol.cpp
//author:  Gordon Griesel
//Edited by:  Andrew McCuan, Will Sparks, Doney Peters, Raag Patel
//date:    2014 - 2018
//mod spring 2015: added constructors
//This program is a game starting point for a 3350 project.
//
//
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include "image.h"
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
						(c)[1]=(a)[1]-(b)[1]; \
						(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1
const int MAX_BULLETS = 11;
const Flt MINIMUM_ASTEROID_SIZE = 60.0;
const int MAX_ENEMIES = 2;

//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern struct timespec timePause;
extern double physicsCountdown;
extern double timeSpan;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);

//-----------------------------------------------------------------------------


Image:: ~Image() { delete [] data; }
Image::Image(const char *fname) {
	if (fname[0] == '\0')
		return;
	//printf("fname **%s**\n", fname);
	int ppmFlag = 0;
	char name[40];
	strcpy(name, fname);
	int slen = strlen(name);
	char ppmname[80];
	if (strncmp(name+(slen-4), ".ppm", 4) == 0)
		ppmFlag = 1;
	if (ppmFlag) {
		strcpy(ppmname, name);
	} else {
		name[slen-4] = '\0';
		//printf("name **%s**\n", name);
		sprintf(ppmname,"%s.ppm", name);
		//printf("ppmname **%s**\n", ppmname);
		char ts[100];
		//system("convert eball.jpg eball.ppm");
		sprintf(ts, "convert %s %s", fname, ppmname);
		system(ts);
	}
	//sprintf(ts, "%s", name);
	FILE *fpi = fopen(ppmname, "r");
	if (fpi) {
		char line[200];
		fgets(line, 200, fpi);
		fgets(line, 200, fpi);
		//skip comments and blank lines
		while (line[0] == '#' || strlen(line) < 2)
			fgets(line, 200, fpi);
		sscanf(line, "%i %i", &width, &height);
		fgets(line, 200, fpi);
		//get pixel data
		int n = width * height * 3;
		data = new unsigned char[n];
		for (int i=0; i<n; i++)
			data[i] = fgetc(fpi);
		fclose(fpi);
	} else {
		printf("ERROR opening image: %s\n",ppmname);
		exit(0);
	}
	if (!ppmFlag)
		unlink(ppmname);
}
Image doneyImg = "./images/doneyImage.jpg";
Image img[14] = { 
"./images/sp_background_seamless.jpg",
"./images/sp_ship.jpg",
"./images/sp_enemy1.jpg",
"./images/sp_menu.jpg",
"./images/sp_menuGalaxy.jpg",
"./images/sp_play.jpg",
"./images/sp_highscores.jpg",
"./images/sp_credits.jpg",
"./images/sp_logo.jpg",
"./images/sp_exit.jpg",
"./images/sp_credits_page.jpg",
"./images/sp_highscore_page.jpg",
"./images/sp_menu_button.jpg",
"./images/powerup.png"
};

//--- From "background" framework ---
class Texture {
public:
	Image *backImage;
	GLuint backTexture;
	float xc[2];
	float yc[2];
};
//-----------------------------------

class Global {
public:
	int show_menu = 1; 
	int show_credits = 0;
	int show_highscore = 0;
	int credits = 0;
	int collision_det = 0;
	int tgif = 0;
	int highscore = 0;
	int save_score;
	int help = 0;
	int xres, yres;
	int mx, my;
	int closeGame = 0;
	int gamestate[4];
	//int gamestate[4] = {show_menu, highscore, credits, closeGame};
	int *arr;
	char keys[65536];
	GLuint shipTexture;
	GLuint enemy1Texture;
	GLuint creditTexture;
	GLuint menuTexture;
	GLuint menuGalTexture;
	GLuint menuPlay;
	GLuint menuHighscores;
	GLuint menuCredits;
	GLuint menuLogo;
	GLuint menuExit;
	GLuint menuCreditsPage;
	GLuint menuHighscorePage;
	GLuint menuButton;
	GLuint powerUp;
	Texture tex; //From "brackground" framework
	Global() {
		xres = 1250;
		yres = 900;
		memset(keys, 0, 65536);
		//gamestate = {show_menu, highscore, credits, closeGame};
		arr = 0;
	}
} gl;

class Ship {
public:
	Vec dir;
	Vec pos;
	Vec vel;
	Vec save_vel;
	int quadrant[2];
	float angle;
	float color[3];
	bool valid_enemy;
	int numbullets;
	int rotated;
	int first_call;
	int num_calls_vel;
	float hp;
public:
	Ship() {
		VecZero(dir);
		pos[0] = (Flt)(gl.xres/2);
		pos[1] = (Flt)(gl.yres/2);
		pos[2] = 0.0f;
		numbullets = 0;
		VecZero(vel);
		angle = 270.0;
		color[0] = color[1] = color[2] = 1.0;
		valid_enemy = 0;
		quadrant[0] = 0;
		first_call = 0;
		quadrant[1] = 0;
		rotated = 0;
		num_calls_vel = 1;
		hp = 3.0;
	}
};

void new_ship(Ship *my_enemy, int a, int resy, int resx);

class Bullet {
public:
	Vec pos;
	Vec vel;
	float color[3];
	struct timespec time;
public:
	Bullet() { }
};

class Asteroid {
public:
	Vec pos;
	Vec vel;
	int nverts;
	Flt radius;
	Vec vert[8];
	float angle;
	float rotate;
	float color[3];
	struct Asteroid *prev;
	struct Asteroid *next;
public:
	Asteroid() {
		prev = NULL;
		next = NULL;
	}
};

class PowerUp {
public:
	Vec pos;
	Vec vel;
	int nverts;
	Flt radius;
	Vec vert[8];
	float angle;
	float rotate;
	float color[3];
	struct PowerUp *prev;
	struct PowerUp *next;
public:
	PowerUp() {
		prev = NULL;
		next = NULL;
	}
};

//-----------------------------------------------------------------------------
//Function prototypes requiured in the game class.

PowerUp* buildPowerUp(PowerUp*, int, float, float);

//-----------------------------------------------------------------------------

class Game {
public:
	Ship ship;
	Ship enemies[MAX_ENEMIES];
	Asteroid *ahead;
	PowerUp *next;
	Bullet *barr;
	Bullet *ebarr;
	int nships;
	int nasteroids;
	int score;
	int nbullets;
	int ebullets;
	int nenemies;
	int num_calls_vel;
	struct timespec bulletTimer;
	struct timespec ebulletTimer;
	struct timespec mouseThrustTimer;
	bool mouseThrustOn;
public:
	Game() {
		score = 0;
		ahead = NULL;
		next = NULL;
		barr = new Bullet[MAX_BULLETS];
		ebarr = new Bullet[100];
		nasteroids = 0;
		nbullets = 0;
		ebullets = 0;
		nships = 0;
		mouseThrustOn = false;
		num_calls_vel = 0;
		//build 10 asteroids...
		for (int j=0; j<5; j++) {
			Asteroid *a = new Asteroid;
			a->nverts = 8;
			a->radius = rnd()*80.0 + 40.0;
			Flt r2 = a->radius / 2.0;
			Flt angle = 0.0f;
			Flt inc = (PI * 2.0) / (Flt)a->nverts;
			for (int i=0; i<a->nverts; i++) {
				a->vert[i][0] = sin(angle) * (r2 + rnd() * a->radius);
				a->vert[i][1] = cos(angle) * (r2 + rnd() * a->radius);
				angle += inc;
			}
			a->pos[0] = (Flt)(rand() % gl.xres);
			a->pos[1] = (Flt)(rand() % gl.yres);
			a->pos[2] = 0.0f;
			a->angle = 0.0;
			a->rotate = rnd() * 4.0 - 2.0;
			a->color[0] = 0.8;
			a->color[1] = 0.8;
			a->color[2] = 0.7;
			a->vel[0] = (Flt)(rnd()*2.0-1.0);
			a->vel[1] = (Flt)(rnd()*2.0-1.0);
			//std::cout << "asteroid" << std::endl;
			//add to front of linked list
			a->next = ahead;
			if (ahead != NULL)
				ahead->prev = a;
			ahead = a;
			++nasteroids;
		}

		//----------------------------------------------------------------
		//Draw a single PowerUp
		PowerUp *p = new PowerUp;
		next = buildPowerUp(p, gl.yres, rnd(), PI);
		//----------------------------------------------------------------

		clock_gettime(CLOCK_REALTIME, &bulletTimer);
		nenemies = 0;
		for (int k = 0; k < MAX_ENEMIES; k++) {
			new_ship(&enemies[k], k, gl.yres, gl.xres);
			nenemies++;
			std::cout << "Ship position x: " << enemies[k].pos[0] << std::endl;
			std::cout << "Ship position Y: " << enemies[k].pos[1] << std::endl;
		
			//enemies[k].valid_enemy = 0;
		}
	}
	~Game() {
		delete [] barr;
	}
} g;

//X Windows variables
class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
public:
	X11_wrapper() { }
	X11_wrapper(int w, int h) {
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		XSetWindowAttributes swa;
		setup_screen_res(gl.xres, gl.yres);
		dpy = XOpenDisplay(NULL);
		if (dpy == NULL) {
			std::cout << "\n\tcannot connect to X server" << std::endl;
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XWindowAttributes getWinAttr;
		XGetWindowAttributes(dpy, root, &getWinAttr);
		int fullscreen=0;
		gl.xres = w;
		gl.yres = h;
		if (!w && !h) {
			//Go to fullscreen.
			gl.xres = getWinAttr.width;
			gl.yres = getWinAttr.height;
			//When window is fullscreen, there is no client window
			//so keystrokes are linked to the root window.
			XGrabKeyboard(dpy, root, False,
				GrabModeAsync, GrabModeAsync, CurrentTime);
			fullscreen=1;
		}
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if (vi == NULL) {
			std::cout << "\n\tno appropriate visual found\n" << std::endl;
			exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		swa.colormap = cmap;
		swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
			PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |
			StructureNotifyMask | SubstructureNotifyMask;
		unsigned int winops = CWBorderPixel|CWColormap|CWEventMask;
		if (fullscreen) {
			winops |= CWOverrideRedirect;
			swa.override_redirect = True;
		}
		win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
			vi->depth, InputOutput, vi->visual, winops, &swa);
		//win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
		//vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
		set_title();
		glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
		show_mouse_cursor(0);
	}
	~X11_wrapper() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	void set_title() {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "Asteroids template");
	}
	void check_resize(XEvent *e) {
		//The ConfigureNotify is sent by the
		//server if the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		if (xce.width != gl.xres || xce.height != gl.yres) {
			//Window size did change.
			reshape_window(xce.width, xce.height);
		}
	}
	void reshape_window(int width, int height) {
		//window has been resized.
		setup_screen_res(width, height);
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
		set_title();
	}
	void setup_screen_res(const int w, const int h) {
		gl.xres = w;
		gl.yres = h;
	}
	void swapBuffers() {
		glXSwapBuffers(dpy, win);
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void set_mouse_position(int x, int y) {
		XWarpPointer(dpy, None, win, 0, 0, 0, 0, x, y);
	}
	void show_mouse_cursor(const int onoff) {
		if (onoff) {
			//this removes our own blank cursor.
			XUndefineCursor(dpy, win);
			return;
		}
		//vars to make blank cursor
		Pixmap blank;
		XColor dummy;
		char data[1] = {0};
		Cursor cursor;
		//make a blank cursor
		blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
		if (blank == None)
			std::cout << "error: out of memory." << std::endl;
		cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
		XFreePixmap(dpy, blank);
		//this makes you the cursor. then set it using this function
		XDefineCursor(dpy, win, cursor);
		//after you do not need the cursor anymore use this function.
		//it will undo the last change done by XDefineCursor
		//(thus do only use ONCE XDefineCursor and then XUndefineCursor):
	}
} x11(0, 0);


//function prototypes
void setupGame();
void renderPowerUpImage(GLuint, float*);
void updatePowerUpPosition (PowerUp*, int, int);
void renderPowerUps(PowerUp*);
void renderTGIF (int, int);
int circ_mov (Ship *my_enemy);
void raag_text(int, int);
void renderDoneyTextCredits(int, int);
void draw_will_text(int, int);
void andrew_credit_text(int, int);
int andrewButtonAction(int mouseX, int mouseY, int screen, int xres);
void andrewBackImg(GLuint texture, int xres, int yres, float xc[], float yc[]);
void andrewDrawMouse(int mouseX, int mouseY);
void andrewDrawShip(GLuint texture, float* pos);
void andrewDrawEnemy(GLuint texture, float* pos);
void andrewBackImgMove(float* xc);
void andrewHelpMenu(int, int, int);
void andrewHighscoreBox(int, int, int, int* arr);
void andrewShowCredits(GLuint texture1, GLuint texture2, int xres, int yres);
void andrewShowHighscore(GLuint texture1, GLuint texture2, int xres, int yres);
void andrewShowMenu(GLuint texture1, GLuint texture2, int xres, int yres);
void andrewShowButtons(GLuint tex1, GLuint tex2, GLuint tex3, GLuint tex4,
 		GLuint tex5, int xres, int yres);
void andrewButtonHighlight(int mouseX, int mouseY, int screen, int xres);
void renderDoneyImage(GLuint, int, int);
void genAndBindDoneyImage();
void enemy_bullets();
void renderDoneyImage();
void init_opengl(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();
int movement(int);
int destroy_ship(float, float, Asteroid *);
void det_coll(int yres, int xres);
void change_vel(Ship *, int, int);
extern void set_to_non_blocking(const int sock);
extern void show_cert_data(SSL *ssl, BIO *outbio, const char *hostname);
extern BIO *ssl_setup_bio(void);
int * high_score(int score);
void det_coll_enemy(int *num_enemies, int num_bullets, Ship *enemies, Bullet *barr); 
void creditBox(int yres, int xres, int bot);
void hit_point_box(int yres, int xres, int bot, Ship player);
void det_coll_player(int, Ship *, Bullet *);

void closeTheGame()
{
	gl.closeGame = 1;
}

//==========================================================================
// M A I N
//==========================================================================
int main()
{
	logOpen();
	init_opengl();
	setupGame();
	srand(time(NULL));
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	x11.set_mouse_position(100,100);
	int done=0;
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			done = check_keys(&e);
			if (gl.closeGame == 1) {
				done = gl.closeGame;
			}
		}
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		timeCopy(&timeStart, &timeCurrent);
		physicsCountdown += timeSpan;
		while (physicsCountdown >= physicsRate) {
			physics();
			physicsCountdown -= physicsRate;
		}
		enemy_bullets();
		render();
		x11.swapBuffers();
	}
	cleanup_fonts();
	logClose();
	return 0;
}

void setupGame()
{
	gl.gamestate[0] = gl.show_menu;
	gl.gamestate[1] = gl.show_highscore;
	gl.gamestate[2] = gl.show_credits;
	gl.gamestate[3] = gl.closeGame;

	//Come Back to later
	//possible seg fault
	//*
	gl.arr = high_score(g.score);
	std::cout << "Contacted server" << std::endl;
	std::cout << *(gl.arr) << std::endl;
	//*/
}

//===============================================
// Imported from (rainforest framework)
// 
//===============================================
unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		//-----------------------------------------------
		//get largest color component...
		//*(ptr+3) = (unsigned char)((
		//		(int)*(ptr+0) +
		//		(int)*(ptr+1) +
		//		(int)*(ptr+2)) / 3);
		//d = a;
		//if (b >= a && b >= c) d = b;
		//if (c >= a && c >= b) d = c;
		//*(ptr+3) = d;
		//-----------------------------------------------
		//this code optimizes the commented code above.
		*(ptr+3) = (a|b|c);
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, gl.xres, gl.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();

	gl.tex.backImage = &img[0];
	glGenTextures(1, &gl.creditTexture);
	glGenTextures(1, &gl.tex.backTexture);
	glGenTextures(1, &gl.shipTexture);
	glGenTextures(1, &gl.enemy1Texture);
	glGenTextures(1, &gl.menuTexture);
	glGenTextures(1, &gl.menuLogo);
	glGenTextures(1, &gl.menuGalTexture);
	glGenTextures(1, &gl.menuPlay);
	glGenTextures(1, &gl.menuHighscores);
	glGenTextures(1, &gl.menuCredits);
	glGenTextures(1, &gl.menuExit);
	glGenTextures(1, &gl.menuCreditsPage);
	glGenTextures(1, &gl.menuHighscorePage);
	glGenTextures(1, &gl.menuButton);
	glGenTextures(1, &gl.powerUp);
	

	// Render Doney's Image
	//------------------------------------------------------------------
	//Code borrowed from Gordon Greisel.
	int w = doneyImg.width;
	int h = doneyImg.height;

	glBindTexture(GL_TEXTURE_2D, gl.creditTexture);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, doneyImg.data);
    //------------------------------------------------------------------
	// Background Image
	//--- From "background" framework ---
	int w0 = gl.tex.backImage->width;
	int h0 = gl.tex.backImage->height;
	glBindTexture(GL_TEXTURE_2D, gl.tex.backTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w0, h0, 0,
							GL_RGB, GL_UNSIGNED_BYTE, img[0].data);
	gl.tex.xc[0] = 0.0;
	gl.tex.xc[1] = 0.25;
	gl.tex.yc[0] = 0.0;
	gl.tex.yc[1] = 1.0;
	//------------------------------------------------------------------
	// Ship Image
	// --- From "rainforest" framework ---
	int w1 = img[1].width;
	int h1 = img[1].height;
	glBindTexture(GL_TEXTURE_2D, gl.shipTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *silhouetteData = buildAlphaData(&img[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w1, h1, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//------------------------------------------------------------------
	// Enemy Ship Image
	// --- From "rainforest" framework ---
	int w2 = img[2].width;
	int h2 = img[2].height;
	glBindTexture(GL_TEXTURE_2D, gl.enemy1Texture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	silhouetteData = buildAlphaData(&img[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w2, h2, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//------------------------------------------------------------------
	// Menu Image
	//*
	int w3 = img[3].width;
	int h3 = img[3].height;
	glBindTexture(GL_TEXTURE_2D, gl.menuTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	silhouetteData = buildAlphaData(&img[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w3, h3, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);

	//
	//*/
	//------------------------------------------------------------------
	// Spinning Galaxy Image
	// --- From "rainforest" framework ---
	//*
	int w4 = img[4].width;
	int h4 = img[4].height;
	glBindTexture(GL_TEXTURE_2D, gl.menuGalTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	silhouetteData = buildAlphaData(&img[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w4, h4, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//*/
	//------------------------------------------------------------------
	// Play Button Image
	//*
	int w5 = img[5].width;
	int h5 = img[5].height;
	glBindTexture(GL_TEXTURE_2D, gl.menuPlay);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	silhouetteData = buildAlphaData(&img[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w5, h5, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//*/
	//------------------------------------------------------------------
	// High Score Button Image
	//*
	int w6 = img[6].width;
	int h6 = img[6].height;
	glBindTexture(GL_TEXTURE_2D, gl.menuHighscores);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	silhouetteData = buildAlphaData(&img[6]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w6, h6, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//*/
	//------------------------------------------------------------------
	// Credits Button Image
	//*
	int w7 = img[7].width;
	int h7 = img[7].height;
	glBindTexture(GL_TEXTURE_2D, gl.menuCredits);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	silhouetteData = buildAlphaData(&img[7]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w7, h7, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//*/
	//*/
	//------------------------------------------------------------------
	// Logo Image
	//*
	int w8 = img[8].width;
	int h8 = img[8].height;
	glBindTexture(GL_TEXTURE_2D, gl.menuLogo);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	silhouetteData = buildAlphaData(&img[8]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w8, h8, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//*/
	//------------------------------------------------------------------
	// Exit Button Image
	//*
	int w9 = img[9].width;
	int h9 = img[9].height;
	glBindTexture(GL_TEXTURE_2D, gl.menuExit);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	silhouetteData = buildAlphaData(&img[9]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w9, h9, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//*/
	//------------------------------------------------------------------
	// Credits Page Image
	int w10 = img[10].width;
	int h10 = img[10].height;
	glBindTexture(GL_TEXTURE_2D, gl.menuCreditsPage);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	silhouetteData = buildAlphaData(&img[10]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w10, h10, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//------------------------------------------------------------------
	// Highscore Page Image
	int w11 = img[11].width;
	int h11 = img[11].height;
	glBindTexture(GL_TEXTURE_2D, gl.menuHighscorePage);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	silhouetteData = buildAlphaData(&img[11]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w11, h11, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//------------------------------------------------------------------
	// Back Button Image
	int w12 = img[12].width;
	int h12 = img[12].height;
	glBindTexture(GL_TEXTURE_2D, gl.menuButton);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	silhouetteData = buildAlphaData(&img[12]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w12, h12, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//----
	//------------------------------------------------------------------
	// Power Up Image
	int w13 = img[13].width;
	int h13 = img[13].height;
	glBindTexture(GL_TEXTURE_2D, gl.powerUp);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	silhouetteData = buildAlphaData(&img[13]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w13, h13, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
	//----
}

void normalize2d(Vec v)
{
	Flt len = v[0]*v[0] + v[1]*v[1];
	if (len == 0.0f) {
		v[0] = 1.0;
		v[1] = 0.0;
		return;
	}
	len = 1.0f / sqrt(len);
	v[0] *= len;
	v[1] *= len;
}

void check_mouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = gl.xres/2;
	static int savey = gl.yres/2;
	gl.mx = savex;
	gl.my = savey;
	//
	//static int ct=0;

	gl.mx = e->xbutton.x;
	gl.my = e->xbutton.y;

	//std::cout << "m" << std::endl << std::flush;
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {

			if (gl.show_menu == 1) {
				int screen = 1;
				int num = andrewButtonAction(gl.mx, (gl.yres - gl.my), screen,
							gl.xres);
				if (num == 1) {
					gl.show_menu = 0;
					gl.gamestate[0] = gl.show_menu;
				}
				if (num == 2) {
					gl.show_menu = 0;
					gl.gamestate[0] = gl.show_menu;
					gl.show_highscore = 1;
					gl.gamestate[1] = gl.show_highscore;
				}
				if (num == 3) {
					gl.show_menu = 0;
					gl.gamestate[0] = gl.show_menu;
					gl.show_credits = 1;
					gl.gamestate[2] = gl.show_credits;
				}
				if (num == 4) {
					gl.show_menu = 0;
					gl.gamestate[0] = gl.show_menu;
					gl.closeGame = 1;
					gl.gamestate[3] = gl.closeGame;
				}
			}
			//*
			if (gl.show_highscore == 1) {
				int screen = 2;
				int num = andrewButtonAction(gl.mx, (gl.yres - gl.my), screen,
							gl.xres);
				if (num == 1) {
					gl.show_menu = 1;
					gl.gamestate[0] = gl.show_menu;
					gl.show_highscore = 0;
					gl.gamestate[1] = gl.show_highscore;
				}
			}
			if (gl.show_credits == 1) {
				int screen = 2;
				int num = andrewButtonAction(gl.mx, (gl.yres - gl.my), screen,
							gl.xres);
				if (num == 1) {
					gl.show_menu = 1;
					gl.gamestate[0] = gl.show_menu;
					gl.show_credits = 0;
					gl.gamestate[1] = gl.show_credits;
				}
			}
			//*/
			//Left button is down
			//a little time between each bullet
			struct timespec bt;
			clock_gettime(CLOCK_REALTIME, &bt);
			double ts = timeDiff(&g.bulletTimer, &bt);
			if (ts > 0.1) {
				timeCopy(&g.bulletTimer, &bt);
				//shoot a bullet...
				if (g.nbullets < MAX_BULLETS) {
					Bullet *b = &g.barr[g.nbullets];
					timeCopy(&b->time, &bt);
					b->pos[0] = g.ship.pos[0];
					b->pos[1] = g.ship.pos[1];
					b->vel[0] = g.ship.vel[0];
					b->vel[1] = g.ship.vel[1];
					//convert ship angle to radians
					Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
					//convert angle to a vector
					Flt xdir = cos(rad);
					Flt ydir = sin(rad);
					b->pos[0] += xdir*20.0f;
					b->pos[1] += ydir*20.0f;
					b->vel[0] += xdir*6.0f + rnd()*0.1;
					b->vel[1] += ydir*6.0f + rnd()*0.1;
					b->color[0] = 1.0f;
					b->color[1] = 1.0f;
					b->color[2] = 1.0f;
					++g.nbullets;
					++g.score;
				}
			}
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	//keys[XK_Up] = 0;
	/*
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		int xdiff = savex - e->xbutton.x;
		int ydiff = savey - e->xbutton.y;
		//gl.mx = xdiff;
		//gl.my = ydiff;
		if (++ct < 10)
			return;		
		//std::cout << "savex: " << savex << std::endl << std::flush;
		//std::cout << "e->xbutton.x: " << e->xbutton.x << std::endl <<
		//std::flush;
		if (xdiff > 0) {
			//std::cout << "xdiff: " << xdiff << std::endl << std::flush;
			//g.ship.angle += 0.05f * (float)xdiff;
			if (g.ship.angle >= 360.0f)
				g.ship.angle -= 360.0f;
		}
		else if (xdiff < 0) {
			//std::cout << "xdiff: " << xdiff << std::endl << std::flush;
			//g.ship.angle += 0.05f * (float)xdiff;
			if (g.ship.angle < 0.0f)
				g.ship.angle += 360.0f;
		}
		if (ydiff > 0) {
			//apply thrust
			//convert ship angle to radians
			Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			g.ship.vel[0] += xdir * (float)ydiff * 0.01f;
			g.ship.vel[1] += ydir * (float)ydiff * 0.01f;
			Flt speed = sqrt(g.ship.vel[0]*g.ship.vel[0]+
					g.ship.vel[1]*g.ship.vel[1]);
			if (speed > 10.0f) {
				speed = 10.0f;
				normalize2d(g.ship.vel);
				g.ship.vel[0] *= speed;
				g.ship.vel[1] *= speed;
			}
			g.mouseThrustOn = true;
			clock_gettime(CLOCK_REALTIME, &g.mouseThrustTimer);
		}
		if (gl.show_menu == 0) {
			x11.set_mouse_position(100,100);
			savex=100;
			savey=100;
		}
	}
	*/
}

int check_keys(XEvent *e)
{
	//keyboard input?
	static int shift=0;
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	//Log("key: %i\n", key);
	if (e->type == KeyRelease) {
		gl.keys[key]=0;
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		return 0;
	}
	if (e->type == KeyPress) {
		//std::cout << "press" << std::endl;
		gl.keys[key]=1;
		if (key == XK_Shift_L || key == XK_Shift_R) {
			shift=1;
			return 0;
		}
	} else {
		return 0;
	}
	if (shift){}
	switch (key) {
		case XK_Escape:
			return 1;
		case XK_f:
			break;
		case XK_s:
			break;
		case XK_Down:
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;
		case XK_c:
			gl.credits = gl.credits ^ 1;
			break;
		case XK_d:
			gl.tgif = gl.tgif ^ 1;
			break;
		case XK_h:
			gl.help = gl.help ^ 1;
			break;
		case XK_Tab:
			gl.highscore = gl.highscore ^ 1;
			break;
		case XK_p:
			gl.save_score = gl.save_score ^ 1;
			break;
		case XK_m:
			gl.show_menu = gl.show_menu ^ 1;
			gl.gamestate[0] = gl.show_menu;
	}
	return 0;
}

void deleteAsteroid(Game *g, Asteroid *node)
{
	//Remove a node from doubly-linked list
	//Must look at 4 special cases below.
	if (node->prev == NULL) {
		if (node->next == NULL) {
			//only 1 item in list.
			g->ahead = NULL;
		} else {
			//at beginning of list.
			node->next->prev = NULL;
			g->ahead = node->next;
		}

	} else {
		if (node->next == NULL) {
			//at end of list.
			node->prev->next = NULL;
		} else {
			//in middle of list.
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
	}
	delete node;
	node = NULL;
}

void buildAsteroidFragment(Asteroid *ta, Asteroid *a)
{
	//build ta from a
	ta->nverts = 8;
	ta->radius = a->radius / 2.0;
	Flt r2 = ta->radius / 2.0;
	Flt angle = 0.0f;
	Flt inc = (PI * 2.0) / (Flt)ta->nverts;
	for (int i=0; i<ta->nverts; i++) {
		ta->vert[i][0] = sin(angle) * (r2 + rnd() * ta->radius);
		ta->vert[i][1] = cos(angle) * (r2 + rnd() * ta->radius);
		angle += inc;
	}
	ta->pos[0] = a->pos[0] + rnd()*10.0-5.0;
	ta->pos[1] = a->pos[1] + rnd()*10.0-5.0;
	ta->pos[2] = 0.0f;
	ta->angle = 0.0;
	ta->rotate = a->rotate + (rnd() * 4.0 - 2.0);
	ta->color[0] = 0.8;
	ta->color[1] = 0.8;
	ta->color[2] = 0.7;
	ta->vel[0] = a->vel[0] + (rnd()*2.0-1.0);
	ta->vel[1] = a->vel[1] + (rnd()*2.0-1.0);
	//std::cout << "frag" << std::endl;
}

void physics()
{
	//std::cout << "Num enemies" << g.nenemies <<std::endl;
	//Moves the background
	andrewBackImgMove(gl.tex.xc);

	Flt d0,d1,dist;
	//Update ship position
	g.ship.pos[0] += g.ship.vel[0];
	g.ship.pos[1] += g.ship.vel[1];
	
	for (int k = g.nenemies; k < MAX_ENEMIES; k++) {
		g.nenemies++;
		new_ship(&g.enemies[k], k, gl.yres, gl.xres);
		std::cout << "Ship position x: " << g.enemies[k].pos[0] << std::endl;
		std::cout << "Ship position y: " << g.enemies[k].pos[1] << std::endl;
		
		//enemies[k].valid_enemy = 0;
	}

	//
	//
	//Update bullet positions
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	int i=0;
	while (i < g.nbullets) {
		Bullet *b = &g.barr[i];
		//How long has bullet been alive?
		double ts = timeDiff(&b->time, &bt);
		if (ts > 2.5) {
			//time to delete the bullet.
			memcpy(&g.barr[i], &g.barr[g.nbullets-1],
				sizeof(Bullet));
			g.nbullets--;
			//do not increment i.
			continue;
		}
		//move the bullet
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];
		//Check for collision with window edges
		if (b->pos[0] < 0.0) {
			b->pos[0] += (float)gl.xres;
		}
		else if (b->pos[0] > (float)gl.xres) {
			b->pos[0] -= (float)gl.xres;
		}
		else if (b->pos[1] < 0.0) {
			b->pos[1] += (float)gl.yres;
		}
		else if (b->pos[1] > (float)gl.yres) {
			b->pos[1] -= (float)gl.yres;
		}
		i++;
	}
	i = 0;
	while (i < g.ebullets) {
		Bullet *b = &g.ebarr[i];
		//How long has bullet been alive?
		//double ts = timeDiff(&b->time, &bt);

		//move the bullet
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];
		//Check for collision with window edges
		if (b->pos[0] < 0.0) {
			b->pos[0] += (float)gl.xres;
			memcpy(&g.ebarr[i], &g.ebarr[g.ebullets-1],
				sizeof(Bullet));
			g.ebullets--;
		}
		else if (b->pos[0] > (float)gl.xres) {
			b->pos[0] -= (float)gl.xres;
			memcpy(&g.ebarr[i], &g.ebarr[g.ebullets-1],
				sizeof(Bullet));
			g.ebullets--;		
		}
		else if (b->pos[1] < 0.0) {
			b->pos[1] += (float)gl.yres;
			memcpy(&g.ebarr[i], &g.ebarr[g.ebullets-1],
				sizeof(Bullet));
			g.ebullets--;			
		}
		else if (b->pos[1] > (float)gl.yres) {
			b->pos[1] -= (float)gl.yres;
			memcpy(&g.ebarr[i], &g.ebarr[g.ebullets-1],
				sizeof(Bullet));
			g.ebullets--;				
		}
		i++;
	}
	//
	//Update powerup positions
	updatePowerUpPosition (g.next, gl.yres, gl.xres);


	//
	//Update asteroid positions
	Asteroid *a = g.ahead;
	while (a) {
		a->pos[0] += a->vel[0];
		a->pos[1] += a->vel[1];
		//Check for collision with window edges
		if (a->pos[0] < -100.0) {
			a->pos[0] += (float)gl.xres+200;
		}
		else if (a->pos[0] > (float)gl.xres+100) {
			a->pos[0] -= (float)gl.xres+200;
		}
		else if (a->pos[1] < -100.0) {
			a->pos[1] += (float)gl.yres+200;
		}
		else if (a->pos[1] > (float)gl.yres+100) {
			a->pos[1] -= (float)gl.yres+200;
		}
		a->angle += a->rotate;
		a = a->next;
	}
	//
	//Asteroid collision with bullets?
	//If collision detected:
	//     1. delete the bullet
	//     2. break the asteroid into pieces
	//        if asteroid small, delete it
	a = g.ahead;
	while (a) {
		//is there a bullet within its radius?
		int i=0;
		while (i < g.nbullets) {
			Bullet *b = &g.barr[i];
			d0 = b->pos[0] - a->pos[0];
			d1 = b->pos[1] - a->pos[1];
			dist = (d0*d0 + d1*d1);
			if (dist < (a->radius*a->radius)) {
				//std::cout << "asteroid hit." << std::endl;
				//this asteroid is hit.
				if (a->radius > MINIMUM_ASTEROID_SIZE) {
					//break it into pieces.
					Asteroid *ta = a;
					buildAsteroidFragment(ta, a);
					int r = rand()%10+5;
					for (int k=0; k<r; k++) {
						//get the next asteroid position in the array
						Asteroid *ta = new Asteroid;
						buildAsteroidFragment(ta, a);
						//add to front of asteroid linked list
						ta->next = g.ahead;
						if (g.ahead != NULL)
							g.ahead->prev = ta;
						g.ahead = ta;
						g.nasteroids++;
					}
				} else {
					a->color[0] = 1.0;
					a->color[1] = 0.1;
					a->color[2] = 0.1;
					//asteroid is too small to break up
					//delete the asteroid and bullet
					Asteroid *savea = a->next;
					deleteAsteroid(&g, a);
					a = savea;
					g.nasteroids--;
				}
				//delete the bullet...
				memcpy(&g.barr[i], &g.barr[g.nbullets-1], sizeof(Bullet));
				g.nbullets--;
				if (a == NULL)
					break;
			}
			i++;
		}
		if (a == NULL)
			break;
		a = a->next;
	}
	//---------------------------------------------------
	//check keys pressed now
	
	//check collision of ship and objects
	a = g.ahead;
	if (destroy_ship(g.ship.pos[0], g.ship.pos[1], a)) {
		gl.collision_det = 1;
	} else {
		gl.collision_det = 0;
	}
	
	if (gl.keys[XK_space]) {
		//a little time between each bullet
		struct timespec bt;
		clock_gettime(CLOCK_REALTIME, &bt);
		double ts = timeDiff(&g.bulletTimer, &bt);
		if (ts > 0.1) {
			timeCopy(&g.bulletTimer, &bt);
			if (g.nbullets < MAX_BULLETS) {
				//shoot a bullet...
				//Bullet *b = new Bullet;
				Bullet *b = &g.barr[g.nbullets];
				timeCopy(&b->time, &bt);
				b->pos[0] = g.ship.pos[0];
				b->pos[1] = g.ship.pos[1];
				b->vel[0] = g.ship.vel[0];
				b->vel[1] = .25 * g.ship.vel[1];
				//convert ship angle to radians
				Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
				//convert angle to a vector
				Flt xdir = cos(rad);
				Flt ydir = sin(rad);
				b->pos[0] += xdir*20.0f;
				b->pos[1] += ydir*20.0f;
				b->vel[0] += xdir*6.0f + rnd()*0.1;
				b->vel[1] += ydir*6.0f + rnd()*0.1;
				b->color[0] = 1.0f;
				b->color[1] = 1.0f;
				b->color[2] = 1.0f;
				g.nbullets++;
				++g.score;
			}
		}
	}


	if (gl.keys[XK_Left]) {
		g.ship.vel[0] = movement(0);
	}

	if (!gl.keys[XK_Left] && !gl.keys[XK_Right]) {
		g.ship.vel[0] = 0;
	}

	if (gl.keys[XK_Right]) {
		g.ship.vel[0] = movement(1);    
	}
	
	if (!gl.keys[XK_Up] && !gl.keys[XK_Down]) {
		g.ship.vel[1] = 0;
	}

	if (gl.keys[XK_Up]) {
		g.ship.vel[1] = movement(1);
	}

	if (gl.keys[XK_Down]) {
		g.ship.vel[1] = movement(0);
	}

	if (gl.keys[XK_Down] && gl.keys[XK_Up]) {
		g.ship.vel[1] = 0;
	}

	if (gl.keys[XK_Left] && gl.keys[XK_Right]) {
		g.ship.vel[0] = 0;
	}

	if (g.mouseThrustOn) {
		//should thrust be turned off
		struct timespec mtt;
		clock_gettime(CLOCK_REALTIME, &mtt);
		double tdif = timeDiff(&mtt, &g.mouseThrustTimer);
		//std::cout << "tdif: " << tdif << std::endl;
		if (tdif < -0.3)
			g.mouseThrustOn = false;
	}

	int quadrant = 0;
	det_coll_enemy(&g.nenemies, g.nbullets, g.enemies, g.barr);
	det_coll_player(g.ebullets, &g.ship, g.ebarr);  	
	for (int k = 0; k < g.nenemies; k++) {
		//std::cout << "Enemy " << k  << " Pos: " << g.enemies[k].pos[0] << " " << g.enemies[k].pos[1] << std::endl;
		g.enemies[k].pos[0] += g.enemies[k].vel[0];
		g.enemies[k].pos[1] += g.enemies[k].vel[1];
		//std::cout << "Y pos: " << g.enemies[k].pos[1] << std::endl;	
		if (g.enemies[k].pos[1] > gl.yres / 3 && g.enemies[k].pos[1] < 
			(gl.yres - gl.yres/3)) {
			//std::cout << "Now rotated: " << g.enemies[k].pos[1] << std::endl;
			g.enemies[k].rotated = 1;
		}

		if (g.enemies[k].rotated == 1) {
			g.enemies[k].first_call += 1;
			quadrant = circ_mov(&g.enemies[k]);
		}

		if (g.enemies[k].pos[0] < 0.0) {
			g.enemies[k].num_calls_vel = 1;
			g.enemies[k].valid_enemy = 0;
			g.enemies[k].rotated = 0;
			g.enemies[k].vel[0] = 0;
			g.enemies[k].vel[1] = 0;
			memcpy(&g.enemies[k], &g.enemies[g.nenemies-1], sizeof(Ship));
				
			g.nenemies--;
		}
		
		else if (g.enemies[k].pos[0] > (float)gl.xres) {
			g.enemies[k].num_calls_vel = 1;
			g.enemies[k].valid_enemy = 0;
			g.enemies[k].rotated = 0;
			g.enemies[k].vel[0] = 0;
			g.enemies[k].vel[1] = 0;
			memcpy(&g.enemies[k], &g.enemies[g.nenemies-1], sizeof(Ship));	
			g.nenemies--;
		}

		else if (g.enemies[k].pos[1] < 0) {
			g.enemies[k].valid_enemy = 0;
			g.enemies[k].num_calls_vel = 1;
			g.enemies[k].vel[0] = 0;
			g.enemies[k].vel[1] = 0;
			g.enemies[k].rotated = 0;
			memcpy(&g.enemies[k], &g.enemies[g.nenemies-1], sizeof(Ship));		
			g.nenemies--;

		}
				
		else if (g.enemies[k].pos[1] > ((float)gl.yres)) {
			std::cout << g.enemies[k].pos[1] << std::endl;
			g.enemies[k].valid_enemy = 0;
			g.enemies[k].num_calls_vel = 1;
			g.enemies[k].rotated = 0;
			g.enemies[k].vel[0] = 0;
			g.enemies[k].vel[1] = 0;		
			memcpy(&g.enemies[k], &g.enemies[g.nenemies-1], sizeof(Ship));
			g.nenemies--;
		}
		//std::cout << "Quadrant " << quadrant << std::endl;
		g.num_calls_vel += 1;
		change_vel(&g.enemies[k], g.num_calls_vel, quadrant);
	}
}

void enemy_bullets() 
{

	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&g.ebulletTimer, &bt);
	
	if (ts > 0.4) {
		timeCopy(&g.ebulletTimer, &bt);
		for (int i = 0; i<g.nenemies; i++) {
		if (g.ebullets < 100 && g.enemies[i].numbullets < 50 && 
			g.enemies[i].valid_enemy) {

			g.enemies[i].numbullets += 1;
			Bullet *b = &g.ebarr[g.ebullets];
			timeCopy(&b->time, &bt);
			b->pos[0] = g.enemies[i].pos[0];
			b->pos[1] = g.enemies[i].pos[1];
			b->vel[0] = g.enemies[i].vel[0];
			b->vel[1] = g.enemies[i].vel[1];
			//convert ship angle to radians
			Flt rad = ((g.enemies[i].angle+90.0) / 360.0f) * PI * 2.0;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			b->pos[0] += xdir*20.0f;
			b->pos[1] += ydir*20.0f;
			b->vel[0] += xdir*6.0f + rnd()*0.1;
			b->vel[1] += ydir*6.0f + rnd()*0.1;
			b->color[0] = 1.0f;
			b->color[1] = 1.0f;
			b->color[2] = 1.0f;
			++g.ebullets;
		}
		}
	}

}

void render()
{	

	glClear(GL_COLOR_BUFFER_BIT);

	//------------------------------------------------------------------------
	//Background Texture
	andrewBackImg(gl.tex.backTexture, gl.xres, gl.yres, gl.tex.xc, gl.tex.yc);

	Rect r;
	//int ytrack;
	r.bot = gl.yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, "Space Patrol");
	ggprint8b(&r, 16, 0x00ffff00, "n bullets: %i", g.nbullets);
	ggprint8b(&r, 16, 0x00ffff00, "n asteroids: %i", g.nasteroids);

	//Revision needed
	//-----------------------------------------------------------------------
	ggprint8b(&r, 16, 0x00ff0000, "------- Best 4----------");
	ggprint8b(&r, 16, 0x00ffff00, " Score   %i", g.score);
	ggprint8b(&r, 16, 0x00ffff00, " Name ");
	ggprint8b(&r, 16, 0x00ffff00, " Game Over ");
	ggprint8b(&r, 16, 0x00ffff00, " Your Score %i ", g.score);
    	ggprint8b(&r, 16, 0x00ffff00, " Best Score ");
    	ggprint8b(&r, 16, 0x00ffff00, " Tap to restart ");
	//-----------------------------------------------------------------------

	if (gl.help == 1) {
		andrewHelpMenu(gl.yres, gl.xres, r.bot);
	}
	else {
		//r.bot = gl.yres - 20;
		//r.left = gl.xres - 175;
		ggprint8b(&r, 16, 0x00ff4500, "[H] Help Menu");
	}
	//if (gl.show_menu == 1) {
	//	ggprint8b(&r, 16, 0x00ffff00, " MENU SHOULD BEEN SHOWN ");
	//}

	hit_point_box(gl.yres, gl.xres, r.bot, g.ship);	
	if (gl.credits == 1) {
		//renderDoneyImage(gl.creditTexture, gl.yres, gl.xres);
		creditBox(gl.yres, gl.xres, r.bot);
		andrew_credit_text(gl.yres, gl.xres);
		renderDoneyTextCredits(gl.yres, gl.xres);
		draw_will_text(gl.yres, gl.xres);
		raag_text(gl.yres, gl.xres);
		//creditBox(gl.yres, gl.xres, r.bot);
	}
	if (gl.highscore == 1) {
		if (!gl.arr) {
			int arrTest[] = {1, 2, 3, 4, 5};
			andrewHighscoreBox(gl.yres, gl.xres, g.score, arrTest);
		} else {
			andrewHighscoreBox(gl.yres, gl.xres, g.score, gl.arr);
		}
	}

	if (gl.save_score == 1) {
		gl.arr = high_score(g.score);
		gl.save_score = gl.save_score ^ 1;
		std::cout << "Contacted server" << std::endl;
		std::cout << *(gl.arr) << std::endl;
		//andrewHighscoreBox(gl.yres, gl.xres, g.score, *(arr));
	}

	if (gl.collision_det == 1) {
		det_coll(gl.yres, gl.xres);
	}
    
	if (gl.tgif == 1) {
		renderTGIF(gl.yres, gl.xres);
		high_score(g.score);
	}

	//-------------------------------------------------------------------------
	//Draw the ship
	glColor3fv(g.ship.color);
	glPushMatrix();
	glTranslatef(g.ship.pos[0], g.ship.pos[1], g.ship.pos[2]);
	//float angle = atan2(ship.dir[1], ship.dir[0]);
	glRotatef(g.ship.angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	//glVertex2f(-10.0f, -10.0f);
	//glVertex2f(  0.0f, 20.0f);
	//glVertex2f( 10.0f, -10.0f);
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
	//Draw the ship texture
	andrewDrawShip(gl.shipTexture, g.ship.pos);

	for (int k = 0; k < g.nenemies; k++) {
		if (g.enemies[k].valid_enemy == 1) {
			//std::cout << "Enemy " << k  << " Pos: " << g.enemies[k].pos[0] << " " << g.enemies[k].pos[1] << std::endl;
			glColor3fv(g.enemies[k].color);
			glPushMatrix();
			glTranslatef(g.enemies[k].pos[0], g.enemies[k].pos[1],
				g.enemies[k].pos[2]);
			//float angle = atan2(ship.dir[1], ship.dir[0]);
			glRotatef(g.enemies[k].angle, 0.0f, 0.0f, 1.0f);
			glBegin(GL_TRIANGLES);
			//glVertex2f(-10.0f, -10.0f);
			//glVertex2f(  0.0f, 20.0f);
			//glVertex2f( 10.0f, -10.0f);
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
			andrewDrawEnemy(gl.enemy1Texture, g.enemies[k].pos);
		}
	}

	if (gl.keys[XK_Right] || g.mouseThrustOn) {
		int i;
		//draw thrust
		Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
		//convert angle to a vector
		Flt xdir = cos(rad);
		Flt ydir = sin(rad);
		Flt xs,ys,xe,ye,r;
		glBegin(GL_LINES);
		for (i=0; i<16; i++) {
			xs = -xdir * 11.0f + rnd() * 4.0 - 2.0;
			ys = -ydir * 11.0f + rnd() * 4.0 - 2.0;
			r = rnd()*40.0+40.0;
			xe = -xdir * r + rnd() * 18.0 - 9.0;
			ye = -ydir * r + rnd() * 18.0 - 9.0;
			glColor3f(rnd()*.3+.7, rnd()*.3+.7, 0);
			glVertex2f(g.ship.pos[0]+xs,g.ship.pos[1]+ys);
			glVertex2f(g.ship.pos[0]+xe,g.ship.pos[1]+ye);
		}
		glEnd();
	}
	//-------------------------------------------------------------------------
	//Draw the asteroids
	{
		Asteroid *a = g.ahead;
		while (a) {
			//Log("draw asteroid...\n");
			glColor3fv(a->color);
			glPushMatrix();
			glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
			glBegin(GL_LINE_LOOP);
			//Log("%i verts\n",a->nverts);
			for (int j=0; j<a->nverts; j++) {
				glVertex2f(a->vert[j][0], a->vert[j][1]);
			}
			glEnd();
			//glBegin(GL_LINES);
			//	glVertex2f(0,   0);
			//	glVertex2f(a->radius, 0);
			//glEnd();
			glPopMatrix();
			glColor3f(0.0f, 0.0f, 1.0f);
			glBegin(GL_POINTS);
			glVertex2f(a->pos[0], a->pos[1]);
			glEnd();
			a = a->next;
		}
	}
	//-------------------------------------------------------------------------
	// Draw the PowerUps
	renderPowerUps(g.next);
	renderPowerUpImage(gl.powerUp, (float*)g.next->pos);
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	//Draw the bullets
	for (int i=0; i<g.nbullets; i++) {
		Bullet *b = &g.barr[i];
		//Log("draw bullet...\n");
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_POINTS);
		glVertex2f(b->pos[0],      b->pos[1]);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]);
		glVertex2f(b->pos[0],      b->pos[1]-1.0f);
		glVertex2f(b->pos[0],      b->pos[1]+1.0f);
		glColor3f(0.8, 0.8, 0.8);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
		glEnd();
	}

	//Draw the bullets
	for (int i=0; i<g.ebullets; i++) {
		Bullet *b = &g.ebarr[i];
		//std::cout << "Drawing enemy bullet" << std::endl;
		//Log("draw bullet...\n");
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_POINTS);
		glVertex2f(b->pos[0],      b->pos[1]);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]);
		glVertex2f(b->pos[0],      b->pos[1]-1.0f);
		glVertex2f(b->pos[0],      b->pos[1]+1.0f);
		glColor3f(0.8, 0.8, 0.8);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
		glEnd();
	}

	if (gl.show_menu == 1) {
		int screen = 1;
		andrewShowMenu(gl.menuTexture, gl.menuGalTexture, gl.xres, gl.yres);
		andrewShowButtons(gl.menuLogo, gl.menuPlay, gl.menuHighscores,
			gl.menuCredits, gl.menuExit, gl.xres, gl.yres);
		andrewButtonHighlight(gl.mx, (gl.yres - gl.my), screen, gl.xres);
		andrewDrawMouse(gl.mx, (gl.yres - gl.my));
	}
	if (gl.show_credits == 1) {
		int screen = 2;
		andrewShowCredits(gl.menuCreditsPage, gl.menuButton, gl.xres, gl.yres);
		andrewButtonHighlight(gl.mx, (gl.yres - gl.my), screen, gl.xres);
		andrewDrawMouse(gl.mx, (gl.yres - gl.my));
	}
	if (gl.show_highscore == 1) {
		int screen = 2;
		andrewShowHighscore(gl.menuHighscorePage, gl.menuButton, gl.xres, gl.yres);
		andrewButtonHighlight(gl.mx, (gl.yres - gl.my), screen, gl.xres);
		andrewDrawMouse(gl.mx, (gl.yres - gl.my));
		if (!gl.arr) {
			int arrTest[] = {1, 2, 3, 4, 5};
			andrewHighscoreBox(gl.yres, gl.xres, g.score, arrTest);
		} else {
			andrewHighscoreBox(gl.yres, gl.xres, g.score, gl.arr);
		}
	}

}
