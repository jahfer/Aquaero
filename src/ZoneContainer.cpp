#include "ZoneContainer.h"


ZoneContainer::ZoneContainer(int x, int y, int z) {
	this->pos = new ofPoint(x, y, z);
}
ZoneContainer::ZoneContainer(ofVec3f pt) {
	this->pos = &pt;
}


void ZoneContainer::loadAsset(char* filename, ofPoint* pos) {
	
	Asset* asset = new Asset(filename);
	asset->setPosition(pos);

	assets.push_back(asset);

}
void ZoneContainer::loadAsset(char* filename, int x, int y, int z) {
	
	ofPoint* pos = new ofPoint(x, y, z);	

	Asset* asset = new Asset(filename);
	asset->setPosition(pos);

	assets.push_back(asset);
}


void ZoneContainer::draw(bool parallaxDir) {
	

	ofPushMatrix();
		ofTranslate(*this->pos);
		
		for (int i=0; i < assets.size(); i++) {
			assets[i]->image.getTextureReference().setTextureMinMagFilter( GL_NEAREST, GL_NEAREST_MIPMAP_LINEAR );
			assets[i]->draw(parallaxDir);
		}

	ofPopMatrix();

}


ZoneContainer::~ZoneContainer(void) {}
