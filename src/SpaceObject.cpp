#include "SpaceObject.h"

SpaceObject::SpaceObject(void) {
	this->comet.loadImage("comet.png");
	this->meteor.loadImage("meteor.png");
	this->launch = false;
}

SpaceObject::~SpaceObject(void) {}

void SpaceObject::init(ofxBox2d *pBox2d) {
	this->startTime = 0;
	this->b2d = pBox2d;
	this->isStopped = true;

	this->b2dProxy.setPhysics(0.1f, 0, 0.1f);
	this->b2dProxy.setup(this->b2d->getWorld(), 1100, 5, 20);
	this->b2dProxy.addForce(ofVec2f(-132, 264), 1);
}

void SpaceObject::setup(ENV_STATE env) {
	this->type = env;
	this->angle = 30;//ofRandom(-30, 30);

	if (!this->startTime) {
		this->startTime = clock();
		this->isStopped = false;
	}

	this->b2dProxy.setPosition(1100, 5);
	//b2dProxy = new ofxBox2dCircle();
}

void SpaceObject::update() {

	if (clock() - this->startTime > 1000) {
		this->isStopped = true;
		this->startTime = 0;
	}
		
	if((int)ofRandom(300) == 2) 
		this->launch = true;
}

void SpaceObject::draw() {

	ofPushMatrix();
		ofTranslate(210, -meteor.height+80);
		ofTranslate(this->b2dProxy.getPosition());
		ofRotate(this->angle);

		switch (this->type) {
		case ENV_METEOR:
			this->meteor.draw(0, 0);
			break;
		case ENV_COMET:
			this->comet.draw(0, 0);
			break;
		}

	ofPopMatrix();
}
