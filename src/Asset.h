#pragma once
#include "ofMain.h"
#include "Defines.h"

class Asset {

	public:
		Asset(char* file);
		~Asset(void);
		void draw(bool moveLeft);
		void setPosition(ofPoint* iPos);

		ofImage image;
		ofPoint* pos;

		float parallax;

};

