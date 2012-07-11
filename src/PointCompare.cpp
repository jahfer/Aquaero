#include "PointCompare.h"


PointCompare::PointCompare(void) {
	this->numUnchangedCoords = 0;
	//this->spawnTimeout = 0;
	this->endTime = 0;
}


PointCompare::~PointCompare(void) {
}

void PointCompare::setTimeout(unsigned short int ms) {
	this->endTime = clock() + ms;
}

bool PointCompare::isAvailable() {
	if (!this->endTime)	
		return true;
	else if (this->endTime < clock()) { 
		this->endTime = 0;
		return true;
	} else								
		return false;
}

bool PointCompare::withinRange() {

	return oldPt.match(newPt, MAX_POINT_RANGE);

}