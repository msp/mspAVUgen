//
//  controllerMacro.h
//  mspAVUgen
//
//  Created by msp on 12/03/2014.
//
//

#ifndef mspAVUgen_controllerMacro_h
#define mspAVUgen_controllerMacro_h

#include "ofMain.h"

namespace msp {
    class avUgen; // forward declaration
    class controllerMacro {
        public:
            // pure virtual function
            virtual double applyRelative(avUgen& _avUgen, int _value) = 0;
            virtual double applyAbsolute(avUgen& _avUgen, int _value) = 0;

            // Mutators
            void setLastValue(double _last);
            void setThisValue(double _this);
            void incrementOrDecrement(int _value);

            // Accessors
            double getLastValue();
            double getThisValue();
            void inspect();

        private:
            double lastValue = -1;
            double thisValue = -1;
        };
}

#endif
