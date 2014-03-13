//
//  firstControllerMacro.h
//  mspAVUgen
//
//  Created by msp on 12/03/2014.
//
//

#ifndef __mspAVUgen__firstControllerMacro__
#define __mspAVUgen__firstControllerMacro__

#include "controllerMacro.h"


namespace msp {
    class avUgen; //forward declaration
    class firstControllerMacro : public controllerMacro {
    public:
        double applyRelative(avUgen& _avUgen, int _value);
        double applyAbsolute(avUgen& _avUgen, int _value);
    };
}

#endif
