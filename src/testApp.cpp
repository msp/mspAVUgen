#include "testApp.h"
#include <stdlib.h>

//--------------------------------------------------------------
void testApp::setup(){

    ofSetLogLevel(OF_LOG_VERBOSE);

    if (isRunningStandalone()) {
        ofLogNotice() << "STANDALONE mode is for distribution and resets data path and log file." << endl;
        ofSetDataPathRoot("../Resources/");
        ofLogToFile("mspAVUgen.log", true);
    }

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

    ofLogNotice() << "Done setup" << endl;
}

//--------------------------------------------------------------
void testApp::update(){
    for (int i=0; i<soundBank.activeSlots.size(); i++) {
        soundBank.activeSlots.at(i) -> update();
    }

    if (soundBank.audioReady == false) {
        ofLogNotice() << "soundBank.audioReady: " << soundBank.audioReady << endl;
    }

    if (soundBank.audioReady == true && soundBank.activeSlots.size() == NUM_UGENS) {

        // TODO wrap up controller / ui state in class.
        // Use Macro concept from Live? Here & setupMIDI()

        // Also, get a wierd crash when switching presets *really* fast with
        // activeSlots length changing and we fall off the end of the array.
        // Disable for LIVE performance?
        if (isRunningStandalone()) {
            rad0 -> setValue(soundBank.activeSlots.at(0)->getRadius());
            rad1 -> setValue(soundBank.activeSlots.at(1)->getRadius());
            rad2 -> setValue(soundBank.activeSlots.at(2)->getRadius());
            rad3 -> setValue(soundBank.activeSlots.at(3)->getRadius());

            hue0 -> setValue(soundBank.activeSlots.at(0)->getColor().getHue());
            hue1 -> setValue(soundBank.activeSlots.at(1)->getColor().getHue());
            hue2 -> setValue(soundBank.activeSlots.at(2)->getColor().getHue());
            hue3 -> setValue(soundBank.activeSlots.at(3)->getColor().getHue());

            thr0 -> setValue(soundBank.activeSlots.at(0)->getThrottle());
            thr1 -> setValue(soundBank.activeSlots.at(1)->getThrottle());
            thr2 -> setValue(soundBank.activeSlots.at(2)->getThrottle());
            thr3 -> setValue(soundBank.activeSlots.at(3)->getThrottle());

            pos0 -> setValue(soundBank.activeSlots.at(0)->getPan() * 127);
            pos1 -> setValue(soundBank.activeSlots.at(1)->getPan() * 127);
            pos2 -> setValue(soundBank.activeSlots.at(2)->getPan() * 127);
            pos3 -> setValue(soundBank.activeSlots.at(3)->getPan() * 127);

            ypos0 -> setValue(soundBank.activeSlots.at(0)->getY());
            ypos1 -> setValue(soundBank.activeSlots.at(1)->getY());
            ypos2 -> setValue(soundBank.activeSlots.at(2)->getY());
            ypos3 -> setValue(soundBank.activeSlots.at(3)->getY());
        }
    }


    for (int i=0; i<presetButtons.size(); i++) {
        if (i == soundBank.currentPreset) {
            presetButtons.at(i) -> setState(OFX_UI_STATE_DOWN);
        } else {
            presetButtons.at(i) -> setState(OFX_UI_STATE_NORMAL);
        }
        presetButtons.at(i) -> stateChange();
    }
}

//--------------------------------------------------------------
void testApp::draw(){

	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    for (int i=0; i<soundBank.activeSlots.size(); i++) {
        ofPushStyle();
        soundBank.activeSlots.at(i) -> draw();
        ofPopStyle();
    }

    ofPopStyle();
    if (debug) drawMIDI();
}

//--------------------------------------------------------------

