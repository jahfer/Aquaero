#include "testApp.h"

// static assets for creatures
ofImage Creature::eyesI[19];
ofImage Creature::stripesI[8];
ofImage Creature::dotsI[8];
ofImage Creature::hornsI[17];
ofImage Creature::mouthsI[22];
ofImage Creature::nFlag[4];
ofImage Creature::nArms[4];

ofImage Boid::jelly[JELLY_FRAMES];

ENV_STATE Boid::zone;

//--------------------------------------------------------------------------------------------------**
//--------------------------------------------------------------------------------------------------**
void testApp::setup(){
//--------------------------------------------------------------------------------------------------**
//--------------------------------------------------------------------------------------------------**
	
	
 /* =============================================================================
	Initialize general settings
	========================================================================== */
	//ofSetLogLevel(OF_LOG_VERBOSE);	
	ofSetLogLevel(OF_LOG_NOTICE);
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	//ofSetFrameRate(FRAMERATE);
	ofSetVerticalSync(true);
	bLearnBackground = true;
	threshold = THRESHOLD;
	creatureID = 0;
	bStartApp = false;
	bSkipIntro = false;

#ifndef _THREADING
 /* =============================================================================
	Allocate memory for image frames
	========================================================================== */
	ofLog(OF_LOG_NOTICE, "- allocating memory for images");
	colorImg.allocate(CAM_W, CAM_H);
	greyImg.allocate(CAM_W, CAM_H);
	greyLastImg.allocate(CAM_W, CAM_H);
	greyBg.allocate(CAM_W, CAM_H);
	greyDiff.allocate(CAM_W, CAM_H);
	0

 /* =============================================================================
	Initialize vector field
	========================================================================== */
	ofLog(OF_LOG_NOTICE, "- setting up vector field");
	VF.setupField(60, 40, ofGetWidth(), ofGetHeight());
#endif
	
 /* =============================================================================
	Initialize Box2D
	========================================================================== */	
	ofLog(OF_LOG_NOTICE, "- initializing Box2d");
	box2d.init();
	box2d.setGravity(GRAVITY_X, GRAVITY_Y);
	box2d.createBounds();
	box2d.setFPS(FRAMERATE);
	ofAddListener(box2d.contactStartEvents, this, &testApp::contactStart);
	ofAddListener(onZoneChange, this, &testApp::zoneChange);
	ofAddListener(onAudioSpike, this, &testApp::audioSpike);
	ofAddListener(blobAnalyzer.onCreatureSpawn, this, &testApp::creatureSpawn);
	
 /* =============================================================================
	Initialize virtual camera
	========================================================================== */
	reverseScroll = false;
	camPosition.set(872, 800, 895);
	screenPosition.set(camPosition.x, 800, 0);
	
 /* =============================================================================
	Initalize graphical assets
	========================================================================== */
	ofLog(OF_LOG_NOTICE, "- loading graphics");
	loadAssets();
	bgLerp = 1.0f;
	bgDefault.set(255);
	bgRain.set(34, 47, 128); 
	env.init(&box2d);
	env.set(ENV_BASIC);
	
	// Load creature assets
	Creature::init();

	// Load jellyfish frames
	for (int i=0; i<9; i++)
		Boid::jelly[i].loadImage("jelly/jelly_000"+ofToString(i+1)+".png");

	for (int i=9; i<=JELLY_FRAMES; i++)
		Boid::jelly[i].loadImage("jelly/jelly_00"+ofToString(i+1)+".png");

	// Shader
	shader.load("shaders/noise.vert", "shaders/noise.frag");
	fbo.allocate(1280, 800);
	sceneFbo.allocate(1280, 800);
	pattern.loadImage("texture05.jpg");
	vignette.loadImage("vignette_small.jpg");

	// Blob drawing
	handRenderer.setMode(ofPath::POLYLINES);
	handRenderer.setFillColor(ofColor(255, 255, 255, 75));

	
 /* =============================================================================
	Initialize Flocks
	========================================================================== */	
	
	ofSeedRandom();
	for(int i = 0; i < FLOCK_COUNT_MAX; i++){
		Boid b = Boid(ofRandom(ofGetWidth()/2, ofGetHeight()/2), 3.0, 0.005);
		flock.addBoid(&b);
	}


 /* =============================================================================
	Initialize Audio
	========================================================================== */	
	ofLog(OF_LOG_NOTICE, "- initializing audio");
	//soundStream.listDevices();

	int bufferSize = 256;
	audioEvent = 25;

	thresholdValue = 20;
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	volHistory.assign(400, 0.0);
	
	bufferCounter	= 0;
	drawCounter		= 0;
	smoothedVol     = 0.0f;
	scaledVol		= 0.0f;
	curVol			= 0.0f;

#ifdef MIC_ID
	//soundStream.setDeviceID(MIC_ID);
	//ofLog(OF_LOG_NOTICE, "- soundStream set up at device #%d", MIC_ID);
#endif

	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);

	//AUDIO LOOPS
	SP_Drum.loadSound("data/sounds/loops/drums.mp3");
	SP_Guitar.loadSound("data/sounds/loops/guitar.mp3");
	SP_Piano.loadSound("data/sounds/loops/piano.mp3");
	SP_Piano2.loadSound("data/sounds/loops/piano_2.mp3");
	SP_SteelDrum.loadSound("data/sounds/loops/steel_drum.mp3");
	SP_Space.loadSound("data/sounds/loops/space.mp3");
	SP_Underwater.loadSound("data/sounds/loops/underwater.mp3");


	//AUDIO LOOP VOLUME
	SP_Drum.setVolume(0.0f);
	SP_Guitar.setVolume(0.0f);
	SP_Piano.setVolume(0.0f);
	SP_Piano2.setVolume(0.0f);
	SP_SteelDrum.setVolume(0.0f);
	SP_Space.setVolume(0.0f);
	SP_Underwater.setVolume(0.0f);

	//AUDIO MUSIC ENABLE LOOP
	SP_Drum.setLoop(true);
	SP_Guitar.setLoop(true);
	SP_Piano.setLoop(true);
	SP_Piano2.setLoop(true);
	SP_SteelDrum.setLoop(true);
	SP_Space.setLoop(true);
	SP_Underwater.setLoop(true);

	//AUDIO INITIALIZE PLAYING
	SP_Drum.play();
	SP_Guitar.play();
	SP_Piano.play();
	SP_Piano2.play();
	SP_SteelDrum.play();
	SP_Space.play();
	SP_Underwater.play();
	
	//AUDIO SFX
	SP_Birds.loadSound("data/sounds/sfx/birds.mp3");
	SP_Bubbles.loadSound("data/sounds/sfx/bubbles.mp3");
	SP_Oscillation.loadSound("data/sounds/sfx/deep_oscillation.mp3");
	SP_Rain.loadSound("data/sounds/sfx/rain.mp3");
	SP_Spacey.loadSound("data/sounds/sfx/spacey_wind.mp3");
	SP_Thunder.loadSound("data/sounds/sfx/thunder.mp3");
	SP_Wind.loadSound("data/sounds/sfx/wind.mp3"); 

	//AUDIO FX LOOP
	SP_Birds.setLoop(true);
	SP_Wind.setLoop(true);
	SP_Bubbles.setLoop(false);
	SP_Oscillation.setLoop(false);
	SP_Rain.setLoop(false);
	SP_Spacey.setLoop(false);
	SP_Thunder.setLoop(false);

	//MULTIPLAY FX 
	SP_Thunder.setMultiPlay(true);

	//AUDIO FX VOLUME
	SP_Birds.setVolume(0.0f);
	SP_Bubbles.setVolume(0.75f);
	SP_Oscillation.setVolume(0.75f);
	SP_Rain.setVolume(0.75f);
	SP_Spacey.setVolume(0.75f);
	SP_Thunder.setVolume(0.75f);
	SP_Wind.setVolume(0.0f);

	//PLAY AUDIO FX
	SP_Birds.play();
	SP_Wind.play();

	// Fade in initial soundtrack
	audioInterp[0] = 0.0f;


 /* =============================================================================
	Initialize video for capture
	========================================================================== */
