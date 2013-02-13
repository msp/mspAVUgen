#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
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
//    circle.moveTo(mouseX,mouseY);
    circle.update();
}

//--------------------------------------------------------------
void testApp::draw(){
//    ofBackgroundGradient(ofColor::gray,ofColor(30,10,30), OF_GRADIENT_CIRCULAR);
    
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    circle.draw();
    
    ofPopStyle();
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