/* Jahfer's Code */

#include "Creature.h"

Creature::Creature() {}
Creature::~Creature(void) {}

// find creature based on ID
// if (creature == 4) ...
bool Creature::operator==(const unsigned short int &other) const {
	return this->ID == other;
}

Creature::Creature(ofxCvBlob* blob, ofxBox2d* pBox2d) {
	
/* ================= Init Variables =================== */

	this->isDead = false;
	this->isDestroyed = false;
	this->centroid = blob->centroid;
	this->electrocuted = false;
	this->scale.set(1.0f, 1.0f);
	this->timeout = 0;

	//set colour
	//col.set(ofRandom(255), ofRandom(255), ofRandom(255));
	col.setHsb(ofRandom(255), 190, 255);
	hue = col.getHue();
	

/* ================== SHAPE DESIGN ==================== */

	// Create shape from blob
	this->shape.addVertexes(blob->pts);
	this->shape.setClosed(true);
	this->shape.simplify();
	this->hand.addVertexes(this->shape.getVertices());
	this->smoothed = this->shape.getSmoothed(4);

	// Prepare to draw
	this->renderer.setMode(ofPath::POLYLINES);
	this->renderer.setFillColor(col);
	
	// Convert smoothed shape from ofPolyline to ofPath
	for(vector<ofPoint>::iterator i = this->smoothed.getVertices().begin(); 
		i != this->smoothed.getVertices().end(); ++i) {
		this->renderer.lineTo(*i);
	}
	

/* ================== APPENDAGES ==================== */
	

	// Appendage variables
	this->inc = 0;
	this->angle = 0;
	this->flagCount = 0;
	this->stripeCount = 0;
	this->genShift = ofRandom(-10,10);
	this->dTrue = 0;

	mRight = mLeft = false;

	hRight = hLeft = 0;

	ofSeedRandom();
	
	if(hue > 0 && hue < 63) {		
		randomAttach = (int)ofRandom(1,4);
		flag = Creature::nFlag[randomAttach];
		
		randomAttach = (int)ofRandom(0,1);
		arm = Creature::nArms[1];
		
		randomAttach = (int)ofRandom(1,19);
		eye = Creature::eyesI[randomAttach];

		randomAttach = (int)ofRandom(0,22);
		mouth = Creature::mouthsI[randomAttach];

		randomAttach = (int)ofRandom(0,17);
		horn = Creature::hornsI[randomAttach];
	} //GREEN
	else if(hue >= 63 && hue < 127) {	
		randomAttach = (int)ofRandom(1,4);
		flag = Creature::nFlag[randomAttach];
		
		randomAttach = (int)ofRandom(2,2);
		arm = Creature::nArms[2];
		
		randomAttach = (int)ofRandom(1,19);
		eye = Creature::eyesI[randomAttach];

		randomAttach = (int)ofRandom(0,22);
		mouth = Creature::mouthsI[randomAttach];

		randomAttach = (int)ofRandom(0,17);
		horn = Creature::hornsI[randomAttach];
	} //BLUE
	else if (hue >= 127 && hue < 190) {
		randomAttach = (int)ofRandom(1,4);
		flag = Creature::nFlag[randomAttach];
		
		randomAttach = (int)ofRandom(3,3);
		arm = Creature::nArms[3];

		randomAttach = (int)ofRandom(1,19);
		eye = Creature::eyesI[randomAttach];

		randomAttach = (int)ofRandom(0,22);
		mouth = Creature::mouthsI[randomAttach];

		randomAttach = (int)ofRandom(0,17);
		horn = Creature::hornsI[randomAttach];
	} //RED
	else if (hue >= 190 && hue < 255) {
		randomAttach = (int)ofRandom(0,0);
		flag = Creature::nFlag[0];
		
		randomAttach = (int)ofRandom(1,4);
		arm = Creature::nArms[4];

		randomAttach = (int)ofRandom(0,19);
		eye = Creature::eyesI[randomAttach];

		randomAttach = (int)ofRandom(0,22);
		mouth = Creature::mouthsI[randomAttach];

		randomAttach = (int)ofRandom(0,17);
		horn = Creature::hornsI[randomAttach];
	}
		
	//marks FLAGELLA attachment points
	for(int i = 0; i < hand.size(); i++) {
		if(hand[i].y < hand[i-5].y && hand[i].y < hand[i+5].y && flagCount <= 0) {
			flagAttach.addVertex(ofVec2f(1,1));
			flagCount = 5;
		} else {
			flagAttach.addVertex(ofVec2f(0,0));
			flagCount--;
		}
	}
	
	//marks ARM attachment points
	for(int i = 0; i < hand.size(); i++) {
		averageX = averageX + hand[i].x;
		averageY = averageY + hand[i].y;
	}

	averageX = averageX/hand.size();
	averageY = averageY/hand.size();
	
	for (int i = 0; i < hand.size(); i++) {	
		difX = abs(hand[i].y - averageY);
		difY = abs(hand[i].x - averageX);

		float atanA = atan2(hand[i].y - averageY,hand[i].x - averageX) * 57;

		if (atanA > (45+genShift) && atanA < (50+genShift) && mRight == false) {
			Aattachments.addVertex(ofVec2f(1,1));
			mRight = true;

		} else if (atanA > (90+genShift) && atanA < (95+genShift) && mLeft == false) {
			Aattachments.addVertex(ofVec2f(1,1));
			mLeft = true;
		} else {
			Aattachments.addVertex(ofVec2f(0,0));
		}
	}

	//marks STRIPE locations
	for (int i = 0; i < hand.size() - 0; i++) {
		if ( hand[i].y > (averageY + 30) && stripeCount <= 0) {
			sAttach.addVertex(ofVec2f(1,1));
			stripeCount = 5;
		} else {
			sAttach.addVertex(ofVec2f(0,0));
			stripeCount --;
		}
	}
		
	//corrects DOTS locations
	for (int i = 0 ; i < hand.size(); i ++) {
		dTrue = 0;
		sX = hand[ofRandom(hand.size())].x;
		sY = hand[ofRandom(hand.size())].y;

		for (int j = 0 ; j < hand.size()-1; j ++) {
			if( ( ((sY - hand[j].y)*(hand[j+1].x - hand[j].x)) - ((sX - hand[j].x)*(hand[j+1].y - hand[j].y)) ) > 0) {
				break;
			}
		}

		if (dTrue > 20) {// hand.size()-1)
			dAttach.addVertex(sX,sY);
		}	
	}

	//marks HORN locations
	for (int i = 0; i < hand.size(); i++) {

		float atanB  = atan2(hand[i].y - centroid.y,hand[i].x - centroid.x) * 57;
		float atanC =  atan2(hand[i].y - averageY,hand[i].x - averageX) * 57;

		/*if (atanB  > (55+genShift) && atanB < (60+genShift) && hRight == false) {
			hAttach.addVertex(ofVec2f(1,1));
			hRight = true;
		} else if (atanC > (70+genShift) && atanC < (75+genShift) && hLeft == false) {
			hAttach.addVertex(ofVec2f(1,1));
			hLeft = true;
		} else {
			hAttach.addVertex(ofVec2f(0,0));
		}*/


		if (atanB  > (150) && atanB < (160) && hRight <= 5) {
			hAttach.addVertex(ofVec2f(1,1));
			hRight++;
		} else if (atanC > (80) && atanC < (90) && hLeft <= 5) {
			hAttach.addVertex(ofVec2f(1,1));
			hLeft++;
		} else {
			hAttach.addVertex(ofVec2f(0,0));
		}
	}
	
	// moved from draw()
	flag.setAnchorPoint(flag.getWidth()*0.5, 0);
	arm.setAnchorPoint(arm.getWidth()*0.5, 0);
	
	eye.setAnchorPoint(eye.getWidth()*0.5, eye.getHeight()*0.5);
	mouth.setAnchorPoint(mouth.getWidth()*0.5, mouth.getHeight()*0.5);


/* ====================== BOX2D ======================== */
	
	// Convert shape from ofPolyline to ofPath
	for(vector<ofPoint>::iterator i = this->shape.getVertices().begin(); i != this->shape.getVertices().end(); ++i) {
		ofPoint pt = *i;
		this->renderer.lineTo(pt);
	}	
	
	// resize shape to end-shrinkage before making box2d object
	this->renderer.scale(0.5f, 0.5f);
	
	// offset shape so scaled object is in center of blob
	ofPoint ctr = this->shape.getBoundingBox().getCenter() * 0.5;
	for(vector<ofPoint>::iterator i = this->renderer.getOutline()[0].getVertices().begin(); i != this->renderer.getOutline()[0].getVertices().end(); ++i) {
		*i += ctr;
	}	

	// Simplify shape for box2d calculations
	this->b2dProxy.addVertexes(this->renderer.getOutline()[0]);
	b2dProxy.triangulate();

	// Set up box2d object
	b2dProxy.setPhysics(5.0f, 0.03f, 0.3f);
	b2dProxy.create(pBox2d->getWorld());
	
	//marks EYE locations
	//eAttach.addVertex(ofRandom(-10,0),ofRandom(0,30));
	//eAttach.addVertex(ofRandom(0,20),ofRandom(0,20));
	//eAttach.addVertex(this->b2dProxy.getCenter()-centroid-30);
	//eAttach.addVertex(this->b2dProxy.getCenter()-centroid+30);
	
/*==================== EYE AND MOUTH LOCATIONS ================*/
	
	//marks EYE locations
	/*float eyeX = 0;
	float eyeY = 0;
	for(int i = 0; shape.size(); i++){
		if(shape[i].x > eyeX) eyeX = shape[i].x;
		if(shape[i].y > eyeY) eyeY = shape[i].y;
	}
	eAttach.addVertex(eyeX,eyeY);

	for(int i = 0; shape.size(); i++){
		if(shape[i].x < eyeX) eyeX = shape[i].x;
		if(shape[i].y < eyeY) eyeY = shape[i].y;
	}
	eAttach.addVertex(eyeX,eyeY);
	*/
	eAttach.addVertex(this->b2dProxy.getCenter().x - centroid.x - 75, this->b2dProxy.getCenter().y - centroid.y);
	eAttach.addVertex(this->b2dProxy.getCenter().x - centroid.x, this->b2dProxy.getCenter().y - centroid.y);

	//marks MOUTH locations
	mAttach.addVertex(this->b2dProxy.getCenter().x - centroid.x - 15 - this->mouth.width*0.5, this->b2dProxy.getCenter().y - centroid.y + 100);
	
/* ===================== CLEAN-UP ======================= */

	this->smoothed.clear();
	this->resampled.clear();
	
}