#ifndef _THREADING
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
#else
	capture.startThread(true, false);
#endif

	introVid.loadMovie("intro.mp4");
	introVid.setLoopState(OF_LOOP_NONE);
		
}

//--------------------------------------------------------------------------------------------------**
//--------------------------------------------------------------------------------------------------**
void testApp::update(){
//--------------------------------------------------------------------------------------------------**
//--------------------------------------------------------------------------------------------------**
	
	ofLogVerbose("Entered update()");

	if (bStartApp) {
		if (!introVid.isPlaying())
			introVid.play();

		if (!introVid.getIsMovieDone() && !bSkipIntro) {
			introVid.update();
			return;
		}
	} else
		return;


	// Reset settings
	ofBackground(100, 100, 100);
	bool bNewFrame = false;

	// Call audio
	ofSoundUpdate();
	musicPlayer();
	ofLogVerbose("Audio updated");

 /* =============================================================================
	Frame Processing
	========================================================================== */
#ifndef _THREADING
	#ifdef _WEBCAM_INPUT
		vidGrabber.grabFrame();
		bNewFrame = vidGrabber.isFrameNew();
	#else
		vidPlayer.idleMovie();
		bNewFrame = vidPlayer.isFrameNew();
	#endif
		
	ofLog(OF_LOG_VERBOSE, "Entered Update()");

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
		greyDiff.threshold(threshold);

		ofLog(OF_LOG_VERBOSE, "Thresholding complete");

	/* =============================================================================
	   Blob Detection
	   ========================================================================== */
		contourFinder.findContours(greyDiff, 1500, CAM_AREA_DIV, 5, true);			
		ofLog(OF_LOG_VERBOSE, "Blob detection complete");

	/* =============================================================================
	   Optical Flow
	   ========================================================================== */
		ofPushMatrix();
			ofTranslate(screenPosition);
			flow.calc(&greyLastImg, &greyImg, &VF);				
			ofLog(OF_LOG_VERBOSE, "Optical Flow calculation complete");
		ofPopMatrix();
		greyLastImg = greyImg;

	/* =============================================================================
	   Creature Creation
	   ========================================================================== */

		blobAnalyzer.read(&contourFinder, &creatures, &box2d);		

		// max capacity reached, so remove oldest object
		if (creatures.size() > CREATURE_COUNT_MAX)
			creatures.begin()->isDead = true;

		ofLog(OF_LOG_VERBOSE, "Blob analysis complete");
	}
