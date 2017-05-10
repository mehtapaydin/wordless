#pragma once

#include "ofMain.h"
#include "ofxFaceTracker.h"
#include "ofxOpticalFlowFarneback.h"
#include "ofxBlur.h"


class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	
	ofVideoGrabber cam;
	ofxFaceTracker tracker;
	ExpressionClassifier classifier;
    ofxOpticalFlowFarneback flowSolver;
    ofxBlur blur;

    ofImage text;
    ofImage justice;
    ofImage freedom;
    ofxCv::ObjectFinder finder;
    
    ofSoundPlayer player1;
    ofSoundPlayer player2;
    
};
