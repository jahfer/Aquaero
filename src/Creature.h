/* Jahfer's Code */

#pragma once

#include "ofxVectorGraphics.h"
#include "ofxOpenCv.h"
#include "ofxCvMain.h"
#include "ofxBox2d.h"
#include "ofxLightning.h"
#include "Math.h"
#include "Defines.h"
#include <time.h>

class Creature {

public:
/* ================= FUNCTIONS =================== */
	Creature(void);
	Creature(ofxCvBlob* blob, ofxBox2d* pBox2d);
	~Creature(void);

	static void init();
	static void draw(vector<Creature>* creatures);
	static bool isAverageOnLeft(vector<Creature>* creatures); 
	static bool hasId(Creature c);
	bool operator==(const unsigned short int &id) const;

	void animate();
	void destroy();
	void electrocute();
	

/* ================  VARIABLES ================== */
	ofPoint centroid;
	ofPolyline shape, resampled, smoothed;
	ofPath renderer;
	ofxBox2dPolygon b2dProxy;
	float newXPos, newYPos;

	bool isDead;
	bool isDestroyed;
	bool isLaunched;
	ofVec2f scale;
	unsigned short int ID;

	bool electrocuted;
	clock_t timeout;
	ofxLightning bolt;
	
	// Creature Appendage Attachment
	ofPolyline hand;

	float inc;
	float angle;
	float flagCount;
	float stripeCount;
	
	bool mRight;
	bool mLeft;

	int hRight;
	int hLeft;

	float averageX;
	float averageY;
	float difX;
	float difY;

	ofColor col;
	int hue;
	int genShift;
	int dTrue;

	float sX;
	float sY;

	int randomAttach;	
	ofPolyline flagAttach;
	ofPolyline Aattachments;
	ofPolyline Sattachments;
	ofPolyline eAttach;
	ofPolyline mAttach;

	ofPolyline sAttach;
	ofPolyline dAttach;
	ofPolyline hAttach;
	
	ofImage limb;
	ofImage flag;
	ofImage arm;
	ofImage eye;
	ofImage stripe;
	ofImage dot;
	ofImage horn;
	ofImage mouth;
	
	static ofImage flagsI[8];
	static ofImage armsI[8];
	static ofImage eyesI[19];
	static ofImage stripesI[8];
	static ofImage dotsI[8];
	static ofImage hornsI[17];
	static ofImage mouthsI[22];

	static ofImage nFlag[4];
	static ofImage nArms[4];
};
