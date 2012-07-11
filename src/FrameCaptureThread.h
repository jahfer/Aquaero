#pragma once
#include "ofxOpenCv.h"
#include "ofxCvMain.h"
#include "OpticalFlow.h"
#include "vectorField.h"
#include "BlobAnalyzer.h"

#include "ofMain.h"
#include "Defines.h"

class FrameCaptureThread : public ofThread {
public:
	FrameCaptureThread(void);
	~FrameCaptureThread(void);
	void threadedFunction();
	
	bool bLearnBackground;	
	ofPoint screenPosition;

	ofVideoGrabber vidGrabber;
	ofVideoPlayer vidPlayer;
	
	ofxCvContourFinder contourFinder;
	
	ofxCvColorImage	colorImg;
	ofxCvGrayscaleImage greyImg;
	ofxCvGrayscaleImage greyLastImg;
	ofxCvGrayscaleImage greyBg;
	ofxCvGrayscaleImage greyDiff;
	
	// Optical Flow
	OpticalFlow flow;
	vectorField VF;
};