void Creature::electrocute() {

	clock_t now = clock();

	if (!this->timeout)
		this->timeout = now;

	if (now - timeout > 3000) {
		if (!this->isDead)
			this->isDead = true;
	}

	this->bolt.setup(0, 0, 0, 0, 6, 60.0f);
	this->bolt.parse();
	ofPushMatrix();
		ofTranslate(0, 0, -10);
		ofPushStyle();
			this->bolt.draw();
		ofPopStyle();
	ofPopMatrix();

	if(this->renderer.getFillColor() != ofColor::black)
		this->renderer.setFillColor(ofColor::black);
}


void Creature::animate() {	
	this->scale -= 0.01;
}

void Creature::destroy() {	
	this->b2dProxy.destroy();
	this->isDestroyed = true;

	ofLog(OF_LOG_VERBOSE, "- creature destroyed");
}

 /* =============================================================================
	Static Functions
	========================================================================== */

void Creature::init() {
	
	//flagella
	Creature::nFlag[0].loadImage("flags/Rflag.png");
	Creature::nFlag[1].loadImage("flags/Gflag.png");
	Creature::nFlag[2].loadImage("flags/Bflag.png");
	Creature::nFlag[3].loadImage("flags/Yflag.png");


	//arms
	Creature::nArms[0].loadImage("arms/Barm.png");
	Creature::nArms[1].loadImage("arms/Garm.png");
	Creature::nArms[2].loadImage("arms/Garm2.png");
	Creature::nArms[3].loadImage("arms/Rarm.png");
	Creature::nArms[4].loadImage("arms/Yarm.png");

	//eyes
	Creature::eyesI[0].loadImage("eyes/eye1.png");
	Creature::eyesI[1].loadImage("eyes/eye_29_right.png");
	Creature::eyesI[2].loadImage("eyes/eye3.png");
	Creature::eyesI[3].loadImage("eyes/eye4.png");
	Creature::eyesI[4].loadImage("eyes/eye2.png");
	Creature::eyesI[5].loadImage("eyes/eye6.png");
	Creature::eyesI[6].loadImage("eyes/eye7.png");
	Creature::eyesI[7].loadImage("eyes/eye_21.png");
	Creature::eyesI[8].loadImage("eyes/eye9.png");
	Creature::eyesI[9].loadImage("eyes/eye10.png");
	Creature::eyesI[10].loadImage("eyes/eye11.png");
	Creature::eyesI[11].loadImage("eyes/eye12.png");
	Creature::eyesI[12].loadImage("eyes/eye13.png");
	Creature::eyesI[13].loadImage("eyes/eye14.png");
	Creature::eyesI[14].loadImage("eyes/eye15.png");
	Creature::eyesI[15].loadImage("eyes/eye16.png");
	Creature::eyesI[16].loadImage("eyes/eye17.png");
	Creature::eyesI[17].loadImage("eyes/eye18.png");
	Creature::eyesI[18].loadImage("eyes/eye19.png");
	Creature::eyesI[19].loadImage("eyes/eye_07.png");
	
	//horns
	Creature::hornsI[0].loadImage("horns/Bhorn.png");
	Creature::hornsI[1].loadImage("horns/BLhorn.png");
	Creature::hornsI[2].loadImage("horns/BLhorn2.png");
	Creature::hornsI[3].loadImage("horns/BRhorn.png");
	Creature::hornsI[4].loadImage("horns/Rhorn2.png");
	Creature::hornsI[5].loadImage("horns/BRhorn2.png");
	Creature::hornsI[6].loadImage("horns/Ghorn.png");
	Creature::hornsI[7].loadImage("horns/GLhorn.png");
	Creature::hornsI[8].loadImage("horns/GRhorn.png");
	Creature::hornsI[9].loadImage("horns/Rhorn.png");
	Creature::hornsI[10].loadImage("horns/RLhorn.png");	
	Creature::hornsI[11].loadImage("horns/RRhorn.png");
	Creature::hornsI[12].loadImage("horns/Yhorn.png");
	Creature::hornsI[13].loadImage("horns/Yhorn2.png");
	Creature::hornsI[14].loadImage("horns/YLhorn.png");
	Creature::hornsI[15].loadImage("horns/YLhorn2.png");
	Creature::hornsI[16].loadImage("horns/YRhorn.png");
	Creature::hornsI[17].loadImage("horns/YRhorn2.png");

	//mouths
	Creature::mouthsI[0].loadImage("mouths/mouth_01.png");
	Creature::mouthsI[1].loadImage("mouths/mouth_02.png");
	Creature::mouthsI[2].loadImage("mouths/mouth_03.png");
	Creature::mouthsI[3].loadImage("mouths/mouth_04.png");
	Creature::mouthsI[4].loadImage("mouths/mouth_05.png");
	Creature::mouthsI[5].loadImage("mouths/mouth_06.png");
	Creature::mouthsI[6].loadImage("mouths/mouth_07.png");
	Creature::mouthsI[7].loadImage("mouths/mouth_08.png");
	Creature::mouthsI[8].loadImage("mouths/mouth_09.png");
	Creature::mouthsI[9].loadImage("mouths/mouth_10.png");
	Creature::mouthsI[10].loadImage("mouths/mouth_11.png");
	Creature::mouthsI[11].loadImage("mouths/mouth_12.png");
	Creature::mouthsI[12].loadImage("mouths/mouth_13.png");
	Creature::mouthsI[13].loadImage("mouths/mouth_14.png");
	Creature::mouthsI[14].loadImage("mouths/mouth_15.png");
	Creature::mouthsI[15].loadImage("mouths/mouth_16.png");
	Creature::mouthsI[16].loadImage("mouths/mouth_17.png");
	Creature::mouthsI[17].loadImage("mouths/mouth_18.png");
	Creature::mouthsI[18].loadImage("mouths/mouth_19.png");
	Creature::mouthsI[19].loadImage("mouths/mouth_20.png");
	Creature::mouthsI[20].loadImage("mouths/mouth_21.png");
	Creature::mouthsI[21].loadImage("mouths/mouth_22.png");


}

