//
//  secondControllerMacro.h
//  mspAVUgen
//
//  Created by msp on 12/03/2014.
//
//

#ifndef __mspAVUgen__secondControllerMacro__
#define __mspAVUgen__secondControllerMacro__

#include "controllerMacro.h"


namespace msp {
    class avUgen; // forward declaration
    class secondControllerMacro : public controllerMacro {
    public:
        double applyRelative(avUgen& _avUgen, int _value);
        double applyAbsolute(avUgen& _avUgen, int _value);
    };
}

#endif
