#pragma once
#include "ofxOpenCv.h"
#include "ofxCvMain.h"
#include "vectorField.h"
#include "Defines.h"
#include <vector>

class OpticalFlow {
public:
	OpticalFlow(void);
	~OpticalFlow(void);

	void calc(ofxCvGrayscaleImage* greyLastImg, ofxCvGrayscaleImage* greyImg, vectorField* VF);
	void draw();
	
	vector<uchar> status;
	vector<float> err;
	cv::Size winSize;
	cv::TermCriteria termcrit;
	vector<cv::KeyPoint> keyPoints;
	vector<cv::KeyPoint> nextPoints;

	clock_t start, end;

	vector<cv::Point2f> points_keyPoints, points_nextPoints;
};

