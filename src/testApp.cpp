#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);

    red = 0;
    blue = 0;
    green = 0;
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    backgroundColor = ofColor(red, green, blue);

    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofSetFrameRate(30);
    ofBackground(backgroundColor);

    setupAVUgens();

    // The UI components use this vector of colours
    for (int i=0; i<soundBank.activeSlots.size(); i++) {
        rgbHsb.push_back(new ofColor(red, green, blue));
    }

    setupUI();
    setupMIDI();
    setupSound(); /* Call this last ! */

    if (debug) cout << "MSP End OF Setup" << endl;
}

//--------------------------------------------------------------
void testApp::update(){
    for (int i=0; i<soundBank.activeSlots.size(); i++) {
        soundBank.activeSlots.at(i) -> update();
        //        soundBank.activeSlots.at(i) -> moveTo(mouseX,mouseY);

    }
}

//--------------------------------------------------------------
void testApp::draw(){

	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    for (int i=0; i<soundBank.activeSlots.size(); i++) {
        ofPushStyle();

        if (debug) cout << "MSP wave[" << i << "]:" << wave[i] << endl;

        soundBank.activeSlots.at(i) -> draw();

        ofPopStyle();
    }

    ofPopStyle();
    if (debug) drawMIDI();
}

//--------------------------------------------------------------
void testApp::audioRequested 	(float * output, int bufferSize, int nChannels){

    float panLeft = 0.0;
    float panRight = 1.0;

	for (int i = 0; i < bufferSize; i++){

        if (soundBank.audioReady){

            for (int j = 0; j < NUM_SOUNDCARD_CHANNELS; j++){
                if (soundBank.audioReady && soundBank.activeSlots.at(j)->isAudioOn() == true){
                    wave[j] = soundBank.activeSlots.at(j)->getAudioOutput();
                }
            }

            if (soundBank.audioReady && soundBank.activeSlots.at(0)-> isAudioOn()) {
                mix.stereo(wave[0] + wave[2] / NUM_SOUNDCARD_CHANNELS, outputs, panLeft);
                output[i*nChannels    ] = outputs[0];
            }

            if (soundBank.audioReady && soundBank.activeSlots.at(1)-> isAudioOn()) {
                mix.stereo(wave[1] + wave[3] / NUM_SOUNDCARD_CHANNELS, outputs, panRight);
                output[i*nChannels + 1] = outputs[1];
            }

            //        if (soundBank.audioReady && soundBank.activeSlots.at(2)-> isAudioOn()) {
            //            mix.stereo(wave[2] / NUM_SOUNDCARD_CHANNELS, outputs, panLeft);
            //            output[i*nChannels + 2] = outputs[0];
            //        }
            //        if (soundBank.audioReady && soundBank.activeSlots.at(3)-> isAudioOn()) {
            //            mix.stereo(wave[3] / NUM_SOUNDCARD_CHANNELS, outputs, panRight);
            //            output[i*nChannels + 3] = outputs[1];
            //
            //        }

        }
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
void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();

	if(name == "RED")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		red = slider->getScaledValue();
        rgbHsb.at(0) -> r = red;
	}
	else if(name == "GREEN")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		green = slider->getScaledValue();
        rgbHsb.at(0) -> g = green;
	}
	else if(name == "BLUE")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		blue = slider->getScaledValue();
        rgbHsb.at(0) -> b = blue;
	}
	else if(name == "HUE")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        rgbHsb.at(0) -> setHue(slider->getScaledValue());
	}
	else if(name == "SATURATION")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        rgbHsb.at(0) -> setSaturation(slider->getScaledValue());
	}
	else if(name == "BRIGHTNESS")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        rgbHsb.at(0) -> setBrightness(slider->getScaledValue());
	}
	else if(name == "FRAMERATE")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        ofSetFrameRate(slider->getScaledValue());
	}
	else if(name == "BGR")
	{
		ofxUISlider *rslider = (ofxUISlider *) e.widget;
		backgroundColor.r = rslider->getScaledValue();
	}
	else if(name == "BGG")
	{
		ofxUISlider *rslider = (ofxUISlider *) e.widget;
		backgroundColor.g = rslider->getScaledValue();
	}
	else if(name == "BGB")
	{
		ofxUISlider *rslider = (ofxUISlider *) e.widget;
		backgroundColor.b = rslider->getScaledValue();
	}
	else if(name == "ALPHA")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		alpha = slider->getScaledValue();
        rgbHsb.at(0) -> a = alpha;
	}
    //	else if(name == "RADIUS")
    //	{
    //		ofxUISlider *slider = (ofxUISlider *) e.widget;
    //		soundBank.activeSlots.at(0)->setRadius(slider->getScaledValue());
    //	}
    //	else if(name == "RESOLUTION")
    //	{
    //		ofxUISlider *slider = (ofxUISlider *) e.widget;
    //		resolution = slider->getScaledValue();          //gets the value from the specified range, otherwise you can get a normalized value (0.0 -> 1.0);
    //        ofSetCircleResolution(resolution);
    //        slider->setValue(resolution);                   //shows the int value on the slider
    //	}
    else if(name == "POSITION_CH1")
	{
		ofxUI2DPad *pad = (ofxUI2DPad *) e.widget;
		soundBank.activeSlots.at(0) -> setX(pad->getPercentValue().x*ofGetWidth());
		soundBank.activeSlots.at(0) -> setY(pad->getPercentValue().y*ofGetHeight());
	}
    else if(name == "POSITION_CH2")
	{
		ofxUI2DPad *pad = (ofxUI2DPad *) e.widget;
		soundBank.activeSlots.at(1) -> setX(pad->getPercentValue().x*ofGetWidth());
		soundBank.activeSlots.at(1) -> setY(pad->getPercentValue().y*ofGetHeight());
	}
    else if(name == "DRAW FILL")
    {
        ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
        drawFill = toggle->getValue();
    }
}

