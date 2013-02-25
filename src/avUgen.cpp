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
        radius = DEFAULT_RADIUS;
        throttle = 0;
        speed = 0;
        lastCount = currentCount = 0;
        randomResolutionSwitch = false;
        color.set( ofRandom(255), ofRandom(255), ofRandom(255), LIGHT_ALPHA);

        initialize();
    }

    avUgen::avUgen(int _x, int _y, int _radius, ofColor _color, int _speed){
        
        x = _x;
        y = _y;
        radius = _radius;
        color = _color;
        speed = _speed;
        throttle = 0;
        lastCount = currentCount = 0;
        randomResolutionSwitch = false;
        
        initialize();
    }
    
    avUgen::~avUgen(){
        if (debug) logger.close();
    }

    void avUgen::initialize(){
        setAudioEngine(SINE);
        setVolume(DEFAULT_VOLUME);
        setFrequency(300);
        visualOutputSwitch = true;
        audioOutputSwitch = true;

        if (debug) logger.open("development.log");

        ofSetCircleResolution(100);

//        midiChannel = 1;
//        midiValue = 1;
//        midiControlNumber = 1;
    }
    
    void avUgen::moveTo(int _xDestiny, int _yDestiny){
        x += ( _xDestiny - x )*ofRandom(0.1, 1.0);
        y += ( _yDestiny - y )*ofRandom(0.1, 1.0);
    }
    
    void avUgen::draw(){

        double radius_wave_mulitplier = isAudioOn() ? getAudio() : 1;
        double radius_base = radius * DEFAULT_RADIUS_MULTPLIER;

        ofSetColor(color.r, color.g, color.b, color.a);
        ofFill();
                
        if (throttle == speed) {
            if (randomResolutionSwitch) ofSetCircleResolution(ofRandom(10));
            throttle = 0;
        } else {
            throttle++;
        }
        
        if (isVisualOn()){
            ofCircle(x, y, radius_base * radius_wave_mulitplier);
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

    int avUgen::getRadius(){
        return radius;
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

    void avUgen::setRandomResolution(){
        randomResolutionSwitch = true;
    }
    
    bool avUgen::isAudioOn(){
        return audioOutputSwitch;
    }

    bool avUgen::isVisualOn(){
        return visualOutputSwitch;
    }

    double avUgen::getVolume(){
        return volume;
    }

    void avUgen::setVolume(double _volume){
        if (_volume == 0) _volume = 1;
        cout << "_volume: " << _volume << endl;

        lastMIDIVolume = _volume;

        double offset = _volume >= 1  ? 10.0 : 1.0;

        // get a non linear curve
        volume = sqrt(_volume);
        cout << "volume: " << volume << endl;
    }

    void avUgen::setMIDIMapping(int _channel, int _control){
        midiChannel.push_back(_channel);
        midiControlNumber.push_back(_control);
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
                
                if (debug) logger << "currentCount: " << currentCount << endl;
                
                audio = osc.sinewave((double)color.getHue());

                // env stuff
                if (currentCount==1){
                    if (debug) logger << "######################### fire ########################" << endl;
                   ADSR.trigger(0, adsrEnv[0]);
                }
                
                ADSRout=ADSR.line(8,adsrEnv);//our ADSR env has 8 value/time pairs.
                audio = audio * ADSRout;
                // end env stuff
            }
        }
        return audio;
    }

    double avUgen::getAudioOutput(){
        return getAudio() * getVolume();
    }

    void avUgen::newMidiMessage(ofxMidiMessage& msg) {


        if (isFireMIDI(msg)) {
            midiMessage = msg;
            ofLogVerbose() << "FIRE MIDI midi ch: " << msg.channel << " control: " << msg.control << " for avUgen: " << this << endl;
            ofLogVerbose() << "value: " << msg.value << endl;

            if (msg.control == midiControlNumber.at(0)){
                ofLogVerbose() << "setting radius/volume" << endl;
                double thisVolume = lastMIDIVolume == 63.0 ? 0 : msg.value;
                radius = thisVolume;
                setVolume(thisVolume);
            } else if (msg.control == midiControlNumber.at(1)){
                ofLogVerbose() << "setting hue/pitch" << endl;
                color.setHue(msg.value);
                frequency = msg.value;
            }
        }
    }

    bool avUgen::isFireMIDI(ofxMidiMessage& msg){
        bool ret = false;

        for(int i=0; i<midiChannel.size(); i++){
            if (midiChannel.at(i) == msg.channel){
                for(int j=0; j<midiControlNumber.size(); j++){
                    if (midiControlNumber.at(j) == msg.control) ret = true;
                }
            }
        }

        return ret;
    }

};

