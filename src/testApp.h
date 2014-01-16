#pragma once

#include "soundBank.h"
#include "avUgen.h"
#include "ofMain.h"
#include "ofxUI.h"
#include "ofxMidi.h"

class testApp : public ofBaseApp, public ofxMidiListener{

    void setupUI();
    void setupMIDI();
    void setupAVUgens();
    void setupSound();
    void drawMIDI();
    void bindMIDI();
    void cyclePreset();
    
	public:

        static const int NUM_SOUNDCARD_CHANNELS = 2;
        static const int NUM_UGENS = 4;

        bool debug = false;

		void setup();
		void update();
		void draw();
        void exit();    

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        ofSoundStream soundStream;
        void audioRequested 	(float * input, int bufferSize, int nChannels); /* output method */
        void audioReceived 	(float * input, int bufferSize, int nChannels); /* input method */
    
        ofxUICanvas *gui;
        void guiEvent(ofxUIEventArgs &e);
        bool drawFill;

        float red, green, blue, alpha, width, height;

        int	initialBufferSize, sampleRate, solo;
        double wave[NUM_UGENS], sample;
        double slot1Wave[512], slot2Wave[512], slot3Wave[512], slot4Wave[512];
        double slot1Out[2], slot2Out[2], slot3Out[2], slot4Out[2];

        ofxMaxiMix mix;
        ofxMaxiOsc mySine;    
        
        ofColor backgroundColor;
        std::vector<ofColor*> rgbHsb;
        std::vector<msp::avUgen*> channels;

        msp::soundBank soundBank;
        msp::avUgen *ch1, *ch2, *ch3, *ch4;

        ofxMidiIn midiIn;
        stringstream text;

        // ofxmidi
        void newMidiMessage(ofxMidiMessage& eventArgs);
        ofxMidiMessage midiMessage;
        bool isFireMIDI(ofxMidiMessage& msg);


};