#else
	
	ofLogVerbose("Reading contours");
	capture.lock();
		blobAnalyzer.read(&capture.contourFinder, &creatures, &box2d);		
	capture.unlock();
	ofLogVerbose("Contours found");

	// max capacity reached, so remove oldest object
	if (creatures.size() > CREATURE_COUNT_MAX) {
		ofLogVerbose("Marking oldest creature as dead");
		creatures.begin()->isDead = true;
		ofLogVerbose("Marked oldest creature as dead");
	}

#endif

	ofLog(OF_LOG_VERBOSE, "Blob analysis complete");

 /* =============================================================================
	Vector Field / Box2D
	========================================================================== */
#ifdef _THREADING
	capture.lock();
		capture.VF.fadeField(VF_FADE_AMOUNT);	
	capture.unlock();
#else
	VF.fadeField(VF_FADE_AMOUNT);	
#endif

	ofLog(OF_LOG_VERBOSE, "Box2d pre-update");
	box2d.update();	
	ofLog(OF_LOG_VERBOSE, "Box2d updated");

	for (vector<Creature>::iterator c = creatures.begin(); c != creatures.end(); ++c) {
		
		ofLog(OF_LOG_VERBOSE, "Entered creature loop");
#ifdef _THREADING
		capture.lock();
			ofxVec2f force = capture.VF.getForceFromPos(c->b2dProxy.getCenter().x, c->b2dProxy.getCenter().y);
		capture.unlock();
#else
		ofxVec2f force = VF.getForceFromPos(c->b2dProxy.getCenter().x, c->b2dProxy.getCenter().y);
#endif

		force.x *= 100;
		force.y *= 100;
		c->b2dProxy.addForce(force, 1);
	}

	
 /* =============================================================================
	Virtual Camera Scroll
	========================================================================== */
	if (!reverseScroll) {
		screenPosition.y -= SCROLL_SPEED;
		camPosition.y	 -= SCROLL_SPEED;

		if (screenPosition.y <= MAX_SCREEN_HEIGHT) {
			reverseScroll = true;
		}
	} else {
		screenPosition.y += SCROLL_SPEED;
		camPosition.y	 += SCROLL_SPEED;	

		if (screenPosition.y >= MIN_SCREEN_HEIGHT) {
			reverseScroll = false;
		}	
	}

#ifdef _THREADING
	capture.lock();
		capture.screenPosition = screenPosition;
	capture.unlock();
#endif

	
 /* =============================================================================
	Reactive Environmental Elements
	========================================================================== */	
	zone = getCurrentZone();
	env.update();
	
	ofLog(OF_LOG_VERBOSE, "Environment updated");

 /* =============================================================================
	Audio
	========================================================================== */
	//lets scale the vol up to a 0-1 range 
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
	//lets record the volume into an array
	volHistory.push_back( scaledVol );	
	//if we are bigger the the size we want to record - lets drop the oldest value
	if( volHistory.size() >= 400 ){
		volHistory.erase(volHistory.begin(), volHistory.begin()+1);
	}

	ofLog(OF_LOG_VERBOSE, "Exited Update()");
	
}

