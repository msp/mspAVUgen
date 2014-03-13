//
//  fourthControllerMacro.cpp
//  mspAVUgen
//
//  Created by matt on 13/03/2014.
//
//

#include "fourthControllerMacro.h"
#include "avUgen.h"

namespace msp {
    double fourthControllerMacro::applyRelative(msp::avUgen& _avUgen, int _value) {
        inspect();
        _avUgen.inspect();

        if (getThisValue() == -1) {
            setThisValue(_avUgen.getPan() * 127);
        }

        incrementOrDecrement(_value);

        ofLogVerbose() << "setting pan: " << getThisValue() << endl;
        _avUgen.setXMIDI(getThisValue());
    }

    double fourthControllerMacro::applyAbsolute(msp::avUgen& _avUgen, int _value) {
        ofLogVerbose() << "setting pan: " << _value << endl;
        _avUgen.setXMIDI(_value);
    }
    
}