#include "ofMain.h"
#include "Defines.h"

#ifndef emptyExample_Boid_h
#define emptyExample_Boid_h

class Boid{
public:
	Boid(ofVec2f l, float ms, float mf);
	~Boid(void);

	static ofImage jelly[JELLY_FRAMES];

	bool bIsJellyfish;
	bool spawnJellys;
	unsigned short int animPos;

	ofVec2f loc, vel, acc;
	float radius;
	float maxForce;
	float maxSpeed;
	ofImage* img;
	ofImage* images;
	static ENV_STATE zone; 
	bool pause;

	void run(vector<Boid>boids);
	void flock(vector<Boid>boids);
	void update();
	void init();
	void seek(ofVec2f target);
	void arrive(ofVec2f target);

	ofVec2f steer(ofVec2f target, bool slowdown);

	void render();
	void borders();

	float heading2D(ofVec2f v);

	ofVec2f separate(vector<Boid>boids);
	ofVec2f align(vector<Boid>boids);
	ofVec2f cohesion(vector<Boid>boids);

};

#endif

