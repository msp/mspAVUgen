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
        
        for (int i=0; i < avUgenNames.size(); i++){
            activeSlots.push_back(new msp::avUgen(avUgenNames.at(i)));
        }
    }
    
    soundBank::~soundBank(){
    }    
}