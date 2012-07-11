//
//  stars.cpp
//  emptyExample
//
//  Created by Stacie Ha on 12-03-15.
//  Copyright 2012 Carleton University. All rights reserved.
//

#include <iostream>
#include "Stars.h"

Stars::Stars(void){
    
    int numStars = 50;
    
    for(int i = 0; i < numStars; i++) {
        circles.push_back( Circles(ofRandom(1024), ofRandom(768)) );
    }
}

Stars::~Stars(void){}

void Stars::draw(){
	//blur.setScale(circles[0].t);
	//blur.setRotation(0);
    
	//blur.begin();

    for(int i=0; i<circles.size(); i++){
        circles[i].draw();
    }

	//blur.end();
	//blur.draw();
		
	ofFill();
	ofSetColor(255);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);

}

