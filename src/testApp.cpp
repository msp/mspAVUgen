#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    
    msp::avUgen * ch1;
    
    ch1 = new msp::avUgen();
    ch1->setX(width/2);
    ch1->setY(height/2);
    ch1->setRadius(100);
    ch1->setSpeed(10);
    ch1->setColor(*new ofColor(233, 52, 70, msp::avUgen::LIGHT_ALPHA));
    
    channels.push_back(ch1);
    
    msp::avUgen * ch2;
    
    ch2 = new msp::avUgen();
    ch2->setX(width/2 - 100);
    ch2->setY(height/2 - 100);
    ch2->setRadius(100);
    ch2->setSpeed(30);
    ch2->setColor(*new ofColor(100, 200, 100, msp::avUgen::LIGHT_ALPHA));
    
    channels.push_back(ch2);
    
    red = 0;
    blue = 0;
    green = 0;
    backgroundColor = ofColor(red, green, blue);
        
    
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetFrameRate(30);    
    ofBackground(backgroundColor);
}

//--------------------------------------------------------------
void testApp::update(){
    for (int i=0; i<channels.size(); i++) {
        channels.at(i) -> update();
//        channels.at(i) -> moveTo(mouseX,mouseY);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
//    ofBackgroundGradient(ofColor::gray,ofColor(30,10,30), OF_GRADIENT_CIRCULAR);    
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    for (int i=0; i<channels.size(); i++) {
        ofPushStyle();
        channels.at(i) -> draw();
        ofPopStyle();
    }
        
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}