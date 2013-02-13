#pragma once

#include "ofMain.h"
#include "avUgen.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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

    float red, green, blue, alpha, width, height;
    
    int	initialBufferSize;
    int	sampleRate;
    double wave, wave2, sample,outputs[2];

    ofxMaxiMix mix;
    ofxMaxiOsc mySine;    
    
    ofColor backgroundColor;
    
    std::vector<msp::avUgen*> channels;
    
    msp::avUgen * ch1;
    msp::avUgen * ch2;
		
};
