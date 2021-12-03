#pragma once
#include "ofMain.h"

class Enemy{
public:
	int sides;
	string shape;
	ofVec2f pos;
	float rot;
	float rotSpeed;
	float speed;
	float angle;
	bool collide;
	float opacity;
	float scale;

	void setup();
	void update(ofVec2f playerPos, ofSoundPlayer sound[], int * points, float * energy, int  * level, string * playerShape);
	void draw();

};