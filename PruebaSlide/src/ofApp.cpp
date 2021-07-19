#include "ofApp.h"

//-----------------------------SETUP----------------------------------
void ofApp::setup(){
	//window
	ofSetFrameRate(120);
	ofSetVerticalSync(false);
	ofSetBackgroundColor(255);
	ofSetWindowShape(800, 600);

	//player
	pos = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);
	ofSetCircleResolution(100);
	rot = 0;
	ang = 0;

	//scene
	scroll = 0;
	fbo.allocate(ofGetWidth()*2, ofGetHeight(), GL_RGB,8);
	fbo.begin();
	ofClear(255,0);
	fbo.end();
}

//---------------------------UPDATE------------------------------------
void ofApp::update(){
	//player
	if (angPrev < ang) angPrev += .03;
	if (angPrev > ang) angPrev -= .03;
	pos.x += SPEED * cos(angPrev);
	pos.y += SPEED * sin(angPrev);
	rot+=1;
	if (ofGetFrameNum() % 3 == 0) {
		color == 0 ? color = 230 : color = 0;
	}
	rad = sin((float)ofGetFrameNum()/30) * 20 + 30;

	//scene
	scroll+=.5;
	if (scroll == ofGetWidth()) {
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
	ofSetColor(color);
	ofTranslate(pos);
	ofRotateDeg(rot);
	ofDrawCircle(0, 0, rad);
	fbo.end();
}

//----------------------------DRAW------------------------------------
void ofApp::draw(){
	// Show fps on window bar
	std::stringstream strm;
	strm << "fps: " << ofGetFrameRate();
	ofSetWindowTitle(strm.str());

	//player
	ofSetColor(255);
	fbo.draw(-scroll, 0);
	ofSetColor(0);
	ofTranslate(pos.x-scroll, pos.y);
	ofRotateDeg(rot);
	ofDrawCircle(0, 0, rad);
	ofSetColor(255);

}

//---------------------------INPUTS------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	des = ofVec2f(x + scroll, y);
	angPrev = ang;
	ady = des.x - pos.x;
	opp = des.y - pos.y;
	hyp = sqrt(ady * ady + opp * opp);

	ady > 0 ? ang = asin(opp / hyp) :
		ady < 0 && opp > 0 ? ang = PI - asin(opp / hyp) : ang = -PI - asin(opp / hyp);
}

//void ofApp::keyPressed(int key){
//
//}
//
//void ofApp::keyReleased(int key){
//
//}
//
//void ofApp::mouseMoved(int x, int y ){
//
//}
//
//void ofApp::mouseDragged(int x, int y, int button){
//
//}
//
//
//void ofApp::mouseReleased(int x, int y, int button){
//
//}
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
