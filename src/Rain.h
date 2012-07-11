#pragma once

#include "ofMain.h"
#include "ofxLightning.h"
#include "Creature.h"
#include <time.h>

class Rain {
public:
	Rain(void);
	~Rain(void);
	
	void setup();
	void update();
	void draw();

	vector<ofPoint> raindrops;
	
	clock_t startTime;
	int rainRate;
	bool isStopped;
	bool isThunder;
	ofxLightning bolt;
	vector<Creature>::iterator target;

	ofVbo vbo;

};