//--------------------------------------------------------------
void testApp::exit()
{
    gui->saveSettings("GUI/guiSettings.xml");
    delete gui;

	midiIn.closePort();

    for(int i=0; i<soundBank.activeSlots.size(); i++){
        midiIn.removeListener(soundBank.activeSlots.at(i));
//        soundBank.saveActivePresetToXML();
    }
}

//--------------------------------------------------------------
void testApp::setupAVUgens(){

    std::vector<string> avUgenNames;
    avUgenNames.push_back("msp0");
    avUgenNames.push_back("msp1");
    avUgenNames.push_back("msp2");
    avUgenNames.push_back("msp3");

    soundBank = *new msp::soundBank(avUgenNames);

    ofLogVerbose() << "soundBank.activeSlots: " << soundBank.activeSlots.size() << endl;

    /*************************************/
    soundBank.activeSlots.at(0)->setMIDIMapping(14,100);
    soundBank.activeSlots.at(0)->setMIDIMapping(14,80);
    soundBank.activeSlots.at(0)->setMIDIMapping(14,60);
    soundBank.activeSlots.at(0)->setMIDIMapping(14,40);

    /*************************************/
    soundBank.activeSlots.at(1)->setMIDIMapping(14,101);
    soundBank.activeSlots.at(1)->setMIDIMapping(14,81);
    soundBank.activeSlots.at(1)->setMIDIMapping(14,61);
    soundBank.activeSlots.at(1)->setMIDIMapping(14,41);

    /*************************************/
    soundBank.activeSlots.at(2)->setMIDIMapping(14,102);
    soundBank.activeSlots.at(2)->setMIDIMapping(14,82);
    soundBank.activeSlots.at(2)->setMIDIMapping(14,62);
    soundBank.activeSlots.at(2)->setMIDIMapping(14,42);

    /*************************************/
    soundBank.activeSlots.at(3)->setMIDIMapping(14,103);
    soundBank.activeSlots.at(3)->setMIDIMapping(14,83);
    soundBank.activeSlots.at(3)->setMIDIMapping(14,63);
    soundBank.activeSlots.at(3)->setMIDIMapping(14,43);

    /*************************************/
    //    soundBank.activeSlots.at(4)->setMIDIMapping(14,104);
    //    soundBank.activeSlots.at(4)->setMIDIMapping(14,84);
    //    soundBank.activeSlots.at(4)->setMIDIMapping(14,64);
    //    soundBank.activeSlots.at(4)->setMIDIMapping(14,44);

    /*************************************/

    ofLogVerbose() << "Done setupAVUgens" << endl;

}

