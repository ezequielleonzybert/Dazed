#include "ofApp.h"

//-----------------------------SETUP----------------------------------
void ofApp::setup() {
	//window
	ofSetFrameRate(120);
	ofSetVerticalSync(false);
	ofSetBackgroundColor(255);
	ofSetWindowShape(1920/2, 1080/2);
	//ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
	//ofToggleFullscreen();
	scale = 1;

	// Menu
	scene = 0;

	// Player
	pos = ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);
	ofSetCircleResolution(30);
	rot = 0;
	ang = 0;
	player.setCurveResolution(30);
	n = 30;
	shape = "circle";
	ang = 0;
	angPrev = 0;
	xCol = false; yCol = false;

	// Scene
	scroll = 0;
	fbo.allocate(ofGetWidth() * 2, ofGetHeight(), GL_RGB, 8);
	fbo.begin();
	ofClear(255);
	fbo.end();
	music.load("audio/music.mp3");
	music.setLoop(true);

	// Enemy
	for (int i = 0; i < ENEMIES; i++) {
		enemy[i].setup();
	}
	sound[0].load("audio/levelUp1.wav");
	sound[1].load("audio/levelUp2.wav");
	sound[2].load("audio/levelUp3.wav");
	sound[3].load("audio/levelDown.wav");
	sound[4].load("audio/transform.wav");

	// Panel
	font1.load("fonts/font.ttf", 30);
	font2.load("fonts/font.ttf", 34);
	energy = 1;
	level = 0;
}

//---------------------------UPDATE------------------------------------
void ofApp::update() {
	if (scene == 1) {
		if (ofGetFrameNum() % 50 == 0) points++;

		// Player Position

		borderCollie(pos, ang, angPrev, xCol, yCol);

		if (angPrev < ang) { 
			angPrev += .03; 
			if (angPrev > PI) {
				float delta = angPrev - PI;
				angPrev = -PI + delta;
				ang -= 2 * PI;
			}
		}
		if (angPrev > ang) {
			angPrev -= .03;
			if (angPrev < -PI) {
				float delta = angPrev + PI;
				angPrev = PI - delta;
				ang += 2 * PI;
			}
		}

		pos.x += SPEED * cos(angPrev);
		pos.y += SPEED * sin(angPrev);
		rot += 1;
		if (ofGetFrameNum() % 3 == 0) {
			color == 10 ? color = 230 : color = 10;
		}

		// Player Shape
		if (shape == "circle") {
			if (curve < 4 * (sqrt(2) - 1) / 3) curve += .005;
			if (p[0].y > 0) a -= .3;
			if (p[1].y > -n) b -= .3;
			if (p[3].y < n) c += .3;
		}
		else {
			if (shape == "square") {
				if (curve > 0) curve -= .005;
				if (p[0].y > 0) {
					a -= .3;
				}
				if (p[1].y > -n) {
					b -= .3;
				}
				if (p[3].y < n) {
					c += .3;
				}
			}
			else {
				if (shape == "triangle") {
					if (curve > 0) curve -= .005;
					if (p[0].y < n / sqrt(3)) {
						a += .3;
					}
					if (p[1].y < -2 * n / sqrt(3)) {
						b += .3;
					}
					if (p[3].y > n / sqrt(3)) {
						c -= .3;
					}
				}
			}
		}

		// Player Vertices
		p[0] = { -n,a,0 };
		p[1] = { 0,-n + b,0 };
		p[2] = { n,a,0 };
		p[3] = { 0,n + c,0 };

		cp[0] = { -n,-n * curve + a, 0 };
		cp[1] = { -n * curve, -n + b, 0 };
		cp[2] = { n* curve,-n + b, 0 };
		cp[3] = { n,-n * curve + a, 0 };
		cp[4] = { n,n* curve + a, 0 };
		cp[5] = { n* curve,n + c, 0 };
		cp[6] = { -n * curve,n + c, 0 };
		cp[7] = { -n,n *curve + a, 0 };

		player.clear();
		player.lineTo(p[0]);
		player.bezierTo(cp[0], cp[1], p[1]);
		player.bezierTo(cp[2], cp[3], p[2]);
		player.bezierTo(cp[4], cp[5], p[3]);
		player.bezierTo(cp[6], cp[7], p[0]);
		player.close();


		// Scene
		scroll += .5;
		if (scroll == ofGetWidth()) {
			ofSetColor(255);
			ofPixels pix;
			ofImage img;
			fbo.readToPixels(pix);
			img.setFromPixels(pix);
			fbo.begin();
			ofClear(255);
			img.draw(-ofGetWidth(), 0);
			fbo.end();
			pos.x -= ofGetWidth();
			scroll = 0;
		}

		//fbo
		fbo.begin();
		ofTranslate(pos);
		ofRotateDeg(rot);
		player.setFillColor(color);
		ofScale(sin((float)ofGetFrameNum() / 30) / 1.8 + 1, sin((float)ofGetFrameNum() / 30) / 1.8 + 1, 0);
		player.draw();
		fbo.end();

		// Enemy
		for (int i = 0; i < ENEMIES; i++) {
			enemy[i].update(ofVec2f(pos.x - scroll, pos.y), sound, &points, &energy, &level, &shape);
		}

		// UI
		if (energy > 0) { 
			if (shape == "circle") energy -= .0002;
			if (shape == "triangle") energy -= .0004;
			if (shape == "square") energy -= .0008;
		}
		else { 
			scene = 2;
			timer = 2;
		}
	}
	if (scene == 2) {
		timer -= 1 / ofGetFrameRate();
	}
}

