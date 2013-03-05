#pragma once

#include "avUgen.h"
#include "ofMain.h"
#include "ofxUI.h"

class testApp : public ofBaseApp{

    void setupUI();
    void setupMIDI();
    void setupAVUgens();
    void setupSound();
    void drawMIDI();
    
	public:

        static const int NUM_CHANNELS = 4;

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
        double wave[NUM_CHANNELS], sample, outputs[NUM_CHANNELS];

        ofxMaxiMix mix;
        ofxMaxiOsc mySine;    
        
        ofColor backgroundColor;
        std::vector<ofColor*> rgbHsb;
        std::vector<msp::avUgen*> channels;
        
        msp::avUgen *ch1, *ch2, *ch3, *ch4;

        ofxMidiIn midiIn;
        stringstream text;

};
