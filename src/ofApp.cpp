#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
    
    peace.load("peace.png");
    
    cam.initGrabber(1280, 720);
    flowSolver.setup(cam.getWidth()/2, cam.getHeight()/2, 0.5, 3, 10, 1, 7, 1.5, false, false);
    ofEnableAlphaBlending();
	ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    blur.setup(cam.getWidth(), cam.getHeight(), 4, .2, 4);

	
	tracker.setup();
	tracker.setRescale(.5);
    

}

void ofApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
		if(tracker.update(toCv(cam))) {
			classifier.classify(tracker);
            flowSolver.update(cam);
		}		
	}
    blur.setScale(ofMap(mouseX, 0, ofGetWidth(), 1, 10));
    blur.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));
}

void ofApp::draw() {
    
	ofSetColor(255);
    
//    blur.begin();
	cam.draw(0, 0);
	tracker.draw();
    flowSolver.drawColored(cam.getWidth(), cam.getHeight(), 10, 3);
    ofSetCircleResolution(64);
    ofCircle(mouseX, mouseY, 32);
//    blur.end();
//    
//    blur.draw();
	
	int w = 100, h = 12;
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(5, 10);
	int n = classifier.size();
	int primary = classifier.getPrimaryExpression();
  for(int i = 0; i < n; i++){
		ofSetColor(i == primary ? ofColor::red : ofColor::black);
		ofDrawRectangle(0, 0, w * classifier.getProbability(i) + .5, h);
		ofSetColor(255);
		ofDrawBitmapString(classifier.getDescription(i), 5, 9);
		ofTranslate(0, h + 5);
  }
	ofPopMatrix();
	ofPopStyle();
	
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), ofGetWidth() - 20, ofGetHeight() - 10);
	ofDrawBitmapStringHighlight(
		string() +
		"r - reset\n" +
		"e - add expression\n" +
		"a - add sample\n" +
		"s - save expressions\n"
		"l - load expressions",
		14, ofGetHeight() - 7 * 12);
}

void ofApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
		classifier.reset();
	}
	if(key == 'e') {
		classifier.addExpression();
	}
	if(key == 'a') {
		classifier.addSample(tracker);
	}
	if(key == 's') {
		classifier.save("expressions");
	}
	if(key == 'l') {
		classifier.load("expressions");
	}
}
