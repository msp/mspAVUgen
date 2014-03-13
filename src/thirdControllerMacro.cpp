//
//  thirdControllerMacro.cpp
//  mspAVUgen
//
//  Created by matt on 13/03/2014.
//
//

#include "thirdControllerMacro.h"
#include "avUgen.h"

namespace msp {
    double thirdControllerMacro::applyRelative(msp::avUgen& _avUgen, int _value) {
        inspect();
        _avUgen.inspect();

        if (getThisValue() == -1) {
            setThisValue(_avUgen.getThrottle());
        }

        incrementOrDecrement(_value);

        ofLogVerbose() << "setting throttle: " << getThisValue() << endl;
        _avUgen.setThrottleMIDI(getThisValue());

    }

    double thirdControllerMacro::applyAbsolute(msp::avUgen& _avUgen, int _value) {
        ofLogVerbose() << "setting throttle: " << _value << endl;
        _avUgen.setThrottleMIDI(_value);
    }
    
}