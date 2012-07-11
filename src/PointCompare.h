#pragma once

#include "ofMain.h"
#include "Defines.h"
#include <time.h>

class PointCompare {
public:
	PointCompare(void);
	~PointCompare(void);
	bool withinRange();
	void setTimeout(unsigned short int ms);
	bool isAvailable();

	clock_t endTime;

	ofPoint oldPt;
	ofPoint newPt;
	int numUnchangedCoords;
	int spawnTimeout;
};