//--------------------------------------------------------------
void testApp::setupMIDI(){
    // ofxMidi

	midiIn.listPorts(); // via instance
	//ofxMidiIn::listPorts(); // via static as well

    //	midiIn.openPort("Audio 8 DJ MIDI input port 0");
	midiIn.openPort("USB Uno MIDI Interface");

	//midiIn.openVirtualPort("ofxMidiIn Input");	// open a virtual port

	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);

    // add testApp as a listener
    //	midiIn.addListener(this);
    // add each avUgen as a midi listener
    for(int i=0; i < soundBank.activeSlots.size(); i++){
        midiIn.addListener(soundBank.activeSlots.at(i));
    }

	// print received messages to the console
	midiIn.setVerbose(true);

    ofLogVerbose() << "Done setupMIDI" << endl;
}

//--------------------------------------------------------------
void testApp::setupUI(){
    // OF UI
    float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 320-xInit;

    gui = new ofxUICanvas(0,0,length+xInit*2.0,ofGetHeight());
	gui->addWidgetDown(new ofxUILabel("SPATIAL - AVUGEN", OFX_UI_FONT_LARGE));

    gui->addSpacer(length, 2);
    gui->addWidgetDown(new ofxUILabel("BACKGROUND CONTROL", OFX_UI_FONT_MEDIUM));
    gui->addSlider("BGR", 0, 255, backgroundColor.r, 95, dim);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addSlider("BGG", 0, 255, backgroundColor.g, 95, dim);
    gui->addSlider("BGB", 0, 255, backgroundColor.b, 95, dim);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);

    gui->addWidgetDown(new ofxUILabel("CIRCLE CONTROL", OFX_UI_FONT_MEDIUM));
    gui->addSlider("HUE", 0.0, 255.0, rgbHsb.at(0)->getHue(), length,dim);
    gui->addSlider("RED", 0.0, 255.0, rgbHsb.at(0)->r, length,dim);
	gui->addSlider("GREEN", 0.0, 255.0, rgbHsb.at(0)->g, length,dim);
    gui->addSlider("BLUE", 0.0, 255.0, rgbHsb.at(0)->b, length,dim);
    gui->addSlider("ALPHA", 0.0, 255.0, rgbHsb.at(0)->a, length,dim);
    //    gui->addSlider("RADIUS", 0.0, 600.0, soundBank.activeSlots.at(0)->getRadius(), length,dim);
    //	gui->addSlider("RESOLUTION", 3, 60, resolution, length,dim);

    gui->addSpacer(length, 2);

    gui->addSpacer(length, 2);
    gui->addSlider("FRAMERATE", 1, 1000, 24, length,dim);
    gui->addWidgetDown(new ofxUILabelToggle(drawFill, "DRAW FILL", OFX_UI_FONT_MEDIUM));

    float padWidth = length;
    float padHeight = length*((float)ofGetHeight()/(float)ofGetWidth());

    gui->addWidgetDown(new ofxUI2DPad(padWidth, padHeight, ofPoint(padWidth*.5, padHeight*.5), "POSITION_CH1"));
    gui->addWidgetDown(new ofxUI2DPad(padWidth, padHeight, ofPoint(padWidth*.5, padHeight*.5), "POSITION_CH2"));

    gui->addSpacer(length, 2);
    gui->addWidgetDown(new ofxUILabel("HIDE & SHOW GUI BY PRESSING 'g'", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUILabel("MOUSE OVER A SLIDER AND", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUILabel("PRESS UP, DOWN, LEFT, RIGHT", OFX_UI_FONT_MEDIUM));

    //    switch off until we need the UI, it interferes with our serialized state
    //    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);

    gui->loadSettings("GUI/guiSettings.xml");
    gui->toggleVisible();

    ofLogVerbose() << "Done setupUI" << endl;

}

//--------------------------------------------------------------
void testApp::setupSound() {
    // Maximillian
    sampleRate 			= 44100;
    initialBufferSize	= 512;

    soundStream.listDevices();
    // soundStream.setDeviceID(5);
    soundStream.setup(this, NUM_SOUNDCARD_CHANNELS, 0, sampleRate, initialBufferSize, 4); /* Call this last ! */
    ofLogVerbose() << "Done setupSound" << endl;
}

void testApp::drawMIDI() {

    for(int i=0; i<soundBank.activeSlots.size(); i++){
        // draw the last recieved message contents to the screen
        text << "Received: " << ofxMidiMessage::getStatusString(soundBank.activeSlots.at(i) -> midiMessage.status);
        ofDrawBitmapString(text.str(), 20, 20);
        text.str(""); // clear

        text << "channel: " << soundBank.activeSlots.at(i) -> midiMessage.channel;
        ofDrawBitmapString(text.str(), 20, 34);
        text.str(""); // clear

        text << "pitch: " << soundBank.activeSlots.at(i) -> midiMessage.pitch;
        ofDrawBitmapString(text.str(), 20, 48);
        text.str(""); // clear
        ofRect(20, 58, ofMap(soundBank.activeSlots.at(i) -> midiMessage.pitch, 0, 127, 0, ofGetWidth()-40), 20);

        text << "velocity: " << soundBank.activeSlots.at(i) -> midiMessage.velocity;
        ofDrawBitmapString(text.str(), 20, 96);
        text.str(""); // clear
        ofRect(20, 105, ofMap(soundBank.activeSlots.at(i) -> midiMessage.velocity, 0, 127, 0, ofGetWidth()-40), 20);

        text << "control: " << soundBank.activeSlots.at(i) -> midiMessage.control;
        ofDrawBitmapString(text.str(), 20, 144);
        text.str(""); // clear
        ofRect(20, 154, ofMap(soundBank.activeSlots.at(i) -> midiMessage.control, 0, 127, 0, ofGetWidth()-40), 20);

        text << "value: " << soundBank.activeSlots.at(i) -> midiMessage.value;
        ofDrawBitmapString(text.str(), 20, 192);
        text.str(""); // clear
        if(soundBank.activeSlots.at(i) -> midiMessage.status == MIDI_PITCH_BEND) {
            ofRect(20, 202, ofMap(soundBank.activeSlots.at(i) -> midiMessage.value, 0, MIDI_MAX_BEND, 0, ofGetWidth()-40), 20);
        }
        else {
            ofRect(20, 202, ofMap(soundBank.activeSlots.at(i) -> midiMessage.value, 0, 127, 0, ofGetWidth()-40), 20);
        }

        text << "delta: " << soundBank.activeSlots.at(i) -> midiMessage.deltatime;
        ofDrawBitmapString(text.str(), 20, 240);
        text.str(""); // clear
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key)
    {
        case 'g':
            gui->toggleVisible();
            break;
		case 'l':
			midiIn.listPorts();
			break;
		case 's':
			soundBank.saveActivePresetToXML();
			break;
		case 'p':

            for(int i = 0; i<soundBank.activeSlots.size(); i++) {
                midiIn.removeListener(soundBank.activeSlots.at(i));
            }

			soundBank.cyclePreset();

            /*************************************/
            soundBank.activeSlots.at(0)->setMIDIMapping(14,100);
            soundBank.activeSlots.at(0)->setMIDIMapping(14,80);
            soundBank.activeSlots.at(0)->setMIDIMapping(14,60);
            soundBank.activeSlots.at(0)->setMIDIMapping(14,40);

            /*************************************/
            soundBank.activeSlots.at(1)->setMIDIMapping(14,101);
            soundBank.activeSlots.at(1)->setMIDIMapping(14,81);
            soundBank.activeSlots.at(1)->setMIDIMapping(14,61);
            soundBank.activeSlots.at(1)->setMIDIMapping(14,41);

            /*************************************/
            soundBank.activeSlots.at(2)->setMIDIMapping(14,102);
            soundBank.activeSlots.at(2)->setMIDIMapping(14,82);
            soundBank.activeSlots.at(2)->setMIDIMapping(14,62);
            soundBank.activeSlots.at(2)->setMIDIMapping(14,42);

            /*************************************/
            soundBank.activeSlots.at(3)->setMIDIMapping(14,103);
            soundBank.activeSlots.at(3)->setMIDIMapping(14,83);
            soundBank.activeSlots.at(3)->setMIDIMapping(14,63);
            soundBank.activeSlots.at(3)->setMIDIMapping(14,43);

            /*************************************/
            //    soundBank.activeSlots.at(4)->setMIDIMapping(14,104);
            //    soundBank.activeSlots.at(4)->setMIDIMapping(14,84);
            //    soundBank.activeSlots.at(4)->setMIDIMapping(14,64);
            //    soundBank.activeSlots.at(4)->setMIDIMapping(14,44);
            
            /*************************************/

            for(int i = 0; i<soundBank.activeSlots.size(); i++) {
                midiIn.addListener(soundBank.activeSlots.at(i));
            }

			break;

        default:
            break;

    }
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