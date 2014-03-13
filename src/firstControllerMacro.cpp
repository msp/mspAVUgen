//
//  firstControllerMacro.cpp
//  mspAVUgen
//
//  Created by msp on 12/03/2014.
//
//

#include "firstControllerMacro.h"
#include "avUgen.h"

namespace msp {
    double firstControllerMacro::applyRelative(msp::avUgen& _avUgen, int _value) {
        inspect();
        _avUgen.inspect();

        // how to do this when multiplers ?
        if (getThisValue() == -1) {
            setThisValue(_avUgen.getRadius());
        }

        incrementOrDecrement(_value);
        
        ofLogVerbose() << "setting radius: " << getThisValue()  << endl;
        _avUgen.setRadius(getThisValue());

        if (_avUgen.getAudioEngine() == avUgen::AM) {
            _avUgen.setVolume(sqrt(getThisValue()) / 2);
        } else {
            // get a non linear curve
            _avUgen.setVolume(sqrt(getThisValue()));
        }

        ofLogVerbose() << "current volume " << _avUgen.getVolume() << endl;

        setLastValue(getThisValue());
    }

    double firstControllerMacro::applyAbsolute(msp::avUgen& _avUgen, int _value) {
        _avUgen.setRadiusMIDI(_value);
    }

}