#pragma once

// openFrameworks Add-Ons
#include "ofxOpenCv.h"
#include "ofxCvMain.h"
#include "ofxBox2d.h"
#include "ofxGrabCam.h"

// External Classes
#include "vectorField.h"

// Project Classes
#include "Creature.h"
#include "OpticalFlow.h"
#include "BlobAnalyzer.h"
#include "ZoneContainer.h"
#include "EnviroState.h"
#include "Flocking.h"
//#include "Stars.h"

#ifdef _THREADING
	#include "FrameCaptureThread.h"
#endif

#include "ofMain.h"
#include "Defines.h"
#include <time.h>


class testApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		// Custom Functions
		void loadAssets();
		ENV_ZONE getCurrentZone();
		void contactStart(ofxBox2dContactArgs &e);
		void exit();
		void audioIn(float* input, int bufferSize, int nChannels);
		void creatureSpawn(Creature* &c);
		void zoneChange(ENV_ZONE &zone);
		void audioSpike(float &vol);
		void musicPlayer();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		#ifdef _WEBCAM_INPUT
			ofVideoGrabber vidGrabber;
		#else
			ofVideoPlayer vidPlayer;
		#endif

		bool bStartApp;
		bool bSkipIntro;
		ofVideoPlayer introVid;

		// Graphical Assets
		vector <ZoneContainer*> zones;
		EnviroState env;
		bool parallaxDir;
		float bgLerp;
		ofColor bgDefault;
		ofColor bgRain;
		ENV_ZONE zone;
		ofEvent<ENV_ZONE> onZoneChange;

		// Shaders
		ofShader shader;
		ofFbo fbo, sceneFbo;
		ofImage pattern, vignette;

		// Camera
		ofxGrabCam cam;
		ofPoint screenPosition, camPosition;
		bool reverseScroll;

		// Video Processing
#ifndef _THREADING
		ofxCvColorImage	colorImg;
		ofxCvGrayscaleImage greyImg;
		ofxCvGrayscaleImage greyLastImg;
		ofxCvGrayscaleImage greyBg;
		ofxCvGrayscaleImage greyDiff;
		ofxCvContourFinder contourFinder;

		// Optical Flow
		OpticalFlow flow;
		vectorField VF;
#else		
		FrameCaptureThread capture;
#endif

		// Blob Detection
		int threshold;
		bool bLearnBackground;
		BlobAnalyzer blobAnalyzer;
		vector<Creature> creatures;
		ofPolyline handShape, handSmoothed;
		ofPath handRenderer;
		unsigned short int creatureID;

		// Box2d Physics
		ofxBox2d box2d;

		Flocking flock;
		//Stars stars;

		// General Audio
		ofEvent<float> onAudioSpike;
		vector<float> left;
		vector<float> right;
		vector <float> volHistory;				
		int bufferCounter;
		int drawCounter;		
		float smoothedVol;
		float scaledVol;
		float curVol;
		float pastVol;		
		float audioEvent;
		//If an increase is detected it must be above this value
		float thresholdValue;
		//how long we wait before getting another audio level and comparing the two
		float volTimeDelay;
		//the minimum level change required to register the 
		float volChangeMin;
		float volDiff;

		float audioInterp[6];

		// Audio Loops
		ofSoundPlayer	SP_Drum;
		ofSoundPlayer	SP_Guitar;
		ofSoundPlayer	SP_Piano;
		ofSoundPlayer	SP_Piano2;
		ofSoundPlayer	SP_SteelDrum;
		ofSoundPlayer	SP_Space;
		ofSoundPlayer	SP_Underwater;
		
		// Audio SFX
		ofSoundPlayer	SP_Birds;
		ofSoundPlayer	SP_Bubbles;
		ofSoundPlayer	SP_Oscillation;
		ofSoundPlayer	SP_Rain;
		ofSoundPlayer	SP_Spacey;
		ofSoundPlayer	SP_Thunder;
		ofSoundPlayer	SP_Wind;
				
		ofSoundStream soundStream;

};
