//
//  avUgen.h
//  mspAVUgen
//
//  Created by msp on 06/02/2013.
//
//

#ifndef mspAVUgen_avUgen_h
#define mspAVUgen_avUgen_h

#include "ofMain.h"
#include "math.h"
#include "ofxMaxim.h"
#include "ofxMidi.h"
#include "ofxXmlSettings.h"

namespace msp {
    class avUgen : public ofxMidiListener{

        bool debug = false;
        bool loadFromXML = false;
        bool saveToXML = false;

        string name;

        // visual
        bool visualOutputSwitch, randomResolutionSwitch, animateRadiusSwitch;
        int x, y, radius, throttle, frame;
        ofColor color;

        // audio
        bool audioOutputSwitch;
        int audioEngine;
        int frequency;
        double volume, lastMIDIRadius, pan;
        int currentCount, lastCount;
        double VCO1out,VCO2out,LFO1out,LFO2out,VCFout,ADSRout, audio;
        double adsrEnv[8]={0.5,100,0.5,250,0.125,125,0,500};

        ofxMaxiOsc VCO1,VCO2,LFO1,LFO2, osc, osc2, osc3;
        ofxMaxiFilter VCF;
        ofxMaxiEnvelope ADSR;
        ofxMaxiOsc timer;

        ofstream logger;

        string pseudoRandomName();
    public:
        
        ~avUgen();
        
        // Constants
        enum { LIGHT_ALPHA = 220, HEAVY_ALPHA = 50 };
        enum { FM = 999, MONO = 998, NOISE = 997, AM = 996 };

        static const int DEFAULT_RADIUS = 30;
        static const int DEFAULT_RADIUS_MULTPLIER = 10;
        static const int DEFAULT_VOLUME = 3;
        
        // Constructors
        avUgen();
        avUgen(string _name);
        avUgen(int _x, int _y, int _radius, ofColor _color);

        void initialize();
        
        // Methods
        void moveTo(int _xDestiny, int _yDestiny);
        void draw();
        void update();
        
        // Mutators
        void setX(int _x);
        void setY(int _y);
        void setPan(double _pan);
        void setRadius(int _radius);
        void setThrottle(int _throttle);
        void setColor(ofColor _color);
        void setOscillator(ofxMaxiOsc _osc);
        void setFrequency(int _frequency);
        void setAudioEngine(int _engine);
        void setVolume(double _volume);
        void switchOffAudio();
        void switchOffVisual();
        void setRandomResolutionSwitch(bool _randomResolutionSwitch);
        void setAnimateRadiusSwitch(bool _animateRadiusSwitch);
        void setVisualOutputSwitch(bool _visualOutputSwitch);
        void setAudioOutputSwitch(bool _audioOutputSwitch);

        void setMIDIMapping(int _channel, int _control);

        // MIDI Mutators
        void setXMIDI(int _x);
        void setYMIDI(int _y);
        void setPanMIDI(int _pan);
        void setRadiusMIDI(int _radius);
        void setThrottleMIDI(int _throttle);
        void setFrequencyMIDI(int _frequency);
        void setVolumeMIDI(int _volume);
        void setHueMIDI(int _hue);

        // Accessors
        string getName();
        int getX();
        int getY();
        double getPan();
        int getRadius();
        int getThrottle();
        ofColor getColor();
        int getFrequency();
        int getAudioEngine();
        double getVolume();
        bool getRandomResolutionSwitch();
        bool getAnimateRadiusSwitch();
        bool getVisualOutputSwitch();
        bool getAudioOutputSwitch();

        double getAudio();
        double getAudioOutput();
        
        bool isAudioOn();
        bool isVisualOn();
        bool isFireMIDI(ofxMidiMessage& msg);

        // ofxmidi
        void newMidiMessage(ofxMidiMessage& eventArgs);
        ofxMidiMessage midiMessage;
        std::vector<int> midiChannel;
        std::vector<int> midiControlNumber;
        std::vector<int> midiValue;

        void inspect();

    };
}

#endif
