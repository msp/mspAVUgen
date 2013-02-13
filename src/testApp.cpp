#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    
    // msp::avUgen(s)    
    ch1 = new msp::avUgen();
    ch1->setX(width/2);
    ch1->setY(height/2);
    ch1->setRadius(100);
    ch1->setSpeed(10);
    ch1->setColor(*new ofColor(233, 52, 70, msp::avUgen::LIGHT_ALPHA));
    
    ch1->setFrequency(100);
    channels.push_back(ch1);
        
    ch2 = new msp::avUgen();
    ch2->setX(width/2 - 100);
    ch2->setY(height/2 - 100);
    ch2->setRadius(100);
    ch2->setSpeed(30);
    ch2->setColor(*new ofColor(100, 200, 100, msp::avUgen::LIGHT_ALPHA));
    
    ch2->setFrequency(101);
    channels.push_back(ch2);
    
    // OF Core
    red = 0;
    blue = 0;
    green = 0;
    backgroundColor = ofColor(red, green, blue);
        
    
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetFrameRate(30);    
    ofBackground(backgroundColor);
    
    // Maximillian
	sampleRate 			= 44100;
	initialBufferSize	= 512;
    
	ofSoundStreamSetup(2,0,this, sampleRate, initialBufferSize, 4);/* Call this last ! */
    ofSoundStreamListDevices();
    cout << "MSP2";
}

//--------------------------------------------------------------
void testApp::update(){
    for (int i=0; i<channels.size(); i++) {
        channels.at(i) -> update();
//        channels.at(i) -> moveTo(mouseX,mouseY);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
//    ofBackgroundGradient(ofColor::gray,ofColor(30,10,30), OF_GRADIENT_CIRCULAR);    
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    for (int i=0; i<channels.size(); i++) {
        ofPushStyle();
        channels.at(i) -> draw();
        ofPopStyle();
    }
    
//    ofCircle(200, 300, wave*150);
    
}

//--------------------------------------------------------------
void testApp::audioRequested 	(float * output, int bufferSize, int nChannels){
	
	for (int i = 0; i < bufferSize; i++){
		
		/* Stick your maximilian 'play()' code in here ! Declare your objects in testApp.h.
		 
		 For information on how maximilian works, take a look at the example code at
		 
		 http://www.maximilian.strangeloop.co.uk
		 
		 under 'Tutorials'.
		 
		 */
		
				
        //		sample=beat.play(0.25, 0, beat.length);
        //		wave=sine1.sinebuf(abs(mouseX));/* mouse controls sinewave pitch. we get abs value to stop it dropping
        //										 delow zero and killing the soundcard*/
        
        
        //      two tones
        //        wave = mySine.sinewave(440) + myOtherSine.sinewave(441); //these two sines will beat together. They're now a bit too loud though..
        
        //      AM1
        wave = ch1->getWaveform();
        wave2 = ch2->getWaveform();
//        wave = mySine.sinewave(110);
        
        //      AM2
//        wave = mySine.sinewave(440)*myOtherSine.sinewave(myPhasor.phasor(0.1,0,440));
		
        //      FM1
        //        wave = mySine.sinewave(myOtherSine.sinewave(1)*440);
        
        //      FM2
        //        wave = mySine.sinewave(myOtherSine.sinewave(myLastSine.sinewave(0.1)*30)*440);//awesome bassline
        
        //      Counting
        //        CurrentCount=myCounter.phasor(1, 1, 9);//phasor can take three arguments; frequency, start value and end value.
        //        wave = mySquare.square(CurrentCount*100);
        
        //      Envelopes
        
        //        myCurrentVolume=myEnvelope.line(4,myEnvelopeData);
        //
        //        CurrentCount=myCounter.phasor(1, 1, 9);//phasor can take three arguments; frequency, start value and end value.
        //
        //        if (CurrentCount<5)//simple if statement
        //
        //            myOscOutput=mySwitchableOsc.square(CurrentCount*100);
        //
        //        else if (CurrentCount>=5)//and the 'else' bit.
        //
        //            myOscOutput=mySwitchableOsc.saw(CurrentCount*50);//one osc object can produce whichever waveform you want.
        //
        //        if (CurrentCount==1)
        //
        //            myEnvelope.trigger(0,myEnvelopeData[0]); //trigger the envelope
        //
        //        wave = myOscOutput*myCurrentVolume;//point me at your speakers and fire.
        
        //      Filters
        //        CurrentCount=myCounter.phasor(1, 1, 9);//phasor can take three arguments; frequency, start value and end value.
        //
        //        if (CurrentCount<5)//simple if statement
        //
        //            myOscOutput=mySwitchableOsc.square(CurrentCount*10);
        //
        //        else if (CurrentCount>=5)//and the 'else' bit.
        //
        //            myOscOutput=mySwitchableOsc.saw(CurrentCount*5);//one osc object can produce whichever waveform you want.
        //
        //        if (CurrentCount==1)
        //
        //            myEnvelope.trigger(0,myEnvelopeData[0]); //trigger the envelope
        //
        //        myFilteredOutput=myFilter.lores(myOscOutput,(myEnvelope.line(6, myEnvelopeData)),10);//lores takes an audio input, a frequency and a resonance factor (1-100)
        //
        //        wave = myFilteredOutput;//point me at your speakers and fire.
        
        
        //      Monosynth
        //so this first bit is just a basic metronome so we can hear what we're doing.
        
//        currentCount=(int)timer.phasor(0.5);//this sets up a metronome that ticks every 2 seconds
//        
//        if (lastCount!=currentCount) {//if we have a new timer int this sample, play the sound
//            
//            ADSR.trigger(0, adsrEnv[0]);//trigger the envelope from the start
//            
//            cout << "tick\n";//the clock ticks
//            
//            lastCount=0;//set lastCount to 0
//        }
//        
//        //and this is where we build the synth
//        
//		
//        ADSRout=ADSR.line(8,adsrEnv);//our ADSR env has 8 value/time pairs.
//        
//        LFO1out=LFO1.sinebuf(0.2);//this lfo is a sinewave at 0.2 hz
//        
//        VCO1out=VCO1.sinewave(100);//here's VCO1. it's a pulse wave at 55 hz, with a pulse width of 0.6
//        VCO2out=VCO2.sinewave(80+LFO1out);//here's VCO2. it's a pulse wave at 110hz with LFO modulation on the frequency, and width of 0.2
//        
//        
//        VCFout=VCF.lores((VCO1out+VCO2out)*0.5, 250+(ADSRout*15000), 10);//now we stick the VCO's into the VCF, using the ADSR as the filter cutoff
//        
//        //        wave = VCFout*ADSRout;//finally we add the ADSR as an amplitude modulator
//        wave = VCO1out*ADSRout;//finally we add the ADSR as an amplitude modulator
        
        
        //		mymix.stereo(sample + wave, outputs, 0.5);
		mix.stereo(wave + wave2, outputs, 0.5);
        
        
		
		output[i*nChannels    ] = outputs[0]; /* You may end up with lots of outputs. add them here */
		output[i*nChannels + 1] = outputs[1];
	}
	
}

//--------------------------------------------------------------
void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){	
	
	
	/* You can just grab this input and stick it in a double, then use it above to create output*/
	
	for (int i = 0; i < bufferSize; i++){
		
		/* you can also grab the data out of the arrays*/
		
		
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}