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
        x = ofRandom(ofGetWindowWidth());
        y = ofRandom(ofGetWindowHeight());
        radius = DEFAULT_RADIUS;
        color.set(ofRandom(255), ofRandom(255), ofRandom(255), LIGHT_ALPHA);

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
        lastCount = 0;
        currentCount = 0;

        if (debug) logger.open("development.log");
        ofSetCircleResolution(100);
        loadXMLSettings();
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

        // TODO this isn't quite right. Not convinced correct values are computed. Test if exact values restored from settings.
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

    int avUgen::getRadius(){
        return radius;
    }

    void avUgen::setRadius(int _radius){
        radius = _radius;
    }

    int avUgen::getThrottle(){
        return throttle;
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

    int avUgen::getFrequency(){
        return frequency;
    }

    void avUgen::setFrequency(int _frequency){
        frequency = _frequency;
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
        lastMIDIVolume = _volume;

        // get a non linear curve
        volume = sqrt(_volume);
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
            ofLogVerbose() << "FIRE MIDI midi ch: " << msg.channel << " control: " << msg.control << " for avUgen: " << name << endl;
            ofLogVerbose() << "value: " << msg.value << endl;

            if (msg.control == midiControlNumber.at(0)){
                double thisVolume = lastMIDIVolume == 63.0 ? 0 : msg.value;
                ofLogVerbose() << "setting radius/volume: " << thisVolume  << endl;
                radius = thisVolume;
                setVolume(thisVolume);
            } else if (msg.control == midiControlNumber.at(1)){
                color.setHue(msg.value);
                frequency = sqrt(msg.value) * (sqrt(msg.value) * 12);
                ofLogVerbose() << "setting hue/pitch: " << frequency << endl;
            } else if (msg.control == midiControlNumber.at(2)){
                ofLogVerbose() << "setting throttle: " << msg.value << endl;
                throttle = msg.value;
            } else if (msg.control == midiControlNumber.at(3)){
                ofLogVerbose() << "setting pan: " << msg.value << endl;
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

    void avUgen::saveXMLSettings(){

        if (saveToXML) {

            ofLogVerbose() << "saving XML settings:" << endl;
            inspect();
            
            ofxXmlSettings settings;
            settings.setValue("avUgen:name", name);
            settings.setValue("avUgen:x", x);
            settings.setValue("avUgen:y", y);
            settings.setValue("avUgen:radius", radius);
            settings.setValue("avUgen:audioEngine", audioEngine);
            settings.setValue("avUgen:volume", volume);
            settings.setValue("avUgen:pan", pan);
            settings.setValue("avUgen:frequency", frequency);
            settings.setValue("avUgen:visualOutputSwitch", visualOutputSwitch);
            settings.setValue("avUgen:audioOutputSwitch", audioOutputSwitch);
            settings.setValue("avUgen:audioEngine", audioEngine);
            settings.setValue("avUgen:randomResolutionSwitch", randomResolutionSwitch);
            settings.setValue("avUgen:animateRadiusSwitch", animateRadiusSwitch);
            settings.setValue("avUgen:throttle", throttle);

            settings.setValue("avUgen:hue", color.getHue());
            settings.setValue("avUgen:saturation", color.getSaturation());
            settings.setValue("avUgen:brightness", color.getBrightness());

            settings.saveFile("avUgen/" + name + ".xml");
        }
    }

    void avUgen::loadXMLSettings(){

        if (loadFromXML) {
            ofLogVerbose() << "loading XML settings:" << endl;

            ofxXmlSettings settings;
            settings.loadFile("avUgen/" + name + ".xml");

            x = settings.getValue("avUgen:x", x);
            y = settings.getValue("avUgen:y", y);
            audioEngine = settings.getValue("avUgen:audioEngine", audioEngine);
            radius = settings.getValue("avUgen:radius", radius);
            volume = settings.getValue("avUgen:volume", volume);
            pan = settings.getValue("avUgen:pan", pan);
            frequency =  settings.getValue("avUgen:frequency", frequency);
            visualOutputSwitch = settings.getValue("avUgen:visualOutputSwitch", visualOutputSwitch);
            audioOutputSwitch = settings.getValue("avUgen:audioOutputSwitch", audioOutputSwitch);
            audioEngine = settings.getValue("avUgen::audioEngine", msp::avUgen::MONO);
            randomResolutionSwitch = settings.getValue("avUgen:randomResolutionSwitch", randomResolutionSwitch);
            animateRadiusSwitch = settings.getValue("avUgen:animateRadiusSwitch", animateRadiusSwitch);
            throttle =  settings.getValue("avUgen:throttle", throttle);

            color.setHue(settings.getValue("avUgen:hue", ofRandom(255)));
            color.setSaturation(settings.getValue("avUgen:saturation", ofRandom(255)));
            color.setBrightness(settings.getValue("avUgen:brightness", ofRandom(255)));

            inspect();
        }
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

