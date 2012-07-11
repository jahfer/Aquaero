#include "Bubbles.h"

Bubbles::Bubbles(void) {
}

void Bubbles::init(ofxBox2d* pBox2d) {
	b2d = pBox2d;
	isStopped = true;
}

void Bubbles::setup() {	
	startTime = 0;
	isStopped = false;
	circles.clear();

	xPosMin = ofRandom(1230);
	xPosMax = xPosMin + 50;
}

void Bubbles::update() {	
	
	if (isStopped) return;

	if (!startTime) 
		startTime = clock();
	if (clock() - startTime > 1000) {
		isStopped = true;
	}
	if (circles.size() > 100) return;

	ofxBox2dCircle circle;
	circle.setPhysics(0.1f, 0, 0.1f);
	circle.setup(b2d->getWorld(), ofRandom(xPosMin, xPosMax), 800, ofRandom(2, 20));
	circle.addForce(ofVec2f(0, -ofRandom(200, 500)), 1);

	circles.push_back(circle);
}

void Bubbles::draw() {	
	
	ofPushMatrix();
		ofFill();
		ofTranslate(1, 1, DEPTH_VENTS);

		for (int i=0; i<circles.size(); i++) {
			ofSetColor(color);

			if (circles[i].getB2DPosition().y < 0) {
				//circles
				circles.erase(circles.begin()+i);
				i--;
				continue;
			}

			ofCircle(circles[i].getPosition(), circles[i].getRadius());
		}
	ofPopMatrix();
}


Bubbles::~Bubbles(void) {}
