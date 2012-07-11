#include "BlobAnalyzer.h"


BlobAnalyzer::BlobAnalyzer(void) {

}


BlobAnalyzer::~BlobAnalyzer(void) {

}


void BlobAnalyzer::read(ofxCvContourFinder* contourFinder, vector<Creature>* creatures, ofxBox2d* pBox2d) {
	
	// Go through all blobs found
	for (int i=0; i < contourFinder->nBlobs; i++) {

		PointCompare* b;		
		// Draw the blob contour on screen
		//contourFinder->blobs[i].draw(320, 240);

		// If the array isn't large enough to hold the blobs, expand
		if ( i > blobList.size()-1 || blobList.empty() ) {
			PointCompare* tmp = new PointCompare;
			blobList.push_back(tmp);
			b = blobList.back();
			b->oldPt = b->newPt = contourFinder->blobs[i].centroid;
		// Otherwise, use existing array position
		} else {
			// Get blob's previous and current position
			b = blobList[i];
			b->oldPt = b->newPt;
			b->newPt = contourFinder->blobs[i].centroid;
			
			// Check if able to create a new blob
			if( !b->isAvailable() ) break;

			// Check if the blob has moved
			if(b->withinRange()) {				
				b->numUnchangedCoords++;
			} else {
				b->numUnchangedCoords = 0;				
			}
		}

		// If blob hasn't moved in a while
		if(b->numUnchangedCoords > SPAWN_THRESHOLD) {
			contourFinder->blobs[i].centroid.x *= CAM_SCALE_X;
			contourFinder->blobs[i].centroid.y *= CAM_SCALE_Y;

			for (vector<ofPoint>::iterator p = contourFinder->blobs[i].pts.begin(); p != contourFinder->blobs[i].pts.end(); ++p) {
				p->x *= CAM_SCALE_X;
				p->y *= CAM_SCALE_Y;
			}

			// Create new character	
			Creature* c = new Creature(&contourFinder->blobs[i], pBox2d);
			// Time-out system from creating a new one
			b->setTimeout(SPAWN_TIMEOUT);
			// reset count
			b->numUnchangedCoords = 0;

			ofNotifyEvent(onCreatureSpawn, c);

		}
	}
}