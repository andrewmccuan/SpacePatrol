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
        float angle;
        float color[3];
        bool valid_enemy; 
        int numbullets;	
};



#endif
