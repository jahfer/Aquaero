# Aquaero - Interactive Art Experience
## Introduction
Aquaero is an interactive art experience that allows users to create creatures using their hands and other objects. Once these generated creatures have been made, they can be moved around by swiping across the table.

[Video Walkthrough »](http://vimeo.com/40615509)

## About
The creatures interact with the environment through audio events triggered by the audience, such as bubbles or lightning strikes. Flocking creatures such as jellyfish, fish and birds can be found in levels and add to the interaction. Audio and Environments transition seamlessly as the characters move around. 

This project was created for Design Studio 3 at Algonquin College, by Russell Baylis, Stacie Ha, Jahfer Husain, and Kyle Thompson. Special thanks to James Acres, Alan Brown, Hala Al-Jaber and Michelle Labonte.

## The Code
The project was written in C++ using OpenFrameworks with a number of add-ons, including:
- [ofxBox2d](https://github.com/vanderlin/ofxBox2d/)
- [Optical Flow]()
- [Vector Field](http://teaching.thesystemis.com/classes/algo/?p=39)
- [ofxGrabCam](https://github.com/elliotwoods/ofxGrabCam/)
- [ofxLightning](https://github.com/companje/ofxLightning)

The project utilized a large number of OpenFrameworks classes, including:
- Threading 
	- `ofThread`
- Shaders
	- `ofShader`
- OpenCV
	- `ofxCvColorImage`
	- `ofxCvGrayscaleImage`
	- `ofxCvContourFinder`
	- `cv::FAST`
	- `cv::calcOpticalFlowPyrLK` 
- Paths/Polylines
	- `ofPath`
	- `ofPolyline`
- FBOs and VBOs
	- `ofFbo`
	- `ofVbo`
- Custom Event Handlers
	- `ofEvent`
- Video Player
	- `ofVideoPlayer`
- Audio
	- `ofSoundPlayer`
	- `ofSoundStream`

## The Team
- *[Russell Baylis](http://rbaylis.com):* Audio Recording/Programming, Environment Design
- *[Stacie Ha](http://stacieha.com):* Asset Design, Flock Programming, Documentation
- *[Jahfer Husain](http://jahfer.com):* Backend Development 
- *[Kyle Thompson](http://kylethompson.ca):* Creature Programming