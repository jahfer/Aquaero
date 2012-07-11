/*
 *  Segment.cpp
 *  lightning
 *
 *  Created by rockabit on 15-06-10.
 *  Copyright 2010 Random. All rights reserved.
 *
 */

#include "LSegment.h"

LSegment::LSegment(ofxVec2f aStart, ofxVec2f aEnd, float aIntensity = 1.0) {
	
	start = aStart;
	end = aEnd;
	intensity = aIntensity;
}

float LSegment::getLength() {
	
	return start.distance(end);
}

float LSegment::getRadians() {
	
	return atan2(end.y - start.y, end.x - start.x);
}

float LSegment::getDegrees() {
	
	return atan2(end.y - start.y, end.x - start.x) * 180 / PI;
}

ofxVec2f LSegment::getMidPoint() {

	return start.getMiddle(end);
}