//--------------------------------------------------------------------------------------------------**
//--------------------------------------------------------------------------------------------------**
void testApp::draw() {
//--------------------------------------------------------------------------------------------------**
//--------------------------------------------------------------------------------------------------**
	

	if (bStartApp) {
		if (!introVid.getIsMovieDone() && !bSkipIntro) {
			introVid.draw(0, 0);
			return;
		}
	} else {
		ofBackground(0);
		return;
	}

	ofLog(OF_LOG_VERBOSE, "Entered Draw()");
	
	fbo.begin();
	ofClear(0, 0, 0, 1);

	/*glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/
	glDisable(GL_DEPTH_TEST);

	cam.begin();

	parallaxDir = Creature::isAverageOnLeft(&creatures);	
	/*if (parallaxDir)
		ofLogError("LEFT");
	else
		ofLogError("RIGHT");*/
	ofLog(OF_LOG_VERBOSE, "Average creature position calculated");

	// draw background shade
	if (!env.rain.isStopped) {
		ofSetColor(bgRain);
		if(bgLerp > 0.0f)			
			ofSetColor( bgRain.getLerped(bgDefault, bgLerp -= 0.01f) );
	} else if (bgLerp < 1.0f) {
		ofSetColor( bgRain.getLerped(bgDefault, bgLerp += 0.01f) );
	} else
		ofSetColor(bgDefault);
	
	// draw background
	for (int i = 0; i < zones.size(); i++)
		zones[i]->draw(parallaxDir);	

	// Draw camera view
	ofPushMatrix();
		cam.setGlobalPosition(camPosition);
		ofTranslate(screenPosition);
	
		// draw environmental elements
		ofPushMatrix();
			bool useChromaticAberration;
			ofTranslate(1, 1, DEPTH_ENV_REACTIVE);

			//stars.draw();

			if (!env.rain.isStopped && env.rain.isThunder)
				SP_Thunder.play();
			env.draw();	
		ofPopMatrix();

		ofPushMatrix();
			ofTranslate(1, 1, DEPTH_ENV);
			flock.run(getCurrentZone());
		ofPopMatrix();

		// draw video
		#ifndef _DEPLOYMENT
			ofPushMatrix();
				ofTranslate(1, 1, DEPTH_VIDEO);
				// Draw video frames
				ofSetHexColor(0xffffff);
			#ifdef _THREADING
				capture.lock();
				capture.colorImg.draw(0, 0);
				#ifdef _DRAW_ALL_VIDEOS
					capture.greyImg.draw(320, 0);
					capture.greyBg.draw(0, 240);
					capture.greyDiff.draw(320, 240);
				#endif
				capture.unlock();
			#else
				colorImg.draw(0, 0);
				#ifdef _DRAW_ALL_VIDEOS
					greyImg.draw(320, 0);
					greyBg.draw(0, 240);
					greyDiff.draw(320, 240);
				#endif
			#endif
				ofLog(OF_LOG_VERBOSE, "Videos drawn");
			ofPopMatrix();
		#endif
			
		ofPushMatrix();
			ofTranslate(1, 1, DEPTH_HANDS);		

			// draw hand outlines
			ofFill();

#ifdef _THREADING
			capture.lock();
				vector<ofxCvBlob> blobs = capture.contourFinder.blobs;
			capture.unlock();
#else
			vector<ofxCvBlob> blobs = contourFinder.blobs;
#endif

			for (int i=0; i < blobs.size(); i++) {
				handShape.addVertexes(blobs[i].pts);
				handShape.setClosed(true);
				handShape.simplify();
				handSmoothed = handShape.getSmoothed(4);
				// Convert smoothed shape from ofPolyLine to ofPath
				for(vector<ofPoint>::iterator j = handSmoothed.getVertices().begin(); j != handSmoothed.getVertices().end(); ++j) {
					j->x *= CAM_SCALE_X;
					j->y *= CAM_SCALE_Y;
					handRenderer.lineTo(*j);
				}

				//glCullFace(GL_BACK);
				handRenderer.draw();

				ofSetHexColor(0x000000);
				//glCullFace(GL_FRONT);	
				handShape.clear();
				handSmoothed.clear();
				handRenderer.clear();
			}
			ofLog(OF_LOG_VERBOSE, "Blobs drawn");
			
			//glCullFace(GL_FRONT);	
			// Print framerate
			ofSetHexColor(0xffffff);
#ifndef _DEPLOYMENT
			char reportStr[32];
			sprintf(reportStr, "%3.0f FPS", ofGetFrameRate());
			ofDrawBitmapString(reportStr, 10, 20);
#endif
		ofPopMatrix();
		
		// Draw creatures
		ofPushMatrix();
			ofTranslate(1, 1, DEPTH_CREATURES);			
			Creature::draw(&creatures);
			ofLog(OF_LOG_VERBOSE, "Creatures drawn");
		ofPopMatrix();

	// end camera view
	ofPopMatrix(); 
	cam.end();	
	fbo.end();

	// graphics cards have up to 8 texture units available, usually.		
	// we'll create another shader that receives the FBO as its input texture and we'll draw a full screen quad
	// this shader will blend the cardboard background with the FBO
	ofTexture baseTex = fbo.getTextureReference();

	sceneFbo.begin();
		shader.begin();	
			// bind screen to texture unit 1
			shader.setUniformTexture( "baseTex", baseTex, 0 );
			// bind pattern to texture unit 1
			shader.setUniformTexture( "bgTex", pattern, 1 );
			// bind vignette to texture unit 2
			shader.setUniformTexture( "vignetteTex", vignette, 2 );
			shader.setUniform2f( "uNoiseOffset", ofRandom(0.0f, (float)WIN_W),  ofRandom(0.0f, (float)WIN_W) );			
			shader.setUniform1i( "uUseAberration", (SP_Thunder.getIsPlaying() && !env.rain.isStopped) );
			shader.setUniform1f( "uAberrationOffset", ofRandom(-10.0f, 10.0f) );

			glBegin(GL_QUADS);  
				glTexCoord2f(0, 0);				glVertex2f(0, 0);  
				glTexCoord2f(0, WIN_H);			glVertex2f(0, WIN_H);   
				glTexCoord2f(WIN_W, WIN_H);		glVertex2f(WIN_W, WIN_H); 
				glTexCoord2f(WIN_W, 0);			glVertex2f(WIN_W, 0);  
			glEnd();  
		shader.end();
	sceneFbo.end();

	ofPushMatrix();
		sceneFbo.draw(0, 0, 1280, 800);
	ofPopMatrix();

	ofLog(OF_LOG_VERBOSE, "Exited Draw()");

}

