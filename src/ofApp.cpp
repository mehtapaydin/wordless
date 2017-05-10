#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

ofImage img;

void ofApp::setup() {
 
//    peace.load("peace.png");
//    justice.load("justice.png");
//    freedom.load("freedom.png");
    
    cam.initGrabber(1280, 720);
    flowSolver.setup(cam.getWidth()/2, cam.getHeight()/2, 0.5, 3, 10, 1, 7, 1.5, false, false);
    ofEnableAlphaBlending();
	ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    blur.setup(cam.getWidth(), cam.getHeight(), 4, .2, 4);

	tracker.setup();
	tracker.setRescale(.5);
    
    classifier.load("expressions");
}


void ofApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
           finder.update(cam);
		if(tracker.update(toCv(cam))) {
			classifier.classify(tracker);
            flowSolver.update(cam);
		}		
	}
    
    int expression = classifier.getPrimaryExpression();
    cout<<expression;
    
    if (expression==1){
        text.load("peace.png");
//        text.draw(0,0);
        player1.load("Moorman-Charlotte_02_Synergy.mp3");
        player1.play();
    } else if (expression==2) {
        text.load("justice.png");
//        text.draw(0,0);
        player2.load("Moorman-Charlotte_04_Plus-Minus.mp3");
        player2.play();
    }
    
    blur.setScale(ofMap(mouseX, 0, ofGetWidth(), 1, 10));
    blur.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));
//    
//    //int expressions = classifier.size();
//    int expression = classifier.getPrimaryExpression();
//    cout<<expression;
//
//    
//    if (expression==1){
//        peace.load("peace.png");
//        cout<<"up";
//        
//    } else if (expression==2){
//        peace.load("justice.png");
//        cout<<"up";
//    }

    
}


void ofApp::draw() {
    
	ofSetColor(255);
    
    blur.begin();
	cam.draw(0, 0);
	tracker.draw();
    flowSolver.drawColored(cam.getWidth(), cam.getHeight(), 10, 3);
    ofSetCircleResolution(64);
    ofCircle(mouseX, mouseY, 32);
    blur.end();
  blur.draw();

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

    
    for(int i = 0; i < finder.size(); i++) {
        ofRectangle object = finder.getObjectSmoothed(i);
        text.setAnchorPercent(.5, .5);
        float scaleAmount = .85 * object.width / text.getWidth();
        ofPushMatrix();
        ofTranslate(object.x + object.width / 2., object.y + object.height * .42);
        ofScale(scaleAmount, scaleAmount);
        text.draw(0,0);
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(object.getPosition());
        ofDrawBitmapStringHighlight(ofToString(finder.getLabel(i)), 0, 0);
        ofDrawLine(ofVec2f(), toOf(finder.getVelocity(i)) * 10);
        ofPopMatrix();
    }
    

    
}

void ofApp::keyPressed(int key) {
    
    if(key == 'x'){
        img.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
        img.save("screenshot.png");
    }
    
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

