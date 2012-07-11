#pragma once

#include "ofxBox2d.h"
#include "ofMain.h"
#include "Defines.h"
#include <time.h>

class Bubbles {

public:
	Bubbles(void);
	~Bubbles(void);

	void init(ofxBox2d* pBox2d);
	void setup();
	void update();
	void draw();

	ofxBox2d* b2d;
	bool isStopped;
	clock_t startTime;
	ofColor color;

	float xPosMin;
	float xPosMax;

	vector<ofxBox2dCircle> circles;

};