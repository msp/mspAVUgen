//
//  avUgen.cpp
//  mspUgen
//
//  Created by msp on 06/02/2013.
//
//

#include "avUgen.h"

namespace msp {
    avUgen::avUgen(){

        radius = 150;
        rgbHsb.set( ofRandom(255), ofRandom(255), ofRandom(255));

        x = ofRandom(ofGetWindowWidth());
        y = ofRandom(ofGetWindowHeight());

        ofSetCircleResolution(100);
    }
    
    void avUgen::moveTo(int _xDestiny, int _yDestiny){
        x += ( _xDestiny - x )*0.1;
        y += ( _yDestiny - y )*0.1;
    }
    
    void avUgen::draw(){
        ofSetColor(rgbHsb);
        ofFill();
        ofCircle(x, y, radius);
    }
    
    void avUgen::update(){
//        rgbHsb.setHue(ofRandom(255));
        ofSetCircleResolution(ofRandom(60));
    }
    
};