void testApp::audioRequested(float * output, int bufferSize, int nChannels){

    float panLeft = 0.0;
    float panRight = 1.0;

	for (int i = 0; i < bufferSize; i++){
        if (soundBank.audioReady){

            for (int j = 0; j < NUM_UGENS; j++){
                if (soundBank.audioReady && soundBank.activeSlots.at(j)->isAudioOn() == true){
                    wave[j] = soundBank.activeSlots.at(j)->getAudioOutput();
                }
            }

            if (soundBank.audioReady) {
                mix.stereo(wave[0] / NUM_SOUNDCARD_CHANNELS, slot1Out, soundBank.activeSlots.at(0)->getPan());
                mix.stereo(wave[1] / NUM_SOUNDCARD_CHANNELS, slot2Out, soundBank.activeSlots.at(1)->getPan());
                mix.stereo(wave[2] / NUM_SOUNDCARD_CHANNELS, slot3Out, soundBank.activeSlots.at(2)->getPan());
                mix.stereo(wave[3] / NUM_SOUNDCARD_CHANNELS, slot4Out, soundBank.activeSlots.at(3)->getPan());
            }

            if (NUM_SOUNDCARD_CHANNELS == 4) {
                if (soundBank.audioReady) {
                    output[i*nChannels    ] = (slot1Out[0] + slot2Out[0]) / 2.0;
                    output[i*nChannels  +1] = (slot1Out[1] + slot2Out[1]) / 2.0;
                    output[i*nChannels  +2] = (slot3Out[0] + slot4Out[0]) / 2.0;
                    output[i*nChannels  +3] = (slot3Out[1] + slot4Out[1]) / 2.0;
                }
            } else if (NUM_SOUNDCARD_CHANNELS == 2) {
                if (soundBank.audioReady) {
                    output[i*nChannels    ] = (slot1Out[0] + slot2Out[0] + slot3Out[0] + slot4Out[0]) / 6.0;
                    output[i*nChannels  +1] = (slot1Out[1] + slot2Out[1] + slot3Out[1] + slot4Out[1]) / 6.0;
                }

            }

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

	if(name == "RAD0")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(0)->controllerMacros.at(0)->applyAbsolute(*soundBank.activeSlots.at(0), slider->getScaledValue());
	}
	else if(name == "RAD1")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(1)->controllerMacros.at(0)->applyAbsolute(*soundBank.activeSlots.at(1), slider->getScaledValue());
	}
	else if(name == "RAD2")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(2)->controllerMacros.at(0)->applyAbsolute(*soundBank.activeSlots.at(2), slider->getScaledValue());
	}
	else if(name == "RAD3")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(3)->controllerMacros.at(0)->applyAbsolute(*soundBank.activeSlots.at(3), slider->getScaledValue());
	}
	else if(name == "HUE0")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(0)->controllerMacros.at(1)->applyAbsolute(*soundBank.activeSlots.at(0), slider->getScaledValue());
	}
	else if(name == "HUE1")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(1)->controllerMacros.at(1)->applyAbsolute(*soundBank.activeSlots.at(1), slider->getScaledValue());
	}
	else if(name == "HUE2")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(2)->controllerMacros.at(1)->applyAbsolute(*soundBank.activeSlots.at(2), slider->getScaledValue());
	}
	else if(name == "HUE3")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(3)->controllerMacros.at(1)->applyAbsolute(*soundBank.activeSlots.at(3), slider->getScaledValue());
	}
	else if(name == "THR0")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(0)->controllerMacros.at(2)->applyAbsolute(*soundBank.activeSlots.at(0), slider->getScaledValue());
	}
	else if(name == "THR1")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(1)->controllerMacros.at(2)->applyAbsolute(*soundBank.activeSlots.at(1), slider->getScaledValue());
	}
	else if(name == "THR2")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(2)->controllerMacros.at(2)->applyAbsolute(*soundBank.activeSlots.at(2), slider->getScaledValue());
	}
	else if(name == "THR3")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(3)->controllerMacros.at(2)->applyAbsolute(*soundBank.activeSlots.at(3), slider->getScaledValue());
	}
	else if(name == "POS0")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(0)->controllerMacros.at(3)->applyAbsolute(*soundBank.activeSlots.at(0), slider->getScaledValue());
	}
	else if(name == "POS1")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(1)->controllerMacros.at(3)->applyAbsolute(*soundBank.activeSlots.at(1), slider->getScaledValue());
	}
	else if(name == "POS2")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(2)->controllerMacros.at(3)->applyAbsolute(*soundBank.activeSlots.at(2), slider->getScaledValue());
	}
	else if(name == "POS3")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(3)->controllerMacros.at(3)->applyAbsolute(*soundBank.activeSlots.at(3), slider->getScaledValue());
	}
	else if(name == "YPOS0")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(0)->controllerMacros.at(4)->applyAbsolute(*soundBank.activeSlots.at(0), slider->getScaledValue());
	}
	else if(name == "YPOS1")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(1)->controllerMacros.at(4)->applyAbsolute(*soundBank.activeSlots.at(1), slider->getScaledValue());
	}
	else if(name == "YPOS2")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(2)->controllerMacros.at(4)->applyAbsolute(*soundBank.activeSlots.at(2), slider->getScaledValue());
	}
	else if(name == "YPOS3")
	{
		ofxUIRotarySlider *slider = (ofxUIRotarySlider *) e.widget;
        soundBank.activeSlots.at(3)->controllerMacros.at(4)->applyAbsolute(*soundBank.activeSlots.at(3), slider->getScaledValue());
	}
	else if(name == "PRS1")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		setPreset(0);
	}
	else if(name == "PRS2")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		setPreset(1);
	}
	else if(name == "PRS3")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		setPreset(2);
	}
	else if(name == "PRS4")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		setPreset(3);
	}
	else if(name == "PRS5")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		setPreset(4);
	}
	else if(name == "PRS6")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		setPreset(5);
	}
	else if(name == "PRS7")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		setPreset(6);
	}
	else if(name == "PRS8")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		setPreset(7);
	}
}

