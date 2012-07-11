#include "Boid.h"

Boid::Boid(ofVec2f l, float ms, float mf){
	acc.set(0,0);
	vel.set(ofRandom(-1,1), ofRandom(-1,1));
	loc.set(l);
	radius = 50.0;

	maxSpeed = ms;
	maxForce = mf;

	pause = false;
	bIsJellyfish = false;
	spawnJellys = true;
	animPos = 0;
}

Boid::~Boid(void) {}

void Boid::init() {
	int num = ofRandom(4);
	this->img = &this->images[num];
}

void Boid::run(vector<Boid>boids){
	flock(boids);
	update();
	borders();
	render();

	//Boid::zone = env;
}

void Boid::flock(vector<Boid>boids){
	ofVec2f sep = separate(boids);
	ofVec2f ali = align(boids);
	ofVec2f coh = cohesion(boids);

	sep *= 7.5;
	ali *= 6.0;
	coh *= 6.0;

	acc += sep;
	acc += ali;
	acc += coh;
}

void Boid::update(){

	if (bIsJellyfish) {
		this->img = &Boid::jelly[animPos++];
		if (animPos >= JELLY_FRAMES)
			animPos = 0;
	}	

	vel += acc;
	vel.limit(maxSpeed);
	loc += vel;
	acc *= 0;
}

void Boid::seek(ofVec2f target){
	acc += steer(target, false);
}

void Boid::arrive(ofVec2f target){
	acc += steer(target, true);
}

ofVec2f Boid::steer(ofVec2f target, bool slowdown){
	ofVec2f steer;
	ofVec2f desired = target - loc;
	float d = desired.length();

	if(d > 0){
		desired.normalize();
		if((slowdown) && (d < 100.0f)) desired *= maxSpeed * (d/100.0f);
		else desired *= maxSpeed;

		steer = target - (desired, vel);
		steer.limit(maxForce);
	} else {
		steer = ofVec2f(0,0);
	}
	return steer;
}

void Boid::render(){
	// limit turning radius to 30 degrees
	float theta = min(ofRadToDeg(heading2D(vel)), 30.0f);
	theta = max(theta, -30.0f);
	
	ofPushMatrix();
		// translate to center of boid		
        ofTranslate(loc);    
        ofRotate(theta);
		ofScale(0.60, 0.60);
		ofSetColor(255);

		img->draw(0, 0); 
    ofPopMatrix();
}

void Boid::borders(){
	
	if (pause) return;

	bool swapImage = false;

	if(loc.x < -radius) { 
		swapImage = true;
		loc.x  = ofGetWidth() + radius;
	}

	if(loc.y < -radius) { 
		swapImage = true;
		loc.y = ofGetHeight() + radius;
	}

	if(loc.x > ofGetWidth()+radius) { 
		swapImage = true;
		loc.x = -radius;
	}

	if(loc.y > ofGetHeight()+radius) { 
		swapImage = true;
		loc.y = -radius; 
	}

	if (swapImage) {
		if (spawnJellys) {
			bIsJellyfish = true;
		} else {
			bIsJellyfish = false;
			int num = ofRandom(4);
			this->img = &this->images[num];
		}
	}		

}

ofVec2f Boid::separate(vector<Boid>boids){
	float desiredseparation = 50.0f; // min distance between boids before moving
	ofVec2f steer(0,0);
	int count = 0;

	for(int i = 0; i < boids.size(); i++){
		Boid other = (Boid) boids.at(i);
		float d = loc.distance(other.loc); // ofVec2f

		if((d > 0) && (d < desiredseparation)){
			ofVec2f diff =  loc - other.loc;
			diff.normalize();
			diff /= d;
			steer += diff;
			count++;
		}
	}

	if(count > 0){
		steer /= float(count);
	}

	if(steer.length() > 0){
		steer.normalize();
		steer *= maxSpeed;
		steer -= vel;
		steer.limit(maxForce);
	}
	return steer;

}

ofVec2f Boid::align(vector<Boid>boids){
	float neighbourlist = 50.0;
	ofVec2f steer(0,0);
	int count = 0;
	
	for(int i = 0; i < boids.size(); i++){
		Boid other = (Boid) boids.at(i);
		float d = loc.distance(other.loc);
		if((d > 0) && (d < neighbourlist)){
			steer += other.vel;
			count++;
		}
	}
	if(count > 0){
		steer /= float(count);
	}

	if(steer.length() > 0){
		steer.normalize();
		steer *= maxSpeed;
		steer -= vel;
		steer.limit(maxForce);
	}
	return steer;
}

ofVec2f Boid::cohesion(vector<Boid>boids){
	float neighbourlist = 50.0;
	ofVec2f sum(0,0);
	int count = 0;

	for(int i = 0; i < boids.size(); i++){
		Boid other = (Boid) boids.at(i);
		float d = loc.distance(other.loc);
		if((d > 0) && (d < neighbourlist)){
			sum += other.loc;
			count++;
		}
	}
	if(count > 0){
		sum /= float(count);
		return steer(sum, false);
	}
	return sum;
}

float Boid::heading2D(ofVec2f v){
   float angle = (float) atan2(-v.y, v.x);
   return -1*angle;
}