//
//  soundBank.cpp
//  mspAVUgen
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

        loadPresetsOrInitialise();

        for (int i = 0; i <= TOTAL_SLOTS; i++){
            for(int j = 0; j <= TOTAL_PRESETS; j++){
                ofLogVerbose() << presetSlots[i][j]-> getName();
            }
        }

        activateCurrentPreset();
    }

    soundBank::~soundBank(){
    }

    void soundBank::setMIDIMapping(int _channel, int _control){
        midiChannel.push_back(_channel);
        midiControlNumber.push_back(_control);
    }

    void soundBank::activateCurrentPreset() {
        audioReady = false;
        activeSlots.clear();

        for (int i = 0; i <= TOTAL_SLOTS; i++){
            activeSlots.push_back(presetSlots[i][currentPreset]);
            ofLogNotice() << "active at slot [" << i << "] : " << activeSlots.at(i) -> getName();
        }
        audioReady = true;
    }

    void soundBank::setPreset(int _preset) {

        if (_preset >= TOTAL_PRESETS || _preset < 0){
            currentPreset = 0;
        }

        currentPreset = _preset;
        activateCurrentPreset();
    }

    void soundBank::cyclePreset(){

        currentPreset++;
        if (currentPreset >= TOTAL_PRESETS){
            currentPreset = 0;
        }

        ofLogNotice() << "currentPresent: " << currentPreset << endl;
        activateCurrentPreset();

        ofLogVerbose() << "cyclePreset: activeSlots: " << activeSlots.size() << endl;
    };

    
    void soundBank::loadPresetsOrInitialise(){

        string filename = "";
        ofxXmlSettings settings;

        for(int i = 0; i <= TOTAL_SLOTS; i++) {

            // initialise the 2d vector
            vector<msp::avUgen*> row;
            presetSlots.push_back(row);

            filename = "avUgen/" + avUgenNames.at(i) + ".xml";

            if(settings.loadFile(filename)){
                ofLogNotice() << filename + " loaded! +++++++++++++++++";

                settings.pushTag("avUgens", 0);
                ofLogNotice() << "Found presets in file: " << settings.getNumTags("avUgen") << endl;

                for (int j = 0; j <= TOTAL_PRESETS; j++){

                    char buffer[100];
                    std::ostringstream buf;                    
                    buf << avUgenNames.at(i) << "-" << j;

                    loadPreset(settings, j, buf.str(), i);
                }

                settings.popTag(); //avUgens

            } else {
                ofLogWarning() << "unable to load "+ filename +" check data/ folder. Initialising defaults instead..";

                for (int j = 0; j <= TOTAL_PRESETS; j++){

                    char buffer[100];
                    std::ostringstream buf;
                    buf << avUgenNames.at(i) << "-" << j;

                    avUgen *preset = new msp::avUgen(settings.getValue("avUgen:name", buf.str(), i));
                    presetSlots[i].push_back(preset);
                }

            }
            
        }

    }


    void soundBank::loadPreset(ofxXmlSettings settings, int i, string name, int slot) {
        avUgen *preset = new msp::avUgen(settings.getValue("avUgen:name", name, i));

        if (loadFromXML){
            preset->setX(settings.getValue("avUgen:x", preset->getX(), i));
            preset->setY(settings.getValue("avUgen:y", preset->getY(), i));
            preset->setAudioEngine(settings.getValue("avUgen:audioEngine", preset->getAudioEngine(), i));
            preset->setRadius(settings.getValue("avUgen:radius", preset->getRadius(), i));
            preset->setVolume(settings.getValue("avUgen:volume", preset->getVolume(), i));
            preset->setPan(settings.getValue("avUgen:pan", preset->getPan(), i));
            preset->setFrequency(settings.getValue("avUgen:frequency", preset->getFrequency(), i));
            preset->setVisualOutputSwitch(settings.getValue("avUgen:visualOutputSwitch", preset->getVisualOutputSwitch(), i));
            preset->setAudioOutputSwitch(settings.getValue("avUgen:audioOutputSwitch", preset->getAudioOutputSwitch(), i));
            preset->setAudioEngine(settings.getValue("avUgen::audioEngine", preset->getAudioEngine(), i));
            preset->setRandomResolutionSwitch(settings.getValue("avUgen:randomResolutionSwitch", preset->getRandomResolutionSwitch(), i));
            preset->setAnimateRadiusSwitch(settings.getValue("avUgen:animateRadiusSwitch", preset->getAnimateRadiusSwitch(), i));
            preset->setThrottle(settings.getValue("avUgen:throttle", preset->getThrottle(), i));
            preset->setMIDIMapping(settings.getValue("avUgen:midiChannel1", 14, i), settings.getValue("avUgen:midiControl1", 100, i));
            preset->setMIDIMapping(settings.getValue("avUgen:midiChannel2", 14, i), settings.getValue("avUgen:midiControl2", 80, i));
            preset->setMIDIMapping(settings.getValue("avUgen:midiChannel3", 14, i), settings.getValue("avUgen:midiControl3", 60, i));
            preset->setMIDIMapping(settings.getValue("avUgen:midiChannel4", 14, i), settings.getValue("avUgen:midiControl4", 40, i));
            preset->setMIDIMapping(settings.getValue("avUgen:midiChannel5", 14, i), settings.getValue("avUgen:midiControl5", 44, i));

            ofColor _color = *new ofColor();
            float hue = settings.getValue("avUgen:hue", preset->getColor().getHue(), i);
            float saturation = settings.getValue("avUgen:saturation", preset->getColor().getSaturation(), i);
            float brightness = settings.getValue("avUgen:brightness", preset->getColor().getBrightness(), i);
            float alpha = settings.getValue("avUgen:alpha", preset->getColor().a, i);
            _color.setHsb(hue, saturation, brightness, alpha);
            preset->setColor(_color);
        }

        preset->inspect();
        presetSlots[slot].push_back(preset);
    }

    void soundBank::savePresetsToXML(){
        if (saveToXML) {

            ofLogVerbose() << "savePresetsToXML()" << endl;

            string filename = "";
            int tagId = 0;

            // file loop
            for (int i = 0; i <= TOTAL_SLOTS; i++){

                ofxXmlSettings settings;

                tagId = settings.addTag("avUgens");
                settings.pushTag("avUgens", tagId);
                avUgen *preset;

                for (int j = 0; j <= TOTAL_PRESETS; j++){

                    preset = presetSlots[i][j];
                    preset->inspect();

                    tagId = settings.addTag("avUgen");
                    settings.pushTag("avUgen",tagId);

                    settings.setValue("name", preset->getName(), i);
                    settings.setValue("x", preset->getX(), i);
                    settings.setValue("y", preset->getY(), i);
                    settings.setValue("audioEngine", preset->getAudioEngine(), i);
                    settings.setValue("radius", preset->getRadius(), i);
                    settings.setValue("volume", preset->getVolume(), i);
                    settings.setValue("pan", preset->getPan(), i);
                    settings.setValue("frequency", preset->getFrequency(), i);
                    settings.setValue("visualOutputSwitch", preset->getVisualOutputSwitch(), i);
                    settings.setValue("audioOutputSwitch", preset->getAudioOutputSwitch(), i);
                    settings.setValue("audioEngine", preset->getAudioEngine(), i);
                    settings.setValue("randomResolutionSwitch", preset->getRandomResolutionSwitch(), i);
                    settings.setValue("animateRadiusSwitch", preset->getAnimateRadiusSwitch(), i);
                    settings.setValue("throttle", preset->getThrottle(), i);

                    settings.setValue("hue", preset->getColor().getHue());
                    settings.setValue("saturation", preset->getColor().getSaturation());
                    settings.setValue("brightness", preset->getColor().getBrightness());
                    settings.setValue("alpha", preset->getColor().a);

                    for (int i = 0; i < preset->midiChannel.size(); i++) {
                        ostringstream buffer;
                        buffer << "midiChannel" << i+1;
                        settings.setValue(buffer.str(), preset->midiChannel.at(i));
                    }
                    for (int i = 0; i < preset->midiControlNumber.size(); i++) {
                        ostringstream buffer;
                        buffer << "midiControl" << i+1;
                        settings.setValue(buffer.str(), preset->midiControlNumber.at(i));
                    }

                    filename = "avUgen/" + avUgenNames.at(i) + ".xml";

                    settings.popTag(); //avUgen
                }

                settings.popTag(); //avUgens

                if (settings.saveFile(filename)) {
                    ofLogNotice() << "written presets +++++++++++++++++: " << " to file: " << filename << endl;
                } else {
                    ofLogError() << "unable to save "+ filename;
                }


            }
        }
    }
}