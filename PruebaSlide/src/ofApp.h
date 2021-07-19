#pragma once

#include "ofMain.h"
#define SPEED (float)1.5

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void mousePressed(int x, int y, int button);
		
		//void keyPressed(int key);
		//void keyReleased(int key);
		//void mouseMoved(int x, int y );
		//void mouseDragged(int x, int y, int button);
		//void mouseReleased(int x, int y, int button);
		//void mouseEntered(int x, int y);
		//void mouseExited(int x, int y);
		//void windowResized(int w, int h);
		//void dragEvent(ofDragInfo dragInfo);
		//void gotMessage(ofMessage msg);

		//SCENE
		ofFbo fbo;
		float scroll;
		ofPixels pix;

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
		float rad;
		float res;
		float dir;
};