//--------------------------------------------------------------
void testApp::exit()
{
    //gui->saveSettings("GUI/guiSettings.xml");
    delete gui;

	midiIn.closePort();

    for(int i=0; i<soundBank.activeSlots.size(); i++){
        midiIn.removeListener(soundBank.activeSlots.at(i));
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
    soundBank.setMIDIMapping(15,88);
    soundBank.setMIDIMapping(15,89);
    soundBank.setMIDIMapping(15,90);
    soundBank.setMIDIMapping(15,91);
    soundBank.setMIDIMapping(15,92);
    soundBank.setMIDIMapping(15,93);
    soundBank.setMIDIMapping(15,94);
    soundBank.setMIDIMapping(15,95);

    ofLogNotice() << "Done setupAVUgens" << endl;

}

//--------------------------------------------------------------
void testApp::setupMIDI(){
    // ofxMidi

	midiIn.listPorts(); // via instance

    // midiIn.openPort("Audio 8 DJ MIDI input port 0");
	midiIn.openPort("USB Uno MIDI Interface");

	//midiIn.openVirtualPort("ofxMidiIn Input");	// open a virtual port

	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);

    // add testApp as a listener
    // midiIn.addListener(this);
    // add each avUgen as a midi listener
    for(int i=0; i < soundBank.activeSlots.size(); i++){
        midiIn.addListener(soundBank.activeSlots.at(i));
    }

    // add the testApp as a listener to allow preset selection on the soundbank
    midiIn.addListener(this);

	// print received messages to the console
	midiIn.setVerbose(true);

    ofLogNotice() << "Done setupMIDI" << endl;
}

