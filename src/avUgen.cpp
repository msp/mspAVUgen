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
        
        ofSetCircleResolution(100);
    }
    
    void avUgen::moveTo(int _xDestiny, int _yDestiny){
        x += ( _xDestiny - x )*ofRandom(0.1, 1.0);
        y += ( _yDestiny - y )*ofRandom(0.1, 1.0);
    }
    
    void avUgen::draw(){
        ofSetColor(color.r, color.g, color.b, color.a);
        ofFill();
                
        if (throttle == speed) {
            ofSetCircleResolution(ofRandom(10));
            throttle = 0;
        } else {
            throttle++;
        }
        
        cout << "MSP isVisualOn(): " << isVisualOn() << endl;
        
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
                //            audio = osc.sinewave((double)color.getHue());
                audio = osc.sinewave(frequency);
            } else if (audioEngine == MONO) {
                

                //      Monosynth
                // so this first bit is just a basic metronome so we can hear what we're doing.
                
                // currentCount=(int)timer.phasor(0.5);//this sets up a metronome that ticks every 2 seconds
                
                currentCount = ofGetFrameRate();
                
                //            if (lastCount != currentCount) {
                //
                //                ADSR.trigger(0, adsrEnv[0]);//trigger the envelope from the start
                //
                //                cout << "tick\n";//the clock ticks
                //
                //                lastCount=0;
                //            }
                
                //            ADSRout=ADSR.line(8,adsrEnv);//our ADSR env has 8 value/time pairs.
                
                //            LFO1out=LFO1.sinebuf(0.2);//this lfo is a sinewave at 0.2 hz
                
                VCO1out=VCO1.square(frequency);
                VCO2out=VCO2.sinewave(frequency+LFO1out);//here's VCO2. it's a pulse wave at 110hz with LFO modulation on the frequency, and width of 0.2
                
                //now we stick the VCO's into the VCF, using the ADSR as the filter cutoff
                //            VCFout=VCF.lores((VCO1out+VCO2out)*0.5, 250+(ADSRout*15000), 10);
                audio = VCO1out; //*ADSRout;//finally we add the ADSR as an amplitude modulator
            }
        }
        
        return audio;
    }
};

