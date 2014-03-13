//
//  controllerMacro.cpp
//  mspAVUgen
//
//  Created by msp on 12/03/2014.
//
//

#include "controllerMacro.h"

namespace msp {

    void controllerMacro::setLastValue(double _last) {
        lastValue = _last;
    }

    void controllerMacro::setThisValue(double _this) {
        thisValue = _this;
    }

    void controllerMacro::incrementOrDecrement(int _value) {
        if (_value >= 1 && _value <= 6) {
            if (getLastValue() < 127) {
                setThisValue(getLastValue() + 1);
            }
        } else if (_value >= 122 && _value <= 127) {
            if (getLastValue() > 0) {
                setThisValue(getLastValue() - 1);
            }
        }
    }

    double controllerMacro::getLastValue() {
        if (lastValue >=0) {
            return lastValue;
        } else {
            return thisValue;
        }
    }

    double controllerMacro::getThisValue() {
        return thisValue;
    }

    void controllerMacro::inspect() {
        
        ofLogNotice() << "controllerMacro: "
        << "thisValue: " << thisValue
        << ", lastValue: " << lastValue
        << endl;
    }
}