//--------------------------------------------------------------
void testApp::exit() {
#ifdef _THREADING
	capture.stopThread();
#endif
}


//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){
	
	float curVol = 0.0;
	pastVol = 0.0;
	
	// samples are "interleaved"
	int numCounted = 0;	

	//lets go through each sample and calculate the root mean square which is a rough way to calculate volume	
	for (int i = 0; i < bufferSize; i++){
		left[i]		= input[i*2]*0.5;
		right[i]	= input[i*2+1]*0.5;

		curVol += left[i] * left[i];
		curVol += right[i] * right[i];

		//create curVol - x and assign to a new variable which can be used to compare increase and decrease of volumetrics
		numCounted += 2;
	}
	
	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;	
	// this is how we get the root of rms :) 
	curVol = sqrt( curVol );
	
	smoothedVol *= 0.93;
	smoothedVol += 0.07 * curVol;
	
	bufferCounter++;	
	
	if (thresholdValue < (curVol*100)) {
		curVol = curVol*100;
		ofNotifyEvent(onAudioSpike, curVol);
	}
}

//--------------------------------------------------------------
void testApp::musicPlayer(){
#ifdef _PLAY_MUSIC

	for (int i=1; i<4; i++) {
		audioInterp[i] = 1.0f;
	}

	// Deep Ocean <=> Coral Reef
	if (screenPosition.y < 0  && screenPosition.y >= -500) {
		audioInterp[1] = screenPosition.y / -500; // Coral Reef
		audioInterp[0] = 1.0f - audioInterp[1];	  // Deep Ocean
		
		SP_Space.setVolume(0.20f * audioInterp[0]);
		SP_Underwater.setVolume(0.80f * audioInterp[0]);
		SP_Wind.setVolume(0.2f * audioInterp[0]);

		SP_Piano.setVolume(0.4f * audioInterp[1]);
		SP_Piano2.setVolume(1.0f * audioInterp[1]);
		SP_SteelDrum.setVolume(0.70f * audioInterp[1]);

	// Coral <=> Mountain Range
	} else if (screenPosition.y < -1500 && screenPosition.y >= -2100) {
		audioInterp[2] = (screenPosition.y+1500) / -600;
		audioInterp[1] = 1.0f - audioInterp[2];
		
		//SP_Piano.setVolume(0.4f * audioInterp[1]);
		SP_Piano2.setVolume(1.0f * audioInterp[1]);
		SP_SteelDrum.setVolume(0.70f * audioInterp[1]);
		
		SP_Drum.setVolume(0.90f * audioInterp[2]);
		SP_Guitar.setVolume(1.00f * audioInterp[2]);
		SP_Piano.setVolume(1.00f * audioInterp[2]);
		//SP_Piano2.setVolume(0.20f * audioInterp[2]);
		SP_Birds.setVolume(0.7f * audioInterp[2]);
		SP_Wind.setVolume(0.0f * audioInterp[2]);

	// Mountains <=> Space
	} else if (screenPosition.y < -3100 && screenPosition.y >= -5000) {
		audioInterp[3] = (screenPosition.y+3100) / -1900;
		audioInterp[2] = 1.0f - audioInterp[3];
				
		SP_Drum.setVolume(0.70f * audioInterp[2]);
		SP_Guitar.setVolume(0.60f * audioInterp[2]);
		SP_Piano.setVolume(0.80f * audioInterp[2]);
		//SP_Piano2.setVolume(0.20f * audioInterp[2]);
		SP_Birds.setVolume(0.7f * audioInterp[2]);
				
		SP_Drum.setVolume(1.0f * audioInterp[3]);
		//SP_Piano.setVolume(1.0f * audioInterp[3]);
		SP_Piano2.setVolume(1.0f * audioInterp[3]);
		SP_Space.setVolume(0.5f * audioInterp[3]);
		SP_Wind.setVolume(0.4f * audioInterp[3]);

	} else {
		// not in transition stage
		switch (zone) {
			case DEEP_SEA:
				audioInterp[0] = min(audioInterp[0]+0.01f, 1.0f);
				SP_Drum.setVolume(0.0f);
				SP_Guitar.setVolume(0.0f);
				SP_Piano.setVolume(0.0f);
				SP_Piano2.setVolume(0.0f);
				SP_SteelDrum.setVolume(0.0f);
				SP_Space.setVolume(0.20f * audioInterp[0]);
				SP_Underwater.setVolume(0.80f * audioInterp[0]);

				SP_Birds.setVolume(0.0f);
				SP_Wind.setVolume(0.2f * audioInterp[0]);
				break;

			case CORAL_REEF:
				SP_Drum.setVolume(0.0f);
				SP_Guitar.setVolume(0.0f);
				SP_Piano.setVolume(0.4f * audioInterp[1]);
				SP_Piano2.setVolume(1.0f * audioInterp[1]);
				SP_SteelDrum.setVolume(0.70f * audioInterp[1]);
				SP_Space.setVolume(0.0f);
				SP_Underwater.setVolume(0.0f);

				SP_Birds.setVolume(0.0f);
				SP_Wind.setVolume(0.0f);
				break;

			case MOUNTAIN_RANGE:
				SP_Drum.setVolume(0.70f * audioInterp[2]);
				SP_Guitar.setVolume(0.60f * audioInterp[2]);
				SP_Piano.setVolume(0.80f * audioInterp[2]);
				SP_Piano2.setVolume(0.20f * audioInterp[2]);
				SP_SteelDrum.setVolume(0.0f * audioInterp[2]);
				SP_Space.setVolume(0.0f * audioInterp[2]);
				SP_Underwater.setVolume(0.0f * audioInterp[2]);

				SP_Birds.setVolume(0.7f * audioInterp[2]);
				SP_Wind.setVolume(0.0f * audioInterp[2]);
				break;

			case ATMOSPHERE:
				SP_Drum.setVolume(1.0f * audioInterp[3]);
				SP_Guitar.setVolume(0.0f * audioInterp[3]);
				SP_Piano.setVolume(1.0f * audioInterp[3]);
				SP_Piano2.setVolume(1.0f * audioInterp[3]);
				SP_SteelDrum.setVolume(0.0f * audioInterp[3]);
				SP_Space.setVolume(0.5f * audioInterp[3]);
				SP_Underwater.setVolume(0.0f * audioInterp[3]);

				SP_Birds.setVolume(0.0f * audioInterp[3]);
				SP_Wind.setVolume(0.4f * audioInterp[3]);
				break;

			case WACKY_SPACE:
				SP_Drum.setVolume(0.1f * audioInterp[3]);
				SP_Guitar.setVolume(0.0f * audioInterp[3]);
				SP_Piano.setVolume(0.0f * audioInterp[3]);
				SP_Piano2.setVolume(0.0f * audioInterp[3]);
				SP_SteelDrum.setVolume(0.0f * audioInterp[3]);
				SP_Space.setVolume(0.8f * audioInterp[3]);
				SP_Underwater.setVolume(0.0f * audioInterp[3]);
				SP_Birds.setVolume(0.0f * audioInterp[3]);
				SP_Wind.setVolume(0.2f * audioInterp[3]);
				break;
		}
	}
#endif
}

