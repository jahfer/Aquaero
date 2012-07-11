#include "FrameCaptureThread.h"


FrameCaptureThread::FrameCaptureThread(void) {
	bLearnBackground = true;

	ofLog(OF_LOG_NOTICE, "- allocating memory for video");
	this->colorImg.allocate(CAM_W, CAM_H);
	this->greyImg.allocate(CAM_W, CAM_H);
	this->greyLastImg.allocate(CAM_W, CAM_H);
	this->greyBg.allocate(CAM_W, CAM_H);
	this->greyDiff.allocate(CAM_W, CAM_H);

	this->VF.setupField(60, 40, ofGetWidth(), ofGetHeight());

	#ifdef _WEBCAM_INPUT
		ofLog(OF_LOG_NOTICE, "- using webcam for input");
		//vidGrabber.setVerbose(true);
		//vidGrabber.listDevices();
		vidGrabber.setDeviceID(WEBCAM_ID);
		vidGrabber.initGrabber(CAM_W, CAM_H);
	#else
		ofLog(OF_LOG_NOTICE, "- using local video file for input");
		vidPlayer.loadMovie("fingers.mov");
		vidPlayer.play();
	#endif
}

FrameCaptureThread::~FrameCaptureThread(void) {}

void FrameCaptureThread::threadedFunction() {
		
	while(isThreadRunning()) {

		bool bNewFrame = false;

	 /* =============================================================================
		Frame Processing
		========================================================================== */
		#ifdef _WEBCAM_INPUT
			vidGrabber.grabFrame();
			bNewFrame = vidGrabber.isFrameNew();
		#else
			vidPlayer.idleMovie();
			bNewFrame = vidPlayer.isFrameNew();
		#endif

		// Check if new
		if(bNewFrame) {
			// Put the new frame in colorImg
			#ifdef _WEBCAM_INPUT
				colorImg.setFromPixels(vidGrabber.getPixels(), CAM_W, CAM_H);
			#else
				colorImg.setFromPixels(vidPlayer.getPixels(), CAM_W, CAM_H);
			#endif
			// Mirror incoming image
			colorImg.mirror(FLIP_V, FLIP_H);
			// Create greyscale version
			greyImg = colorImg;
			// Set the background to compare against
			if (bLearnBackground == true) {
				greyBg = greyImg;
				bLearnBackground = false;
			}
			// Isolate shapes from background
			greyDiff.absDiff(greyBg, greyImg);
			greyDiff.threshold(THRESHOLD);

			//ofLog(OF_LOG_VERBOSE, "Thresholding complete");

		/* =============================================================================
		   Blob Detection
		   ========================================================================== */
			lock();
				contourFinder.findContours(greyDiff, 500, CAM_AREA_DIV, 5, true);		
			unlock();

			//ofLog(OF_LOG_VERBOSE, "Blob detection complete");

		/* =============================================================================
		   Optical Flow
		   ========================================================================== */
			ofPushMatrix();
				lock();
					ofTranslate(screenPosition);
					flow.calc(&greyLastImg, &greyImg, &VF);		
				unlock();

				//ofLog(OF_LOG_VERBOSE, "Optical Flow calculation complete");
			ofPopMatrix();
			greyLastImg = greyImg;

		}
	}

}
