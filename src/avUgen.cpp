//
//  avUgen.cpp
//  mspUgen
//
//  Created by msp on 06/02/2013.
//
//

#include "avUgen.h"

namespace msp {
    avUgen::avUgen(){

        x = ofRandom(ofGetWindowWidth());
        y = ofRandom(ofGetWindowHeight());
        radius = 150;
        throttle = 0;
        speed = 0;
        
        color.set( ofRandom(255), ofRandom(255), ofRandom(255), LIGHT_ALPHA);
        ofSetCircleResolution(100);
    }

    avUgen::avUgen(int _x, int _y, int _radius, ofColor _color, int _speed){
        
        x = _x;
        y = _y;
        radius = _radius;
        color = _color;
        speed = _speed;
        throttle = 0;
        ofSetCircleResolution(100);
    }
    
    void avUgen::moveTo(int _xDestiny, int _yDestiny){
        x += ( _xDestiny - x )*ofRandom(0.1, 1.0);
        y += ( _yDestiny - y )*ofRandom(0.1, 1.0);
    }
    
    void avUgen::draw(){
        ofSetColor(color.r, color.g, color.b, color.a);
        ofFill();
                
        if (throttle == speed) {
            cout << "firing! " << endl;
            ofSetCircleResolution(ofRandom(10));
            throttle = 0;
        } else {
            throttle++;
        }
        
        ofCircle(x, y, radius);

    }
    
    void avUgen::update(){
//        color.setHue(ofRandom(255));
    }
    
    void avUgen::setX(int _x) {
        x = _x;
    }

    void avUgen::setY(int _y){
        y = _y;
    }
    
    void avUgen::setRadius(int _radius){
        radius = _radius;
    }
    
//    TODO higher numbers slow it down. Make this clear.
    void avUgen::setSpeed(int _speed){
        speed = _speed;
    }
    
    void avUgen::setColor(ofColor _color){
        color = _color;
    }
    
    void avUgen::setOscillator(ofxMaxiOsc _osc){
        osc = _osc;
    }
    
    void avUgen::setFrequency(int _frequency){
        frequency = _frequency;
    }
    
    double avUgen::getAudio(){
        return osc.sinewave((double)color.getHue());
        
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
    }
};

