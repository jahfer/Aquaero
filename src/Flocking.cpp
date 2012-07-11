#include "Flocking.h"
// Flock
Flocking::Flocking(void){
	for (int j=0; j<4; j++)
		images[DEEP_SEA][j].loadImage("fish/fish_0"+ofToString(j+1)+".png");	
	for (int j=0; j<4; j++) 
		images[CORAL_REEF][j].loadImage("fish/fish_0"+ofToString(j+1)+".png");
	for (int j=0; j<4; j++) 
		images[MOUNTAIN_RANGE][j].loadImage("birds/bird_0"+ofToString(j+1)+".png");
	for (int j=0; j<4; j++) 
		images[ATMOSPHERE][j].loadImage("birds/bird_0"+ofToString(j+1)+".png");		
	for (int j=0; j<4; j++) 
		images[WACKY_SPACE][j].loadImage("birds/bird_0"+ofToString(j+1)+".png");
	
}

Flocking::~Flocking(void){
}

void Flocking::run(ENV_ZONE zone){

	bool bPause =  (zone == ATMOSPHERE || zone == WACKY_SPACE);

	for (int i = 0 ; i < boids.size(); i++) {
		boids[i].pause = bPause;
		if (zone == DEEP_SEA)
			boids[i].spawnJellys = true;
		else
			boids[i].spawnJellys = false;

		boids[i].run(boids);
		boids[i].images = images[zone];
	}
}

void Flocking::addBoid(Boid* b) {
	b->images = images[DEEP_SEA];
	b->init();
	boids.push_back(*b);
}