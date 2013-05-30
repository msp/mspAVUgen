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

        char buffer [80];

        for (int i = 0; i <= TOTAL_SLOTS; i++){
            for (int j = 0; j <= 3; j++){
                if (j == 0) {
                    presetSlots[i].push_back(new msp::avUgen(avUgenNames.at(i)));
                } else {

                    sprintf(buffer, "preset %d %d",i, j);
                    ofLogVerbose() << "preset name: " << buffer << endl;
                    presetSlots[i].push_back(new msp::avUgen());
                }
            }
        }

        ofLogVerbose() << presetSlots[0][0]-> getName();
        ofLogVerbose() << presetSlots[0][1]-> getName();
        ofLogVerbose() << presetSlots[0][2]-> getName();

        ofLogVerbose() << presetSlots[1][0]-> getName();
        ofLogVerbose() << presetSlots[1][1]-> getName();
        ofLogVerbose() << presetSlots[1][2]-> getName();

        ofLogVerbose() << presetSlots[2][0]-> getName();
        ofLogVerbose() << presetSlots[2][1]-> getName();
        ofLogVerbose() << presetSlots[2][2]-> getName();

        ofLogVerbose() << presetSlots[3][0]-> getName();
        ofLogVerbose() << presetSlots[3][1]-> getName();
        ofLogVerbose() << presetSlots[3][2]-> getName();

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

        if (currentPreset == TOTAL_SLOTS){
            currentPreset = 0;
        } else {
            currentPreset++;
        }

        ofLogVerbose() << "cyclePreset: activeSlots: " << activeSlots.size() << endl;
    };
}