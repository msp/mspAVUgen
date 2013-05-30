//
//  soundBank.cpp
//  mspUgen
//
//  Created by msp on 30/05/2013.
//
//

#include "soundBank.h"

namespace msp {
    soundBank::soundBank(){

    }
    soundBank::soundBank(std::vector<string> _avUgenNames){
        avUgenNames = _avUgenNames;

        // initialise the 2d vector
        for(int i = 0; i <= TOTAL_SLOTS; i++) {
            vector<msp::avUgen*> row;
            presetSlots.push_back(row);
        }

        for (int i = 0; i < avUgenNames.size(); i++){
            loadXMLPresetOrInitialise(avUgenNames.at(i), i);
        }

        ofLogVerbose() << presetSlots[0][0]-> getName();
        ofLogVerbose() << presetSlots[0][1]-> getName();

        ofLogVerbose() << presetSlots[1][0]-> getName();
        ofLogVerbose() << presetSlots[1][1]-> getName();

        ofLogVerbose() << presetSlots[2][0]-> getName();
        ofLogVerbose() << presetSlots[2][1]-> getName();

        ofLogVerbose() << presetSlots[3][0]-> getName();
        ofLogVerbose() << presetSlots[3][1]-> getName();

        cyclePreset();
    }

    soundBank::~soundBank(){
    }

    void soundBank::cyclePreset(){

        ofLogVerbose() << "currentPresent: " << currentPreset << endl;

        audioReady = false;
        activeSlots.clear();
        
        for (int i = 0; i <= TOTAL_SLOTS; i++){
            activeSlots.push_back(presetSlots[i][currentPreset]);
            ofLogVerbose() << "active at slot [" << i << "] : " << activeSlots.at(i) -> getName();
        }
        audioReady = true;

        if (currentPreset == TOTAL_PRESETS){
            currentPreset = 0;
        } else {
            currentPreset++;
        }

        ofLogVerbose() << "cyclePreset: activeSlots: " << activeSlots.size() << endl;
    };

    
    void soundBank::loadXMLPresetOrInitialise(string name, int slot){

        string filename = "";

        if (loadFromXML) {

            avUgen *preset;
            ofxXmlSettings settings;
            filename = "avUgen/" + name + ".xml";

            if(settings.loadFile(filename)){
                ofLogNotice() << filename + " loaded!";

                int numTags = settings.getNumTags("avUgens:avUgen");
                ofLogVerbose() << "Found tags: " << numTags << endl;

                if(numTags > 0){
                    // we push into the preset avUgens tag this temporarirly
                    // treats the tag as the document root.
                    settings.pushTag("avUgens", 0);

                    // we see how many avUgens we have stored in <avUgen> tags
                    int numPresets = settings.getNumTags("avUgen");
                    ofLogVerbose() << "Found presets: " << numPresets << endl;

                    if(numPresets > 0){

                        for(int i = 0; i < numPresets; i++){

                            preset = new msp::avUgen(settings.getValue("avUgen:name", name, i));

                            preset->setX(settings.getValue("avUgen:x", preset->getX(), i));
                            preset->setY(settings.getValue("avUgen:y", preset->getY(), i));
                            preset->setAudioEngine(settings.getValue("avUgen:audioEngine", preset->getAudioEngine(), i));
                            preset->setRadius(settings.getValue("avUgen:radius", preset->getRadius(), i));
                            //TODO raw set volume without sqrt
                            preset->setVolume(settings.getValue("avUgen:volume", preset->getVolume(), i));
                            //TODO raw set pan without /100
                            preset->setPan(settings.getValue("avUgen:pan", preset->getPan(), i));
                            preset->setFrequency(settings.getValue("avUgen:frequency", preset->getFrequency(), i));
                            preset->setVisualOutputSwitch(settings.getValue("avUgen:visualOutputSwitch", preset->getVisualOutputSwitch(), i));
                            preset->setAudioOutputSwitch(settings.getValue("avUgen:audioOutputSwitch", preset->getAudioOutputSwitch(), i));
                            preset->setAudioEngine(settings.getValue("avUgen::audioEngine", msp::avUgen::MONO, i));
                            preset->setRandomResolutionSwitch(settings.getValue("avUgen:randomResolutionSwitch", preset->getRandomResolutionSwitch(), i));
                            preset->setAnimateRadiusSwitch(settings.getValue("avUgen:animateRadiusSwitch", preset->getAnimateRadiusSwitch(), i));
                            preset->setThrottle(settings.getValue("avUgen:throttle", preset->getThrottle(), i));

                            ofColor _color = *new ofColor();
                            _color.setHue(settings.getValue("avUgen:hue", ofRandom(255), i));
                            _color.setSaturation(settings.getValue("avUgen:saturation", ofRandom(255), i));
                            _color.setBrightness(settings.getValue("avUgen:brightness", ofRandom(255), i));
                            preset->setColor(_color);

                            preset->inspect();

                            presetSlots[slot].push_back(preset);

                            ofLogVerbose() << presetSlots[slot][i]-> getName();
                            
                        }
                    }
                    
                    settings.popTag();
                }
                
            } else {
                ofLogError() << "unable to load "+ filename +" check data/ folder";
            }
        }
    }
}