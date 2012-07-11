//
//  Circle.cpp
//  emptyExample
//
//  Created by Stacie Ha on 12-03-16.
//  Copyright 2012 Carleton University. All rights reserved.
//

#include "Circles.h"

Circles::Circles(float ix, float iy){
    x = ix;
    y = iy;
	radius = ofRandom(2,5);

    c.r = 255;
	c.g = 255;
	c.b = 255;
	c.a = 255;

	//t = 1.0f;
	t = 1.0f;
	growing = false;
	isOn = false;

    ofAddListener(ofEvents.mouseMoved, this, &Circles::mouseMoved);
    
}
Circles::~Circles(void){

}


void Circles::draw(){

	if (!isOn && (int)ofRandom(1000) == 5) {
		isOn = true;
		start = clock();
	}

	if (isOn) {
		if (growing && t < 1.0f)  // Grows it
			t+=0.001f;
		
		else if (!growing && t > 0.1f) { // Shrinks it
			t-=0.001f;
		}

		else  { // Changes state
			growing = !growing;
		}
	}


	if ((clock() - start) > 2000)
		isOn = false;

    //ofLog(OF_LOG_NOTICE, "Circle drawn at %2.0f, %2.0f", x, y);

	radius *= t;
	c.a *= t;

	ofPushMatrix();
		ofSetColor(c, c.a*.08);
		ofFill();
		ofCircle(x, y, radius+9*t);
		//ofNoFill();
		//ofCircle(x, y, radius+9*t);
	ofPopMatrix();

	ofPushMatrix();
		ofSetColor(0,0,0, c.a*.03);
		ofFill();
		ofCircle(x, y, radius+5*t);
		ofNoFill();
		ofCircle(x, y, radius+5*t);
	ofPopMatrix();

	ofPushMatrix();
		ofSetColor(c, c.a*.10);
		ofFill();
		ofCircle(x, y, radius+4*t);
		ofNoFill();
		ofCircle(x, y, radius+4*t);
	ofPopMatrix();

	ofPushMatrix();
		ofSetColor(c, c.a);
		ofFill();
		ofCircle(x, y, radius);
		ofNoFill();
		ofCircle(x, y, radius);
	ofPopMatrix();

	//ofCircle(ofRandom(1024), ofRandom(768), radius);
}


void Circles::mouseMoved(ofMouseEventArgs &args){
    mouse.set(args.x, args.y);
}
    
