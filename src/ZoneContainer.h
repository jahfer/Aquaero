#pragma once

#include "ofMain.h"
#include "Asset.h"

class ZoneContainer {
	public:
		ZoneContainer(ofVec3f pt);
		ZoneContainer(int x, int y, int z);
		~ZoneContainer(void);

		void loadAsset(char* filename, ofPoint* pos);
		void loadAsset(char* filename, int x, int y, int z);
		void draw(bool parallaxDir);

		vector <Asset*> assets;
		ofPoint* pos;

		// Shaders
		ofShader shader;
		ofFbo fbo;
};