//----------------------------DRAW------------------------------------
void ofApp::draw() {
	if (scene == 0) {
		menuDraw();
	}
	if (scene == -1) {
		ofImage img;
		img.load("image/instructions.png");
		ofPushMatrix();
		ofScale(.5);
		img.draw(0, 0);
		ofPopMatrix();
	}
	if (scene == 1) {
		int w = ofGetWidth();
		int h = ofGetHeight();
		ofBackground(255);

		// Show fps on window bar
		std::stringstream strm;
		strm << "fps: " << ofGetFrameRate();
		ofSetWindowTitle(strm.str());

		//scale
		//scale != 1 ? ofScale(scale, scale, 1) : ofScale(scale, scale, 1);

		//player
		ofSetColor(255);
		fbo.draw(-scroll, 0);
		ofPushMatrix();
		ofTranslate(pos.x - scroll, pos.y);
		ofRotateDeg(rot);
		player.setFillColor(0);
		ofScale(sin((float)ofGetFrameNum() / 30) / 1.8 + 1, sin((float)ofGetFrameNum() / 30) / 1.8 + 1, 0);
		player.draw();
		ofPopMatrix();

		//enemy
		for (int i = 0; i < ENEMIES; i++) {
			enemy[i].draw();
		}

		// Panel
		ofSetColor(10);
		ofDrawRectangle(0, 0, w, 70);
		ofSetColor(230);
		ofDrawRectangle(w / 2 - 100, 15, 200, 40);
		ofDrawCircle(w * 6 / 7, 35, 20);
		ofDrawCircle(w * 6 / 7 + 35, 35, 20);
		ofDrawCircle(w * 6 / 7 + 70, 35, 20);
		ofSetColor(10);
		ofDrawRectangle(w / 2 - 90, 22.5, 180 * energy, 25);
		ofDrawCircle(w * 6 / 7, 35, 15 * ofClamp(level, 0, 1));
		ofDrawCircle(w * 6 / 7 + 35, 35, 15 * ofClamp(level - 1, 0, 1));
		ofDrawCircle(w * 6 / 7 + 70, 35, 15 * ofClamp(level - 2, 0, 1));
		ofSetColor(230);
		font1.drawString(to_string(points), 50, 50);
	}
	if (scene == 2) {
		ofBackground(10);
		ofSetColor(230);
		font1.drawString("SCORE:", 390, 220);
		font1.drawString(to_string(points), 540, 220);
		font1.drawString("TRY AGAIN", 390, 300);
	}
}

// --------------------------BORDER COLLISION ------------------------

