#include "ofMain.h"
#include "Boid.h"
#include "Defines.h"

#ifndef emptyExample_Flocking_h
#define emptyExample_Flocking_h

// Flock
class Flocking{
public:
	Flocking(void);
	~Flocking(void);

	vector<Boid> boids;

	void run(ENV_ZONE zone);
	void addBoid(Boid* b);

	ofImage fish[4];
	ofImage images[5][4];

};

#endif