void Creature::draw(vector<Creature>* creatures) {
	
	for (vector<Creature>::iterator c = creatures->begin(); c != creatures->end(); ++c) {
		
	 /* ===================== CREATURE CLEAN-UP ======================= */

		// Remove destroyed creatures from vector
		if(c->isDestroyed) {
			creatures->erase(c);
			c--; continue;
		}
		// Destroy all dead creatures
		else if (c->isDead) {
			if (c->scale.match(ofVec2f(0.0f, 0.0f), 0.01f))
				c->destroy();
			else
				c->animate();
		// shrink down to interaction scale
		} else if (! c->scale.match(ofVec2f(0.5f, 0.5f), 0.01f)) 
			c->animate();

		// Empty renderer for next pass
		c->renderer.clear();

		
	 /* ===================== DEFORMATION MATH ======================= */

		ofVec3f center = c->b2dProxy.getCenter();
		c->newXPos = center.x - c->centroid.x;
		c->newYPos = center.y - c->centroid.y;

		// Apply ripple effect
		for(int i = 0; i < c->shape.size(); i++) {
			c->hand[i].x = c->shape[i].x + (sin(c->shape[i - abs(i % 5)].y + c->inc) * CREATURE_WIGGLE_AMT);
			c->hand[i].y = c->shape[i].y + (sin(c->shape[i - (i % 5)].x + c->inc) * CREATURE_WIGGLE_AMT);
			c->inc += CREATURE_WIGGLE_SPEED;
		}
		
		// Clean up creature shape
		c->hand.setClosed(true);
		c->hand = c->hand.getSmoothed(4);
				
		for(vector<ofPoint>::iterator i = c->hand.getVertices().begin(); i != c->hand.getVertices().end(); ++i) {
			c->renderer.lineTo(*i);
	    }

		ofPushMatrix();						
			glTranslatef(center.x, center.y, 0); 
			glScalef(c->scale.x, c->scale.y, 0);
			ofRotate(c->b2dProxy.getRotation());
			
			// Draw electrocution
			if (c->electrocuted) 
				c->electrocute();

			// Out of bounds
			if (center.y < 0 || center.y > WIN_H || center.x < 0 || center.x > WIN_W)	 
				c->isDead = true;
			
			//glCullFace(GL_BACK);
			// Draw creature shape
			//c->renderer.draw(-c->centroid.x, -c->centroid.y);
			
			for (int i = 0; i < c->hand.size()-1; i++) {
				// Calculate normals
				c->angle = atan( (c->hand[i].y - c->hand[i+6].y) / (c->hand[i].x - c->hand[i+6].x) );
				c->angle *= 57.2957795;

				// Correct normals
				if(c->hand[i+6].x <= c->hand[i].x) {
					c->angle += 180;
				}
							
				//glCullFace(GL_FRONT);
						
				ofPoint transPos;
				transPos.x = (c->hand[i].x + c->hand[i+1].x)*0.5 - c->centroid.x;
				transPos.y = (c->hand[i].y + c->hand[i+1].y)*0.5 - c->centroid.y;
				
				ofPushMatrix();
					ofTranslate(transPos);
					ofRotate(c->angle);
					// Draw flagella
					if(c->flagAttach[i].x != 0 ) 
							c->flag.draw(0 - c->flag.width*0.5,-10);						
					// Draw arms
					if(c->Aattachments[i].x != 0) 
						c->arm.draw(0 - c->arm.width*0.5,-10);
									
					// Draw horns
					if(c-> hAttach[i].x != 0 ) 
						//ofRect(0,0,3,10);
						c->horn.draw(0 - c->horn.width*0.5,-10);
				ofPopMatrix();
			}

			c->renderer.draw(-c->centroid.x, -c->centroid.y);
			
			//Draw face
			ofPushMatrix();
				ofPushStyle();
					ofSetColor(c->renderer.getFillColor());
					ofCircle(-10, 45, 75);
				ofPopStyle();
			ofPopMatrix();

			// Draw eyes
			ofPushMatrix();
				ofScale(.8,.8,1);
				c->eye.draw(c->eAttach[0].x,c->eAttach[0].y);
				c->eye.draw(c->eAttach[1].x,c->eAttach[1].y);
			ofPopMatrix();

			// Draw mouth
			if (c->shape.getPerimeter() > 750) {
				ofPushMatrix();
					ofScale(.8,.8,1);
					c->mouth.draw(c->mAttach[0].x, c->mAttach[0].y);
				ofPopMatrix();
			}

			// Draws dots
			for(int i = 0; i < c->dAttach.size(); i++) {
				ofPushMatrix();
					ofTranslate(c->dAttach[i].x - c->centroid.x, c->dAttach[i].y - c->centroid.y);
					ofEllipse(0,0,2,2);
				ofPopMatrix();
			}
			
		ofPopMatrix();
	}

	//glCullFace(GL_BACK);
}

// Find average position of all creatures on screen
bool Creature::isAverageOnLeft(vector<Creature>* creatures) {

	float avgX = 0.0f;
	for (vector<Creature>::iterator c = creatures->begin(); c != creatures->end(); ++c) {
		avgX += c->b2dProxy.getCenter().x;
	}	

	avgX /= creatures->size();

	return (avgX >= WIN_W_HALF) ? false : true;

}