void ofApp::borderCollie(ofVec2f pos, float &ang, float &angPrev, bool &xCol, bool &yCol) {
	
	if ((pos.x - scroll < 0 || pos.x - scroll > 960)) {
		if (!xCol) {
			if (angPrev > 0) {									//UP +
				if (angPrev < PI * 3 / 4 && angPrev > PI / 2) {
					angPrev = PI / 4;
					ang = angPrev;
				}
				else {
					if (angPrev < PI / 2 && pos.x - scroll < 0) angPrev = PI / 4;
					else {
						angPrev = -angPrev + PI;
						ang = angPrev;
					}
				}
			}
			else {												//DOWN -
				if (angPrev > -PI * 3 / 4 && angPrev < -PI / 2) {
					angPrev = -PI / 4;
					ang = angPrev;
				}
				else {
					if (angPrev > -PI / 2 && pos.x - scroll < 0) angPrev = -PI / 4;
					else {
						angPrev = -angPrev - PI;
						ang = angPrev;
					}					
				}
			}
			xCol = true;
		}
	}
	else {
		xCol = false;
	}

	if ((pos.y < 70 || pos.y > 540)) {
		if (!yCol) {
			angPrev = -angPrev;
			ang = angPrev;
			yCol = true;
		}
	}
	else {
		yCol = false;
	}
}

//---------------------------INPUTS------------------------------

void ofApp::mousePressed(int x, int y, int button) {

	if (scene == 0) {
		if (playHover) {
			scene = 1;
			music.play();
		}
		if (instructionHover) {
			scene = -1;
		}

		mouseRel = false;
	}

	if (scene == -1 && mouseRel == true) {
		scene = 0;
		mouseRel = false;
	}


	if (scene == 1) {
		des = ofVec2f(x / scale + scroll, y / scale);

		ang = atan2(des.y - pos.y, des.x - pos.x);

		float delta1, delta2;

		if ((delta1 = fabs(angPrev - ang)) > PI)
		{
			ang -= 2 * PI;

			if (delta2 = fabs(angPrev - ang) > delta1)
			{
				ang += 4 * PI;
			}
		}

		mouseRel = false;
	}

	if (scene == 2 && timer < 0) {
		shape = "circle";
		level = 0;
		points = 0;
		energy = 1;
		scroll = 0;
		fbo.begin();
		ofClear(255);
		fbo.end();
		pos.x = ofGetWindowWidth() / 2;
		pos.y = ofGetWindowHeight() / 2;
		scene = 1;

		mouseRel = false;
	}

}

void ofApp::mouseMoved(int x, int y ){
	if (scene == 0) {
		if (x > 426 && x < 510 && y > 395 && y < 430) {
			playHover = true;
		}
		else playHover = false;
		if (x > 360 && x < 580 && y > 470 && y < 500) {
			instructionHover = true;
		}
		else instructionHover = false;
	}
}

void ofApp::mouseReleased(int x, int y, int button) {
	mouseRel = true;
}

//-----------------------SCENES--------------------------------

void ofApp::menuDraw() {

	ofImage img;
	img.load("image/menu.png");
	ofPushMatrix();
	ofScale(.5);
	img.draw(0,0);
	ofPopMatrix();
	if (playHover) {
		font2.drawString("PLAY", 426, 432);
	}
	else {
		font1.drawString("PLAY", 430, 430);
	}
	if (instructionHover) {
		font2.drawString("INSTRUCTIONS", 352, 502);
	}
	else {
		font1.drawString("INSTRUCTIONS", 360, 500);
	}
}

void ofApp::keyPressed(int key) {
	//switch (key)
	//{
	///*case OF_KEY_TAB:
	//	if (ofGetWidth() == ofGetScreenWidth()) {
	//		ofToggleFullscreen();
	//		ofSetWindowShape(ofGetWidth()/2, ofGetHeight()/2);
	//		scale = .5;
	//	}
	//	else {
	//		ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
	//		ofToggleFullscreen();
	//		scale = 1;
	//	}
	//	break;*/
	//case '1':
	//	shape = "circle";
	//	break;
	//case '2':
	//	shape = "triangle";
	//	break;
	//case '3':
	//	shape = "square";
	//	break;
	//default:
	//	break;
	//}
}

//
//void ofApp::keyReleased(int key){
//
//}
//
//
//void ofApp::mouseDragged(int x, int y, int button){
//
//}
//
//

//
//void ofApp::mouseEntered(int x, int y){
//
//}
//
//void ofApp::mouseExited(int x, int y){
//
//}
//
//void ofApp::windowResized(int w, int h){
//
//}
//
//void ofApp::gotMessage(ofMessage msg){
//
//}
//
//void ofApp::dragEvent(ofDragInfo dragInfo){ 
//
//}

