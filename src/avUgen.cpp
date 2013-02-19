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
        lastCount = currentCount = 0;
        
        audioEngine = SINE;
        visualOutputSwitch = true;
        audioOutputSwitch = true;
        targetCount = (int) ofGetFrameRate() * 10;

        logger.open("development.log");
        
        color.set( ofRandom(255), ofRandom(255), ofRandom(255), LIGHT_ALPHA);
        ofSetCircleResolution(100);
    }

    avUgen::avUgen(int _x, int _y, int _radius, ofColor _color, int _speed){
        
        x = _x;
        y = _y;
        radius = _radius;
        color = _color;
        speed = _speed;
        throttle = 0;
        lastCount = currentCount = 0;
        
        audioEngine = SINE;
        visualOutputSwitch = true;
        audioOutputSwitch = true;
        targetCount = (int) ofGetFrameRate() * 10;
        logger.open("development.log");
        
        ofSetCircleResolution(100);
    }
    
    avUgen::~avUgen(){
        logger.close();
    }
    
    void avUgen::moveTo(int _xDestiny, int _yDestiny){
        x += ( _xDestiny - x )*ofRandom(0.1, 1.0);
        y += ( _yDestiny - y )*ofRandom(0.1, 1.0);
    }
    
    void avUgen::draw(){
        ofSetColor(color.r, color.g, color.b, color.a);
        ofFill();
                
        if (throttle == speed) {
//            ofSetCircleResolution(ofRandom(10));
            throttle = 0;
        } else {
            throttle++;
        }
        
        if (isVisualOn()){
            ofCircle(x, y, radius);
        }
    }
    
    void avUgen::update(){
//        color.setHue(ofRandom(255));
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
        color = _color;
    }
    
    void avUgen::setOscillator(ofxMaxiOsc _osc){
        osc = _osc;
    }
    
    void avUgen::setFrequency(int _frequency){
        frequency = _frequency;
    }
    
    void avUgen::setAudioEngine(int _audioEngine) {
        audioEngine = _audioEngine;
    }
    
    void avUgen::switchOffAudio(){
        audioOutputSwitch = false;
    }

    void avUgen::switchOffVisual(){
        visualOutputSwitch = false;
    }
    
    bool avUgen::isAudioOn(){
        return audioOutputSwitch;
    }

    bool avUgen::isVisualOn(){
        return visualOutputSwitch;
    }    
    
    double avUgen::getAudio(){
        audio = 1;
        if (isAudioOn()) {
            if (audioEngine == SINE) {
                audio = osc.sinewave(frequency);
            } else if (audioEngine == MONO) {
                
                // Metronome
                // Phasor can take three arguments; frequency, start value and end
                currentCount = timer.phasor((int)ofGetFrameRate()/10, 1, 50);
                
                logger << "currentCount: " << currentCount << endl;
                
                audio = osc.sinewave((double)color.getHue());

                // env stuff
                if (currentCount==1){
                    logger << "######################### fire ########################" << endl;
                   ADSR.trigger(0, adsrEnv[0]);
                }
                
                ADSRout=ADSR.line(8,adsrEnv);//our ADSR env has 8 value/time pairs.
                audio = audio * ADSRout;
                // end env stuff
            }
        }
        return audio;
    }
};