//--------------------------------------------------------------
void testApp::setupUI(){
    // OF UI
    float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 370-xInit;
    float presetButtonSize= 30;
    float padding = 22;

    gui = new ofxUICanvas(0,0,length+xInit*2.0,ofGetHeight() + 50);
    gui -> setTheme(OFX_UI_THEME_GRAYDAY);
	gui->addWidgetDown(new ofxUILabel("SPATIAL - AVUGEN", OFX_UI_FONT_LARGE));

    gui->addSpacer(length, 2);


    ofColor paddingColor = ofColor(255,0,0,200);

    rad0 = (ofxUIRotarySlider *) gui->addWidgetDown(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(0)->getRadius(), "RAD0"));
    rad0 -> setPadding(padding);
    rad1 = (ofxUIRotarySlider *) gui->addWidgetRight(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(1)->getRadius(), "RAD1"));
    rad1 -> setPadding(padding);
    rad2 = (ofxUIRotarySlider *) gui->addWidgetRight(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(2)->getRadius(), "RAD2"));
    rad2 -> setPadding(padding);
    rad3 = (ofxUIRotarySlider *) gui->addWidgetRight(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(3)->getRadius(), "RAD3"));
    rad3 -> setPadding(padding);

    hue0 = (ofxUIRotarySlider *) gui->addWidgetDown(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(0)->getColor().getHue(), "HUE0"));
    hue0 -> setPadding(padding);
    hue1 = (ofxUIRotarySlider *) gui->addWidgetRight(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(1)->getColor().getHue(), "HUE1"));
    hue1 -> setPadding(padding);
    hue2 = (ofxUIRotarySlider *) gui->addWidgetRight(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(2)->getColor().getHue(), "HUE2"));
    hue2 -> setPadding(padding);
    hue3 = (ofxUIRotarySlider *) gui->addWidgetRight(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(3)->getColor().getHue(), "HUE3"));
    hue3 -> setPadding(padding);

    thr0 = (ofxUIRotarySlider *) gui->addWidgetDown(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(0)->getThrottle(), "THR0"));
    thr0 -> setPadding(padding);
    thr1 = (ofxUIRotarySlider *) gui->addWidgetRight(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(1)->getThrottle(), "THR1"));
    thr1 -> setPadding(padding);
    thr2 = (ofxUIRotarySlider *) gui->addWidgetRight(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(2)->getThrottle(), "THR2"));
    thr2 -> setPadding(padding);
    thr3 = (ofxUIRotarySlider *) gui->addWidgetRight(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(3)->getThrottle(), "THR3"));
    thr3 -> setPadding(padding);

    pos0 = (ofxUIRotarySlider *) gui->addWidgetDown(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(0)->getPan() * 127, "POS0"));
    pos0 -> setPadding(padding);
    pos1 = (ofxUIRotarySlider *) gui->addWidgetRight(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(1)->getPan() * 127, "POS1"));
    pos1 -> setPadding(padding);
    pos2 = (ofxUIRotarySlider *) gui->addWidgetRight(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(2)->getPan() * 127, "POS2"));
    pos2 -> setPadding(padding);
    pos3 = (ofxUIRotarySlider *) gui->addWidgetRight(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(3)->getPan() * 127, "POS3"));
    pos3 -> setPadding(padding);

    ypos0 = (ofxUIRotarySlider *) gui->addWidgetDown(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(0)->getY(), "YPOS0"));
    ypos0 -> setPadding(padding);
    ypos1 = (ofxUIRotarySlider *) gui->addWidgetRight(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(1)->getY(), "YPOS1"));
    ypos1 -> setPadding(padding);
    ypos2 = (ofxUIRotarySlider *) gui->addWidgetRight(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(2)->getY(), "YPOS2"));
    ypos2 -> setPadding(padding);
    ypos3 = (ofxUIRotarySlider *) gui->addWidgetRight(new ofxUIRotarySlider(dim*4, 0, 127, soundBank.activeSlots.at(3)->getY(), "YPOS3"));
    ypos3 -> setPadding(padding);

    gui->addSpacer(length, 4);

    gui->addWidgetDown(new ofxUILabel("SWITCH PRESETS (KEYS 1-8)", OFX_UI_FONT_MEDIUM));

    padding = padding + 30;

    prs1 = (ofxUIButton *) gui->addWidgetDown(new ofxUIButton("PRS1", false, presetButtonSize, presetButtonSize));
    prs1 -> setPadding(padding);
    presetButtons.push_back(prs1);
    prs2 = (ofxUIButton *) gui->addWidgetRight(new ofxUIButton("PRS2", false, presetButtonSize, presetButtonSize));
    prs2 -> setPadding(padding);
    presetButtons.push_back(prs2);
    prs3 = (ofxUIButton *) gui->addWidgetRight(new ofxUIButton("PRS3", false, presetButtonSize, presetButtonSize));
    prs3 -> setPadding(padding);
    presetButtons.push_back(prs3);
    prs4 = (ofxUIButton *) gui->addWidgetRight(new ofxUIButton("PRS4", false, presetButtonSize, presetButtonSize));
    presetButtons.push_back(prs4);
    prs5 = (ofxUIButton *) gui->addWidgetDown(new ofxUIButton("PRS5", false, presetButtonSize, presetButtonSize));
    prs5 -> setPadding(padding);
    presetButtons.push_back(prs5);
    prs6 = (ofxUIButton *) gui->addWidgetRight(new ofxUIButton("PRS6", false, presetButtonSize, presetButtonSize));
    prs6 -> setPadding(padding);
    presetButtons.push_back(prs6);
    prs7 = (ofxUIButton *) gui->addWidgetRight(new ofxUIButton("PRS7", false, presetButtonSize, presetButtonSize));
    prs7 -> setPadding(padding);
    presetButtons.push_back(prs7);
    prs8 = (ofxUIButton *) gui->addWidgetRight(new ofxUIButton("PRS8", false, presetButtonSize, presetButtonSize));
    presetButtons.push_back(prs8);

    gui->addSpacer(length, 4);

    gui->addWidgetDown(new ofxUILabel("HIDE & SHOW GUI: PRESS 'g'", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUILabel("SAVE PRESETS: PRESS 's'", OFX_UI_FONT_MEDIUM));

    gui->addSpacer(length, 4);
    gui->addWidgetDown(new ofxUILabel("MOUSE OVER A SLIDER AND", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUILabel("PRESS UP, DOWN, LEFT, RIGHT", OFX_UI_FONT_MEDIUM));

    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);

    // gui->loadSettings("GUI/guiSettings.xml");

    if (!isRunningStandalone()) {
        gui->toggleVisible();
    }

    ofLogNotice() << "Done setupUI" << endl;

}

