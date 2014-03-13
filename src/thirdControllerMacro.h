//
//  thirdControllerMacro.h
//  mspAVUgen
//
//  Created by msp on 13/03/2014.
//
//

#ifndef __mspAVUgen__thirdControllerMacro__
#define __mspAVUgen__thirdControllerMacro__

#include "controllerMacro.h"

namespace msp {
    class avUgen; // forward declaration
    class thirdControllerMacro : public controllerMacro {
    public:
        double applyRelative(avUgen& _avUgen, int _value);
        double applyAbsolute(avUgen& _avUgen, int _value);
    };
}
#endif
