#pragma once
#include "ofxOpenCv.h"
#include "ofxBox2d.h"
#include "PointCompare.h"
#include "Creature.h"

#include "Defines.h"

class BlobAnalyzer {
public:
	BlobAnalyzer(void);
	~BlobAnalyzer(void);

	void read(ofxCvContourFinder* contourFinder, vector<Creature>* creatures, ofxBox2d* pBox2d);

	vector<PointCompare*> blobList;
	ofEvent<Creature*> onCreatureSpawn;
};

