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
        int x, y, radius, speed, throttle;
        ofColor rgbHsb;
    public:

        enum { LIGHT_ALPHA = 220, HEAVY_ALPHA = 50 };
        
        // Constructor
        avUgen();
        avUgen(int _x, int _y, int _radius, ofColor _color, int _speed);
        
        // Methods
        void moveTo(int _xDestiny, int _yDestiny);
        void draw();
        void update();
        
        // Accessors
        void setX(int _x);
        void setY(int _y);
        void setRadius(int _radius);
        void setSpeed(int _speed);
        void setColor(ofColor _color);
    };
}

#endif
