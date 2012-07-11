//
//  stars.h
//  emptyExample
//
//  Created by Stacie Ha on 12-03-15.
//  Copyright 2012 Carleton University. All rights reserved.
//

#include "ofMain.h"
#include "Circles.h"

#ifndef emptyExample_stars_h
#define emptyExample_stars_h

class Stars{
public:
    Stars(void);
    ~Stars(void);
    
    vector<Circles>circles;
    
    void update();
    void draw();
    void display();
    int numStars;

};



#endif
