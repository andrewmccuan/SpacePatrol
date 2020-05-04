#ifndef _ASTEROID_H 
#define _ASTEROID_H

//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt     Matrix[4][4];

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

};

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
};

class Bullet {
public:
    Vec pos;
    Vec vel;
    float color[3];
    struct timespec time;

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


#endif
