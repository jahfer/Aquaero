//
//  Circle.h
//  emptyExample
//
//  Created by Stacie Ha on 12-03-16.
//  Copyright 2012 Carleton University. All rights reserved.
//

#include "ofMain.h"
#include <time.h>

#ifndef emptyExample_Circle_h
#define emptyExample_Circle_h

class Circles{
public:
    Circles(float ix, float iy);
    ~Circles(void);
    
    float x, y;
    float radius;
    ofColor c;
	clock_t start;

	float t;
	bool growing, isOn;
    
    void draw();
    
    void mouseMoved(ofMouseEventArgs &args);
    ofVec2f mouse;
};


#endif