//--------------------------------------------------------------
void testApp::contactStart(ofxBox2dContactArgs &e) {
	
	if(e.a != NULL && e.b != NULL) {
		
		// bubble collision
		if (e.a->GetType() == b2Shape::e_circle) {			
			
			ofLog(OF_LOG_VERBOSE, "Collision Type A");

			// collision with creature
			// push it off-screen!
			if (e.b->GetType() == b2Shape::e_polygon) {
				unsigned short int* id = (unsigned short int*)e.b->GetBody()->GetUserData();
				if (id) {
					vector<Creature>::iterator c = find(creatures.begin(), creatures.end(), *id);

					c->b2dProxy.addForce(ofVec2f(0, -100), 1);
					e.b->SetSensor(true);
				}
			}

			// pop bubbles
			e.a->SetSensor(true);
			box2d.getWorld()->DestroyBody(e.a->GetBody());
			
		// bubble collision
		} else if (e.b->GetType() == b2Shape::e_circle) {	
			
			ofLog(OF_LOG_VERBOSE, "Collision Type B");

			// collision with creature
			if (e.a->GetType() == b2Shape::e_polygon) {
				
				unsigned short int* id = (unsigned short int*)e.a->GetBody()->GetUserData();
				if (id) {
					vector<Creature>::iterator c = find(creatures.begin(), creatures.end(), *id);

					c->b2dProxy.addForce(ofVec2f(0, -100), 1);
					e.a->SetSensor(true);
				}
			}

			// pop bubbles
			e.b->SetSensor(true);	
			box2d.getWorld()->DestroyBody(e.b->GetBody());
			
		}

		// Creature + Creature collision
		else if(e.a->GetType() == b2Shape::e_polygon && e.b->GetType() == b2Shape::e_polygon) {

			ofLog(OF_LOG_VERBOSE, "Collision Type C");

			unsigned short int* idA = (unsigned short int*)e.a->GetBody()->GetUserData();		
			unsigned short int* idB = (unsigned short int*)e.b->GetBody()->GetUserData();

			if (idA && idB) {
				vector<Creature>::iterator cA = find(creatures.begin(), creatures.end(), *idA);
				vector<Creature>::iterator cB = find(creatures.begin(), creatures.end(), *idB);

				if (cA->electrocuted && !cB->electrocuted) {
					cB->electrocuted = true;
				} else if (cB->electrocuted && !cA->electrocuted) {
					cA->electrocuted = true;
				}
			}
		}
	}

}

