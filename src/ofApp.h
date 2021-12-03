#pragma once

#include "enemy.h"
//#include "ofMain.h"

#define SPEED (float)1.5
#define SOUNDS (int)5
#define ENEMIES (int)10

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void mousePressed(int x, int y, int button);
	void keyPressed(int key);
	void mouseMoved(int x, int y);
	void mouseReleased(int x, int y, int button);
	void borderCollie(ofVec2f pos, float &ang, float &angPrev, bool &xCol, bool &yCol);
	//void keyReleased(int key);
	//void mouseDragged(int x, int y, int button);
	//void mouseEntered(int x, int y);
	//void mouseExited(int x, int y);
	//void windowResized(int w, int h);
	//void dragEvent(ofDragInfo dragInfo);
	//void gotMessage(ofMessage msg);

	

	int scene;
	float timer;
	bool mouseRel;
	ofTrueTypeFont font1, font2;

	// Scene 0 
	void menuDraw();
	bool playHover;
	bool instructionHover;

	//SCENE
	ofFbo fbo;
	float scroll;
	ofPixels pix;
	ofSoundPlayer music;
	float scale;

	//PANEL
	int points; float energy; int level;

	//PLAYER
	ofVec2f pos;
	ofVec2f des;
	float ady;;
	float opp;
	float hyp;
	float ang;
	float angPrev;
	float rot;
	int color;
	float res;
	float dir;
	ofPath player;
	ofPoint cp[8];
	ofPoint p[4];
	float n;
	float a = 0, b = 0, c = 1;
	float curve;
	string shape;
	bool xCol, yCol;


	//ENEMY
	Enemy enemy[ENEMIES];
	ofSoundPlayer sound[SOUNDS];
};