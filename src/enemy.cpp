#include "enemy.h"

#define CIRCLERESOL (int)30

void Enemy::setup(){
	
	//Shape setup
	int r = rand() % 3;
	switch (r)
	{
	case 0:
		sides = CIRCLERESOL;
		shape = "circle";
		break;
	case 1:
		sides = 3;
		shape = "triangle";
		break;
	case 2:
		sides = 4;
		shape = "square";
		break;
	default:
		break;
	}

	//Pos setup (out of screen)
	int h = ofGetHeight();
	int w = ofGetWidth();
	int ho = h + 200;
	int wo = w + 200;

	int rh = rand() % ho - 100;
	int rw = rand() % wo - 100;

	pos.x = rw;

	if (rw > w || rw < 0) {
		pos.y = rh;
	}
	else pos.y = ((rand() % 2) * ho) - 100;


	//Angle setup
	angle = (rand() % 181) * ((rand() % 2) * 2 - 1); 

	//Speed
	speed = rand() % 3 + 1;

	//Rotation
	rot = 0;
	rotSpeed = (rand() % 2) * 2 - 1;

	//Collide
	collide = false;
	opacity = 255;
	scale = 0;



}

void Enemy::update(ofVec2f playerPos, ofSoundPlayer sound[], int * points, float * energy, int * level, string * playerShape) {
	int h = ofGetHeight() + 100;
	int w = ofGetWidth() + 100;
	//pos update
	if (pos.x > w || pos.x < -100 || pos.y > h || pos.y < -100) setup();
	else {
		pos.x += speed * cos(angle);
		pos.y += speed * sin(angle);
	}

	//Rot update
	rot += rotSpeed;

	//Collision
	if ((playerPos.x < pos.x + 30 && playerPos.x > pos.x - 30) && (playerPos.y < pos.y + 30 && playerPos.y > pos.y - 30) && !collide) {
		
		collide = true;
		if (*playerShape == "square") {
			sound[*level].play();
			*energy = ofClamp(*energy + .2, 0, 1);
			*points += 500;
			if (*level < 2) (*level)++;
		}
		else{
			if (*playerShape == "triangle") {
				if (shape == "square") {
					sound[3].play();
					*energy = ofClamp(*energy - .2, 0, 1);
					*points -= 500;
					if (*level < 1) {
						*level = 3;
						*playerShape = "circle";
					}
					else (*level)--;
				}
				else {
					*energy = ofClamp(*energy + .2, 0, 1);
					if (*level > 2) { 
						*playerShape = "square"; 
						*level = 0;
						sound[4].play();
					}
					else {
						sound[*level].play();
						(*level)++;
					}
					if (shape == "triangle") {
						*points += 200;
					}
					else {
						points += 100;
					}
				}
			}
			else {
				if (shape == "circle") {
					*energy = ofClamp(*energy + .2, 0, 1);
					*points += 100;
					if (*level > 2) {
						*playerShape = "triangle";
						*level = 0;
						sound[4].play();
					}
					else { 
						sound[*level].play();
						(*level)++; 
					}
				}

				else{
					sound[3].play();
					*energy = ofClamp(*energy - .2, 0, 1);
					if (*level > 0) (*level)--;
					if (shape == "square") *points -= 500;
					if (shape == "triangle") *points -= 200;
				}
			}
		}
	}
	

	if (collide) {
		opacity -= 4;
		scale += .5;
	}
}

void Enemy::draw() {

	ofPushMatrix();
	ofTranslate(pos);
	ofRotateDeg(rot);
	ofSetColor(0, opacity);
	ofSetCircleResolution(sides);
	switch (sides)
	{
	case CIRCLERESOL:
		ofDrawCircle(0, 0, 35+scale);
		break;
	case 3:
		ofDrawCircle(0, 0, 38+scale);
		break;
	case 4:
		ofDrawCircle(0, 0, 36+scale);
		break;
	default:
		break;
	}
	ofSetColor(230, opacity);
	ofSetCircleResolution(sides);
	ofDrawCircle(0,0, 30+scale);
	ofPopMatrix();
	ofSetCircleResolution(CIRCLERESOL);

}