//--------------------------------------------------------------
void testApp::setupSound() {
    // Maximillian
    sampleRate 			= 44100;
    initialBufferSize	= 512;

    soundStream.listDevices();
    // soundStream.setDeviceID(3); //(ma++ ingalls for Cycling '74: Soundflower (2ch))
    soundStream.setup(this, NUM_SOUNDCARD_CHANNELS, 0, sampleRate, initialBufferSize, 4); /* Call this last ! */
    ofLogNotice() << "Done setupSound" << endl;
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
            ofLogNotice() << "About to save presets" << endl;
			soundBank.savePresetsToXML();
			break;
		case 'p':
            cyclePreset();
			break;
        case '1':
            setPreset(0);
            break;
        case '2':
            setPreset(1);
            break;
        case '3':
            setPreset(2);
            break;
        case '4':
            setPreset(3);
            break;
        case '5':
            setPreset(4);
            break;
        case '6':
            setPreset(5);
            break;
        case '7':
            setPreset(6);
            break;
        case '8':
            setPreset(7);
            break;
        default:
            break;

    }
}

void testApp::newMidiMessage(ofxMidiMessage& msg) {
    if (isFireMIDI(msg)) {
        midiMessage = msg;
        ofLogVerbose() << "TEST APP midi ch: " << msg.channel << " control: " << msg.control << endl;
        ofLogVerbose() << "value: " << msg.value << endl;

        setPreset(soundBank.currentPreset);
    }
}

bool testApp::isFireMIDI(ofxMidiMessage& msg){
    bool ret = false;

    ofLogVerbose() << "TEST APP midi ch: " << msg.channel << " control: " << msg.control << endl;
    ofLogVerbose() << "value: " << msg.value << endl;

    for(int i=0; i<soundBank.midiChannel.size(); i++){
        if (soundBank.midiChannel.at(i) == msg.channel){
            for(int j=0; j<soundBank.midiControlNumber.size(); j++){
                if (soundBank.midiControlNumber.at(j) == msg.control) {
                    soundBank.currentPreset = j;
                    ret = true;
                }
            }
        }
    }

    return ret;
}

void testApp::cyclePreset() {
    for(int i = 0; i<soundBank.activeSlots.size(); i++) {
        midiIn.removeListener(soundBank.activeSlots.at(i));
    }

    soundBank.cyclePreset();

    for(int i = 0; i<soundBank.activeSlots.size(); i++) {
        midiIn.addListener(soundBank.activeSlots.at(i));
    }
}

void testApp::setPreset(int _preset) {
    for(int i = 0; i<soundBank.activeSlots.size(); i++) {
        midiIn.removeListener(soundBank.activeSlots.at(i));
    }

    soundBank.setPreset(_preset);

    for(int i = 0; i<soundBank.activeSlots.size(); i++) {
        midiIn.addListener(soundBank.activeSlots.at(i));
    }
}

bool testApp::isRunningStandalone(){
    string sTrue = "true";
    char const* tmp = getenv("STANDALONE");
    bool retVal = false;

    if(tmp != NULL) {
        string standalone (tmp);

        if (sTrue.compare(standalone) == 0) {
            retVal = true;
        }
    }

    return retVal;
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