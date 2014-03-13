//
//  fifthControllerMacro.cpp
//  mspAVUgen
//
//  Created by matt on 13/03/2014.
//
//

#include "fifthControllerMacro.h"
#include "avUgen.h"

namespace msp {
    double fifthControllerMacro::applyRelative(msp::avUgen& _avUgen, int _value) {
        inspect();
        _avUgen.inspect();

        if (getThisValue() == -1) {
            setThisValue(_avUgen.getPan() * 127);
        }

        incrementOrDecrement(_value);

        ofLogVerbose() << "setting ypos: " << getThisValue() << endl;
        _avUgen.setYMIDI(getThisValue());
    }

    double fifthControllerMacro::applyAbsolute(msp::avUgen& _avUgen, int _value) {
        ofLogVerbose() << "setting ypos: " << _value << endl;
        _avUgen.setYMIDI(_value);
    }
    
}