#include "Asset.h"


Asset::Asset(char* file) {
	this->image.loadImage(file);
	this->image.draw(0, 0);
}

void Asset::setPosition(ofPoint* iPos) {
	this->pos = iPos;
	parallax = abs(1/this->pos->z);
}

Asset::~Asset(void) {}

void Asset::draw(bool moveLeft) {

	this->image.draw(this->pos->x, this->pos->y, this->pos->z);
	
	if(moveLeft && this->pos->x < 872) 
		this->pos->x += parallax;

	else if(!moveLeft && this->pos->x > -872)
		this->pos->x -= parallax;
}