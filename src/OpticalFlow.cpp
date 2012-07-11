#include "OpticalFlow.h"



OpticalFlow::OpticalFlow(void) {	
	winSize.width = winSize.height = 5;
	termcrit.type = CV_TERMCRIT_ITER|CV_TERMCRIT_EPS;
	termcrit.maxCount = 20;
	termcrit.epsilon = 0.03;
}


OpticalFlow::~OpticalFlow(void) {
	
}


// http://forum.openframeworks.cc/index.php/topic,6904.msg32891.html#msg32891

void OpticalFlow::calc(ofxCvGrayscaleImage* greyLastImg, ofxCvGrayscaleImage* greyImg, vectorField* VF) {
	
	keyPoints.clear();
	nextPoints.clear();
	points_keyPoints.clear();
	points_nextPoints.clear();

	cv::FAST(greyLastImg->getCvImage(), keyPoints, 60, true);
	cv::FAST(greyImg->getCvImage(), nextPoints, 60, true);
	cv::KeyPoint::convert(keyPoints, points_keyPoints);
	cv::KeyPoint::convert(nextPoints, points_nextPoints);

	cv::calcOpticalFlowPyrLK(greyLastImg->getCvImage(), greyImg->getCvImage(),
								points_keyPoints, points_nextPoints, status, err,
								winSize, 3, termcrit, 0.5, 1);
	
	
	for (int i=0; i < points_nextPoints.size(); i++) {	

		float vx = points_nextPoints[i].x - points_keyPoints[i].x;
		float vy = points_nextPoints[i].y - points_keyPoints[i].y;

		VF->addVectorCircle(
			points_nextPoints[i].x * CAM_SCALE_X, 
			points_nextPoints[i].y * CAM_SCALE_Y,
			vx, vy, 
			VF_RADIUS,   // radius
			VF_STRENGTH // strength
		);
	}
	
}

void OpticalFlow::draw() {

	ofSetHexColor(0xffffff);
	int pointCount = points_nextPoints.size();
	for (int i=0; i < points_nextPoints.size(); i++) {
		ofNoFill();
		ofSetHexColor(0xff0000);
		ofLine(
			points_keyPoints[i].x,  points_keyPoints[i].y, 
			points_nextPoints[i].x, points_nextPoints[i].y
		);
	}

}
