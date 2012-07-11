#pragma once

#include "Defines.h"
#include "ofxBox2d.h"
#include <time.h>
#include "ofMain.h"

class SpaceObject {
public:
	SpaceObject(void);
	~SpaceObject(void);

	void init(ofxBox2d *pBox2d);
	void setup(ENV_STATE env);
	void update();
	void draw();

	ENV_STATE type;
	ofxBox2d *b2d;
	float angle;
	bool launch;
	ofxBox2dCircle b2dProxy;
	ofImage comet, meteor;

	clock_t startTime;
	bool isStopped;
	
};

