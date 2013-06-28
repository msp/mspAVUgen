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
        name = pseudoRandomName();
        initialize();
    }

    avUgen::avUgen(string _name){
        name = _name;
        initialize();
    }

    avUgen::~avUgen(){
        if (debug) logger.close();
    }

    void avUgen::initialize(){

        setX(ofRandom(ofGetWindowWidth()));
        setY(ofRandom(ofGetWindowHeight()));

        color.set(255,0,0, LIGHT_ALPHA); //default red

        setAudioEngine(MONO);

        setRadiusMIDI(ofRandom(127));
        setPan(0.5);

        setHueMIDI(ofRandom(127));
        setThrottleMIDI(ofRandom(127));

        visualOutputSwitch = true;
        audioOutputSwitch = true;
        randomResolutionSwitch = false;
        animateRadiusSwitch = false;

        frame = 0;
        lastCount = 0;
        currentCount = 0;


        if (debug) logger.open("development.log");
        ofSetCircleResolution(100);
    }

    string avUgen::pseudoRandomName() {
        ostringstream buffer;
        buffer << "mspUgen_" << ofRandom(10000);
        return buffer.str();
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
    }

    string avUgen::getName(){
        return name;
    }

    int avUgen::getX(){
        return x;
    }

    void avUgen::setX(int _x) {
        x = _x;
    }

    // expect a midi value 1 - 127
    void avUgen::setXMIDI(int _x) {
        _x = _x > 127 ? 127 : _x;
        _x = _x < 1 ? 1 : _x;

        setPanMIDI(_x);
        x = (ofGetWindowWidth() / 127) * _x;
    }

    double avUgen::getPan(){
        return pan;
    }

    void avUgen::setPan(double _pan) {
        pan = _pan;
    }

    // expect a midi value 1 - 127
    void avUgen::setPanMIDI(int _pan) {
        pan = (double) _pan / 127;
        ofLogVerbose() << "pan : " << pan << endl;
    }

    int avUgen::getY(){
        return y;
    }

    void avUgen::setY(int _y){
        y = _y;
    }

    int avUgen::getRadius(){
        return radius;
    }

    void avUgen::setRadius(int _radius){
        radius = _radius;
    }

    // expect a midi value 1 - 127
    void avUgen::setRadiusMIDI(int _radius){
        lastMIDIRadius = _radius;
        setVolumeMIDI(_radius);
        radius = _radius;
    }

    int avUgen::getThrottle(){
        return throttle;
    }

    void avUgen::setThrottle(int _throttle){
        throttle = _throttle;
    }

    // expect a midi value 1 - 127
    void avUgen::setThrottleMIDI(int _throttle){
        throttle = _throttle;
    }

    ofColor avUgen::getColor(){
        return color;
    }

    void avUgen::setColor(ofColor _color){
        color = _color;
    }

    void avUgen::setOscillator(ofxMaxiOsc _osc){
        osc = _osc;
    }

    int avUgen::getFrequency(){
        return frequency;
    }

    void avUgen::setFrequency(int _frequency){
        frequency = _frequency;
    }

    // expect a midi value 1 - 127
    void avUgen::setFrequencyMIDI(int _frequency){

        int minFreq = 20;
        int maxFreq = 20000;

        float p1 = (float) _frequency / 127; // we now have p1 = [0..1]
        frequency = minFreq + (p1*p1) * (maxFreq-minFreq);

        ofLogVerbose() << "setting frequency " << frequency << endl;
    }

    int avUgen::getAudioEngine() {
        return audioEngine;
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

    bool avUgen::getRandomResolutionSwitch(){
        return randomResolutionSwitch;
    }

    void avUgen::setRandomResolutionSwitch(bool _randomResolutionSwitch){
        randomResolutionSwitch = _randomResolutionSwitch;
    }

    bool avUgen::getAnimateRadiusSwitch(){
        return animateRadiusSwitch;
    }
    
    void avUgen::setAnimateRadiusSwitch(bool _animateRadiusSwitch){
        animateRadiusSwitch = _animateRadiusSwitch;
    }

    bool avUgen::getVisualOutputSwitch() {
        return visualOutputSwitch;
    }

    void avUgen::setVisualOutputSwitch(bool _visualOutputSwitch){
        visualOutputSwitch = _visualOutputSwitch;
    }

    bool avUgen::getAudioOutputSwitch() {
        return audioOutputSwitch;
    }

    void avUgen::setAudioOutputSwitch(bool _audioOutputSwitch){
        audioOutputSwitch = _audioOutputSwitch;
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
        volume = _volume;
    }

    // expect a midi value 1 - 127
    void avUgen::setVolumeMIDI(int _volume){
        // get a non linear curve
        volume = sqrt(_volume);
    }

    // expect a midi value 1 - 127
    void avUgen::setHueMIDI(int _hue){
        color.setHue((_hue * 2) - 10);
        ofLogVerbose() << "setting hue " << color.getHue() << endl;
        setFrequencyMIDI(_hue);
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

                audio = osc.sinewave(frequency);

                // env stuff
                if (frame == throttle) {
                    ADSR.trigger(0, adsrEnv[0]);
                }

                ADSRout=ADSR.line(8,adsrEnv);//our ADSR env has 8 value/time pairs.
                audio = audio * ADSRout;
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
            ofLogVerbose() << "FIRE MIDI midi ch: " << msg.channel << " control: " << msg.control << " for avUgen: " << name << endl;
            ofLogVerbose() << "value: " << msg.value << endl;

            if (msg.control == midiControlNumber.at(0)){
                double thisRadius = lastMIDIRadius == 63.0 ? 0 : msg.value;
                ofLogVerbose() << "setting radius/volume: " << thisRadius  << endl;
                setRadiusMIDI(thisRadius);
            } else if (msg.control == midiControlNumber.at(1)){
                setHueMIDI(msg.value);
            } else if (msg.control == midiControlNumber.at(2)){
                ofLogVerbose() << "setting throttle: " << msg.value << endl;
                setThrottleMIDI(msg.value);
            } else if (msg.control == midiControlNumber.at(3)){
                ofLogVerbose() << "setting pan: " << msg.value << endl;
                setXMIDI(msg.value);
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

    void avUgen::inspect() {
        ofLogVerbose() << "avUgen: "<< name
        << ", x: " << x
        << ", y: " << y
        << ", pan: " << pan
        << ", frequency: " << frequency
        << ", volume: " << volume
        << ", radius: " << radius
        << ", hue: " << color.getHue()
        << ", saturation: " << color.getSaturation()
        << ", brightness: " << color.getBrightness()
        << endl;
    }

};

