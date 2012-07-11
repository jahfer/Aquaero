#pragma once

#include "Defines.h"
#include "Rain.h"
#include "ofxLightning.h"
#include "Creature.h"
#include "Bubbles.h"
#include "SpaceObject.h"

class EnviroState {

public:
	EnviroState(void);
	~EnviroState(void);

	void init(ofxBox2d* pBox2d);
	void set(ENV_STATE iMode);
	void update();
	void draw();
	bool isRunning();

	ENV_STATE mode;

	// Thunder storm
	Rain rain;
	Bubbles vents;
	Bubbles bubbles;
	SpaceObject spaceObj;

};

