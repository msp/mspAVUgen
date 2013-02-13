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

        x = ofRandom(ofGetWindowWidth());
        y = ofRandom(ofGetWindowHeight());
        radius = 150;
        throttle = 0;
        speed = 0;
        
        rgbHsb.set( ofRandom(255), ofRandom(255), ofRandom(255), LIGHT_ALPHA);
        ofSetCircleResolution(100);
    }

    avUgen::avUgen(int _x, int _y, int _radius, ofColor _color, int _speed){
        
        x = _x;
        y = _y;
        radius = _radius;
        rgbHsb = _color;
        speed = _speed;
        throttle = 0;
        ofSetCircleResolution(100);
    }
    
    void avUgen::moveTo(int _xDestiny, int _yDestiny){
        x += ( _xDestiny - x )*ofRandom(0.1, 1.0);
        y += ( _yDestiny - y )*ofRandom(0.1, 1.0);
    }
    
    void avUgen::draw(){
        ofSetColor(rgbHsb.r, rgbHsb.g, rgbHsb.b, rgbHsb.a);
        ofFill();
                
        if (throttle == speed) {
            cout << "firing! " << endl;
            ofSetCircleResolution(ofRandom(10));
            throttle = 0;
        } else {
            throttle++;
        }
        
        ofCircle(x, y, radius);

    }
    
    void avUgen::update(){
        rgbHsb.setHue(ofRandom(255));
    }
    
    void avUgen::setX(int _x) {
        x = _x;
    }

    void avUgen::setY(int _y){
        y = _y;
    }
    
    void avUgen::setRadius(int _radius){
        radius = _radius;
    }
    
//    TODO higher numbers slow it down. Make this clear.
    void avUgen::setSpeed(int _speed){
        speed = _speed;
    }
    
    void avUgen::setColor(ofColor _color){
        rgbHsb = _color;
    }
    
    void avUgen::setOscillator(ofxMaxiOsc _osc){
        osc = _osc;
    }
    
    void avUgen::setFrequency(int _frequency){
        frequency = _frequency;
    }
    
    double avUgen::getWaveform(){
        return osc.sinewave((double)rgbHsb.getHue());
    }
};

