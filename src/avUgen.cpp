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
        color.set( ofRandom(255), ofRandom(255), ofRandom(255), LIGHT_ALPHA);

        initialize();
    }

    avUgen::avUgen(string _name){
        name = _name;
        initialize();
    }

    avUgen::avUgen(int _x, int _y, int _radius, ofColor _color){
        
        x = _x;
        y = _y;
        radius = _radius;
        color = _color;
        
        initialize();
    }
    
    avUgen::~avUgen(){
        if (debug) logger.close();
    }

    void avUgen::initialize(){
        setAudioEngine(SINE);
        setVolume(DEFAULT_VOLUME);
        setPan(0.5);
        setFrequency(300);
        visualOutputSwitch = true;
        audioOutputSwitch = true;
        randomResolutionSwitch = false;
        animateRadiusSwitch = false;
        throttle = 10;
        frame = 0;
        lastCount = currentCount = 0;

        if (debug) logger.open("development.log");

        ofSetCircleResolution(100);

    }
    
    void avUgen::moveTo(int _xDestiny, int _yDestiny){
        x += ( _xDestiny - x )*ofRandom(0.1, 1.0);
        y += ( _yDestiny - y )*ofRandom(0.1, 1.0);
    }
    
    void avUgen::draw(){

        double radius_wave_mulitplier = isAudioOn() ? getAudio() : 1;
        double radius_base = radius * DEFAULT_RADIUS_MULTPLIER;
        double circle_radius = animateRadiusSwitch ? radius_base * radius_wave_mulitplier : radius_base;

        ofSetColor(color.r, color.g, color.b, color.a);
        ofFill();
                
        if (frame == throttle) {
//            if (name == "msp1") ofLogVerbose() << "DRAW frame: "<< frame << " throttle: "<< throttle << endl;
            if (randomResolutionSwitch) ofSetCircleResolution(ofRandom(10));
            if (isVisualOn()) ofCircle(x, y, circle_radius);
            frame = 0;
        } else {
            if (frame < throttle) {
                frame++;
            } else {
                frame = 0;
            }

        }        
    }
    
    void avUgen::update(){
//        color.setHue(ofRandom(255));
    }

    string avUgen::getName(){
        return name;
    }

    // return absolute x pos on screen
    int avUgen::getX(){
        return x;
    }

    // expect a midi value 1 - 127
    void avUgen::setX(int _x) {
        _x = _x > 127 ? 127 : _x;
        _x = _x < 1 ? 1 : _x;
        x = (ofGetWindowWidth() / 127) * _x;
    }

    // return a pan between 0 & 1
    double avUgen::getPan(){
//        ofLogVerbose() << "x: " << x << endl;
//        ofLogVerbose() << "pan: " << pan << endl;
        return pan;
    }

    // expect a midi value 1 - 127
    void avUgen::setPan(double _pan) {
        pan = _pan / 100;
    }

    int avUgen::getY(){
        return y;
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

    void avUgen::setThrottle(int _throttle){
        throttle = _throttle;
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

    void avUgen::setRandomResolution(bool _randomResolutionSwitch){
        randomResolutionSwitch = _randomResolutionSwitch;
    }

    void avUgen::setAnimateRadius(bool _animateRadiusSwitch){
        animateRadiusSwitch = _animateRadiusSwitch;
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
        lastMIDIVolume = _volume;

        // get a non linear curve
        volume = sqrt(_volume);
        ofLogVerbose() << "volume: " << volume << endl;
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

                audio = osc.sinewave((double)color.getHue());

                // env stuff
                if (frame == throttle) {
//                    if (currentCount==1){
//                      if (name == "msp1"){
//                        ofLogVerbose() << "AUDIO frame: " << frame << " currentCount: " << currentCount << endl;
//                        ADSR.trigger(0, adsrEnv[0]);
//                      }
//                    }

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
        return getVolume() == 0 ? 0.0 : getAudio() * getVolume();
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
                frequency = msg.value * 100;
            } else if (msg.control == midiControlNumber.at(2)){
                ofLogVerbose() << "setting throttle" << endl;
                throttle = msg.value;
            } else if (msg.control == midiControlNumber.at(3)){
                ofLogVerbose() << "setting pan" << endl;
                setPan(msg.value);
                setX(msg.value);
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

