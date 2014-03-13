//
//  secondControllerMacro.cpp
//  mspAVUgen
//
//  Created by msp on 12/03/2014.
//
//

#include "secondControllerMacro.h"
#include "avUgen.h"

namespace msp {
    double secondControllerMacro::applyRelative(msp::avUgen& _avUgen, int _value) {
        inspect();
        _avUgen.inspect();

        // how to do this when multiplers ?
        if (getThisValue() == -1) {
            setThisValue(_avUgen.getRadius());
        }

        incrementOrDecrement(_value);

        _avUgen.setHueMIDI(getThisValue());
    }

    double secondControllerMacro::applyAbsolute(msp::avUgen& _avUgen, int _value) {
        _avUgen.setHueMIDI(_value);
    }

}