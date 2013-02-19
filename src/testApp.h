#pragma once

#include "ofMain.h"
#include "avUgen.h"
#include "ofxUI.h"

class testApp : public ofBaseApp{

	public:
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
    
        void audioRequested 	(float * input, int bufferSize, int nChannels); /* output method */
        void audioReceived 	(float * input, int bufferSize, int nChannels); /* input method */
    
        ofxUICanvas *gui;
        void guiEvent(ofxUIEventArgs &e);
        bool drawFill;

        float red, green, blue, alpha, width, height;
        
        int	initialBufferSize, sampleRate, radius_multiplier;
        double wave[4], sample, outputs[2];

        ofxMaxiMix mix;
        ofxMaxiOsc mySine;    
        
        ofColor backgroundColor;
        ofColor rgbHsb;
    
        std::vector<msp::avUgen*> channels;
        
        msp::avUgen *ch1, *ch2, *ch3, *ch4;
		
};
