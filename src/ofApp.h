#pragma once

#include "ofMain.h"
#include "ofxFaceTracker.h"
#include "ofxOpticalFlowFarneback.h"
#include "ofxBlur.h"
#include "FaceOsc.h"
//#include "ofxOsc.h"


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
    
    ofxOscReceiver	receiver;
    
    ofImage peace; // load image
    
    float mouthHeight;
    float mouthWidth;
    float eyeLeft;
    float eyeRight;
    float eyebrowLeft;
    float eyebrowRight;
    float jaw;
    float nostrils;
    
};
