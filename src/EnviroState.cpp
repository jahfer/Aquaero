#include "EnviroState.h"


EnviroState::EnviroState(void) {
	vents.color = ofColor(0, 150);
	bubbles.color = ofColor(125, 195, 225, 70);
}

EnviroState::~EnviroState(void) {}

void EnviroState::init(ofxBox2d* pBox2d) {
	vents.init(pBox2d);
	bubbles.init(pBox2d);
	spaceObj.init(pBox2d);
}

bool EnviroState::isRunning() {

	if (!vents.isStopped) {
		return true;
	} else if (!rain.isStopped) {
		return true;
	} else if (!bubbles.isStopped) {
		return true;
	} else 
		return false;
}

void EnviroState::set(ENV_STATE iMode) {
	mode = iMode;

	switch(mode) {
		case ENV_VENTS:
			vents.setup();
			break;
		case ENV_BUBBLES:
			bubbles.setup();
			break;
		case ENV_STORM:
			rain.setup();
			break;
		/*case ENV_METEOR:
			if (spaceObj.isStopped)
				spaceObj.setup(ENV_METEOR);
			break;
		case ENV_COMET:
			if (spaceObj.isStopped)
				spaceObj.setup(ENV_COMET);
			break;*/
	}

}

void EnviroState::update() {
	switch(mode) {
		case ENV_VENTS:
			vents.update();
			break;

		case ENV_BUBBLES:
			bubbles.update();
			break;

		case ENV_STORM:
			rain.update();
			break;

		/*case ENV_COMET:
		case ENV_METEOR:
			spaceObj.update();
			break;*/
	}
}

void EnviroState::draw() {
	switch(mode) {
		case ENV_VENTS:
			vents.draw();
			break;

		case ENV_BUBBLES:
			bubbles.draw();
			break;

		case ENV_STORM:
			rain.draw();
			break;

		/*case ENV_COMET:
		case ENV_METEOR:
			spaceObj.draw();
			break;*/
	}
}