//--------------------------------------------------------------
ENV_ZONE testApp::getCurrentZone() {	
	ENV_ZONE curZone;

	if (screenPosition.y >= -500)
		curZone = DEEP_SEA;
	else if (screenPosition.y < -500  && screenPosition.y >= -2100)
		curZone = CORAL_REEF;
	else if (screenPosition.y < -2100 && screenPosition.y >= -3700)
		curZone = MOUNTAIN_RANGE;
	else if (screenPosition.y < -3700 && screenPosition.y >= -5300) 
		curZone = ATMOSPHERE;
	else
		curZone = WACKY_SPACE;

	if (curZone != zone)
		ofNotifyEvent(onZoneChange, curZone);

	return curZone;
}

void testApp::creatureSpawn(Creature* &c) {
	ofLog(OF_LOG_VERBOSE, "Entered creatureSpawn()");
	// For access to creature from b2d object
	c->ID = creatureID;
	c->b2dProxy.setData(new unsigned short int(creatureID));

	ofLog(OF_LOG_NOTICE, "- creature #%d spawned at address %p", c->ID, c);
	creatures.push_back(*c);

	ofLog(OF_LOG_VERBOSE, "Pushed creature to vector");
	creatureID++;

	ofLog(OF_LOG_VERBOSE, "Exited creatureSpawn()");
}

//--------------------------------------------------------------
// called via event listener
void testApp::zoneChange(ENV_ZONE &zone) {
	ofLog(OF_LOG_NOTICE, "- zone changed to %d", zone+1);

	// cue music transition
}

