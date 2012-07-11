#include "Rain.h"


Rain::Rain(void) {
	isStopped = true;

	ofVec3f v[4];
	ofFloatColor c[4];
	const ofIndexType Faces[] = {2, 0, 1, 3};

	// top left
	c[0].r = 0.118f;
	c[0].g = 0.34f;
	c[0].b = 0.6f;
	// top right
	c[1].r = 0.118f;
	c[1].g = 0.34f;
	c[1].b = 0.6f;
	// bottom left
	c[2].r = 0.49f;
	c[2].g = 0.725f;
	c[2].b = 0.91f;
	// bottom right
	c[3].r = 0.49f;
	c[3].g = 0.725f;
	c[3].b = 0.91f;
	
	// top left
	v[0].x = 1.0f;
	v[0].y = 0.0f;
	v[0].z = 0.0f;	
	// top right
	v[1].x = 2.0f;
	v[1].y = 0.0f;
	v[1].z = 0.0f;
	// bottom left	
	v[2].x = 0.0f;
	v[2].y = 60.0f;
	v[2].z = 0.0f;
	// bottom right
	v[3].x = 3.0f;
	v[3].y = 60.0f;
	v[3].z = 0.0f;

	vbo.setVertexData(&v[0], 4, GL_STATIC_DRAW);
	vbo.setColorData(&c[0], 4, GL_STATIC_DRAW);
	vbo.setIndexData(&Faces[0], 4, GL_STATIC_DRAW);

	glEnable(GL_DEPTH_TEST);
	
}


Rain::~Rain(void) {
}

void Rain::setup() {
	startTime = 0;
	isStopped = false;
	raindrops.empty();
}

void Rain::update() {

	for(vector<ofPoint>::iterator i = this->raindrops.begin(); i != this->raindrops.end(); ++i) {
		i->y += 10;
		i->x -= 5;
	}

	if(isStopped) return;

	if (!startTime) 
		startTime = clock();
	clock_t curTime = clock();

	if (curTime - startTime > 9000)
		isStopped = true;
	else if (curTime - startTime > 7000)
		rainRate = 1;
	else if (curTime - startTime > 5000)
		rainRate = 2;
	else if (curTime - startTime > 3000)
		rainRate = 3;
	else if (curTime - startTime > 1000)
		rainRate = 2;
	else
		rainRate = 1;

	for (unsigned short int i=0; i<rainRate; i++)
		this->raindrops.push_back(ofPoint(ofRandom(1600), ofRandom(-10, 0), 0));

	if((int)ofRandom(100) == 2 && !this->isStopped) 
		this->isThunder = true;
	else 
		this->isThunder = false;
}

void Rain::draw() {
	
	ofFill();
	ofSetHexColor(0x00CCFF);

	for(vector<ofPoint>::iterator i = this->raindrops.begin(); i != this->raindrops.end(); ++i) {
		if(i->y < 1280) {
			ofPushMatrix();
				ofTranslate(*i);
				ofRotate(22.5f);
				glPointSize(10.0f);
				vbo.drawElements(GL_QUADS, 4);
			ofPopMatrix();

		} else {
			this->raindrops.erase(i);
			i--;
		}
	}

	if(this->isThunder) {
		bolt.setup(ofRandom(1300), 0, target->b2dProxy.getCenter().x, target->b2dProxy.getCenter().y);
		bolt.parse();
		bolt.draw();
		target->electrocuted = true;
	}
}