#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

ofImage img;
bool expressionSet = false;


void ofApp::setup() {
    
    
    ofSetVerticalSync(true);
    ofSetFrameRate(120);
    
    finder.setup("haarcascade_frontalface_default.xml");
    finder.setPreset(ObjectFinder::Fast);
    finder.getTracker().setSmoothingRate(.3);
    cam.setup(1280, 720);
    
    
    // FaceOSC sends to port 8338 by default
//    receiver.setup(8338);

    
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
    
//    // check for waiting osc messages
//    while(receiver.hasWaitingMessages()) {
//        
//        // get the next message
//        ofxOscMessage m;
//        receiver.getNextMessage(m);
//        
//        // load face from any FaceOSC messages
//        face.parseOSC(m);
//    }
//    
//    // found face?
//    if(face.found > 0) {
//        cout << "---------" << endl << face.toString();
//    }
//    
    
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
        player1.load("Moorman-Charlotte_02_Synergy.mp3");
        player1.play();
        cout<<"up";
      
    } else if (expression==2) {
        text.load("justice2.png");
        player2.load("Moorman-Charlotte_04_Plus-Minus.mp3");
        player2.play();
        img.grabScreen(0, 0 , ofGetWidth(), ofGetHeight());
        img.save("screenshot.png");
        cout<<"down";
    }
    
    blur.setScale(ofMap(mouseX, 0, ofGetWidth(), 1, 10));
    blur.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));

    
}


void ofApp::draw() {
    
    
//    // draw a face
//    if(face.found > 0) {
//        ofPushMatrix();
//        ofTranslate(face.posePosition);
//        ofScale(face.poseScale, face.poseScale);
//        
//        ofSetColor(0);
//        ofNoFill();
//        ofDrawEllipse(-20, face.eyeLeft * -9, 20, 7);
//        ofDrawEllipse(20, face.eyeRight * -9, 20, 7);
//        ofDrawEllipse(0, 20, face.mouthWidth * 3, face.mouthHeight * 3);
//        ofDrawEllipse(-5, face.nostrils * -1, 7, 3);
//        ofDrawEllipse(5, face.nostrils * -1, 7, 3);
//        
//        ofSetRectMode(OF_RECTMODE_CENTER);
//        ofFill();
//        ofDrawRectangle(-20, face.eyebrowLeft * -5, 25, 5);
//        ofDrawRectangle(20, face.eyebrowRight * -5, 25, 5);
//        ofSetRectMode(OF_RECTMODE_CORNER);
//        ofPopMatrix();
//    }
//    
//    ofPushMatrix();
//    ofTranslate(face.posePosition);
//    ofScale(face.poseScale, face.poseScale);
//    
//    
//    // the face is cry
//    if(face.mouthHeight > 5) {
//        text.draw(face.posePosition.x+200, face.posePosition.y+200);
//    }
//    ofSetColor(255,255,255);
//    ofPopMatrix();
    
    
    
// ___________________BLUR BEGINS
	ofSetColor(255);
    
    blur.begin();
	cam.draw(0, 0);
	tracker.draw();
    flowSolver.drawColored(cam.getWidth(), cam.getHeight(), 10, 3);
    ofSetCircleResolution(64);
    ofDrawCircle(mouseX, mouseY, 32);
    blur.end();
  blur.draw();
    
// ___________________BLUR ENDS

    
    
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
                                
                                "x - grab screenshot\n" +
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