//--------------------------------------------------------------
// called via event listener
void testApp::audioSpike(float &vol) {
	ofLog(OF_LOG_NOTICE, "- volume changed to %2.2f", vol);

	switch (zone) {
		case DEEP_SEA:
			if (env.vents.isStopped)  {
				env.set(ENV_VENTS);	
				SP_Bubbles.play();
			}
			break;

		case CORAL_REEF:
			if (env.bubbles.isStopped) {
				env.set(ENV_BUBBLES);
				SP_Bubbles.play();
			}
			break;

		case MOUNTAIN_RANGE:
			if (env.rain.isStopped) {
				env.set(ENV_STORM);
				SP_Rain.play();
				SP_Birds.setVolume(0.0f);
			} else {
				SP_Birds.setVolume(0.7f);
			}
			// give the lightning something to strike
			if (creatures.size()) {
				vector<Creature>::iterator c = find(creatures.begin(), creatures.end(), (int)ofRandom(creatureID-1));
				//env.rain.target = &creatures.at( (int)ofRandom(creatures.size()) );
				env.rain.target = c;
			}
			break;

		case ATMOSPHERE:
			env.set(ENV_METEOR);
			break;

		case WACKY_SPACE:
			env.set(ENV_COMET);
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch (key) {
		case ' ':
#ifdef _THREADING
			capture.lock();
				capture.bLearnBackground = true;
			capture.unlock();
#else
			bLearnBackground = true;
#endif
			break;

		case '+':
			min(++threshold, 255);
			break;

		case '-':
			max(--threshold, 0);
			break;

		case 's':
			soundStream.start();
			break;

		case 'e':
			soundStream.stop();
			break;

		case '9':
			thresholdValue --;
			ofLog(OF_LOG_NOTICE, "Audio threshold lowered to %2.0f", thresholdValue);
			break;
		
		case '0':
			thresholdValue ++;
			ofLog(OF_LOG_NOTICE, "Audio threshold raised to %2.0f", thresholdValue);
			break;

		case 'f':
			ofToggleFullscreen();
			break;

		case '`':
			bStartApp = true;
			break;

		case 'i':
			bSkipIntro = true;
			break;

	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::loadAssets() {

	// Zone 1
	ZoneContainer* deepSea = new ZoneContainer(0, 0, 0);
	deepSea->loadAsset("zones/level_1_4.png", 0, 0, -4);
	deepSea->loadAsset("zones/level_1_3.png", 0, 0, -3);
	deepSea->loadAsset("zones/level_1_2.png", 0, 0, -2);
	deepSea->loadAsset("zones/level_1_1.png", 0, 0, -1);
	zones.push_back(deepSea);

	// Zone 2
	ZoneContainer* coralReef = new ZoneContainer(0, -1600, 0);
	coralReef->loadAsset("zones/level_2_4.png", 0, 0, -7);
	coralReef->loadAsset("zones/level_2_3.png", 0, 0, -6);
	coralReef->loadAsset("zones/level_2_2.png", 0, 0, -5);
	coralReef->loadAsset("zones/level_2_1.png", 0, 0, -4);
	zones.push_back(coralReef);

	// Zone 3
	ZoneContainer* mountainRange = new ZoneContainer(0, -3200, 0);
	mountainRange->loadAsset("zones/level_3_4.png", 0, 0, -10); 
	mountainRange->loadAsset("zones/level_3_3.png", 0, 0, -9); 
	mountainRange->loadAsset("zones/level_3_2.png", 0, 0, -8); 
	mountainRange->loadAsset("zones/level_3_1.png", 0, 0, -7); 
	zones.push_back(mountainRange);

	// Zone 4
	ZoneContainer* atmosphere = new ZoneContainer(0, -4800, 0);
	atmosphere->loadAsset("zones/level_4_3.png", 0, 0, -12); 
	atmosphere->loadAsset("zones/level_4_2.png", 0, 0, -11); 
	atmosphere->loadAsset("zones/level_4_1.png", 0, 0, -10); 
	zones.push_back(atmosphere);

	// Zone 5
	ZoneContainer* wackySpace = new ZoneContainer(0, -6400, 0);
	wackySpace->loadAsset("zones/level_5_3.png", 0, 0, -8); 
	wackySpace->loadAsset("zones/level_5_2.png", 0, 0, -6); 
	wackySpace->loadAsset("zones/level_5_1.png", 0, 0, -3);
	zones.push_back(wackySpace);

}