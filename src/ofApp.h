#pragma once

#include "ofMain.h"
#include "ofxFaceTracker.h"
#include "ofxOpticalFlowFarneback.h"
#include "ofxBlur.h"
#include "ofxCv.h"
#include "ofxOsc.h"
#include "Face.h"


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
    ofImage peace;
    ofImage justice;
    ofImage freedom;

    ofxCv::ObjectFinder finder;
    
    ofSoundPlayer player1;
    ofSoundPlayer player2;

    ofxOscReceiver receiver;
    Face face;

};
