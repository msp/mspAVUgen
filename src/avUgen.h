//
//  avUgen.h
//  mspUgen
//
//  Created by msp on 06/02/2013.
//
//

#ifndef mspUgen_avUgen_h
#define mspUgen_avUgen_h

#include "ofMain.h"

namespace msp {
    class avUgen {
    public:
        // Constructor
        avUgen();
        
        // Methods
        void moveTo(int _xDestiny, int _yDestiny);
        void draw();
        void update();
        
        // Properties
        int x;
        int y;
        int radius;
        ofColor rgbHsb;
    };
}

#endif
