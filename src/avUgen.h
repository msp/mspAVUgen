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
#include "ofxMaxim.h"

namespace msp {
    class avUgen {
        
        // visual
        bool visualOutputSwitch, randomResolutionSwitch;
        int x, y, radius, speed, throttle;
        ofColor color;

        // audio
        bool audioOutputSwitch;
        int audioEngine;
        int frequency;
        int currentCount, lastCount, targetCount;
        double VCO1out,VCO2out,LFO1out,LFO2out,VCFout,ADSRout, audio;
        double adsrEnv[8]={0.5,100,0.5,250,0.125,125,0,500};

        ofxMaxiOsc VCO1,VCO2,LFO1,LFO2, osc;
        ofxMaxiFilter VCF;
        ofxMaxiEnvelope ADSR;
        ofxMaxiOsc timer;

        ofstream logger;
    public:
        
        ~avUgen();
        
        // Constants
        enum { LIGHT_ALPHA = 220, HEAVY_ALPHA = 50 };
        enum { SINE = 999, MONO = 998 };
        
        // Constructor
        avUgen();
        avUgen(int _x, int _y, int _radius, ofColor _color, int _speed);
        
        // Methods
        void moveTo(int _xDestiny, int _yDestiny);
        void draw();
        void update();
        
        // Mutators
        void setX(int _x);
        void setY(int _y);
        void setRadius(int _radius);
        void setSpeed(int _speed);
        void setColor(ofColor _color);
        void setOscillator(ofxMaxiOsc _osc);
        void setFrequency(int _frequency);
        void setAudioEngine(int _engine);
        void switchOffAudio();
        void switchOffVisual();
        void setRandomResolution();
        
        // Accessors
        double getAudio();
        bool isAudioOn();
        bool isVisualOn();
    };
}

#endif
