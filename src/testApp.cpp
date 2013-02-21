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
    for (int i=0; i<channels.size(); i++) {
        rgbHsb.push_back(new ofColor(red, green, blue));
    }

    setupUI();
    setupMIDI();
    setupSound(); /* Call this last ! */

    if (debug) cout << "MSP End OF Setup" << endl;
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

	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    for (int i=0; i<channels.size(); i++) {
        ofPushStyle();
        if (i == 0) channels.at(i) -> setColor(*rgbHsb.at(i));

        if (debug) cout << "MSP wave[" << i << "]:" << wave[i] << endl;

        if (wave[i] > 0) {
            if (i == 0) {
//                channels.at(i) -> setRadius(channels.at(i)->getRadius() + radius_multiplier * wave[i]);
            } else {
                channels.at(i) -> setRadius(channels.at(i)->getRadius() * wave[i]);
            }
        } else {
//            if (i == 0) channels.at(i) -> setRadius(radius_multiplier);
        }

        channels.at(i) -> draw();
        
        ofPopStyle();
    }

    ofPopStyle();

//    drawMIDI();    
}

//--------------------------------------------------------------
void testApp::audioRequested 	(float * output, int bufferSize, int nChannels){
	
	for (int i = 0; i < bufferSize; i++){

//        TODO refactor this mess!
        if (ch1->isAudioOn() == true){
            wave[0] = ch1->getAudio();
        }

        if (ch2->isAudioOn()){
            wave[1] = ch2->getAudio();
        }

        if (ch3->isAudioOn()){
            wave[2] = ch3->getAudio();
        }

        if (ch4->isAudioOn()){
            wave[3] = ch4->getAudio();
        }

        float pan = 0.5;

        if (ch1-> isAudioOn() && ch2->isAudioOn() && ch3->isAudioOn() && ch4->isAudioOn()){
            mix.quad((wave[0] + wave[1] + wave[2] + wave[3]) / 2, outputs, pan, pan);
        } else if (ch1-> isAudioOn() && ch2->isAudioOn() && ch3->isAudioOn()){
            mix.stereo(wave[0] + wave[1] + wave[2], outputs, pan);
        } else if (ch1-> isAudioOn() && ch2->isAudioOn()){
            mix.stereo(wave[0] + wave[1], outputs, pan);
        } else {
            mix.stereo(wave[solo], outputs, pan);
        }        		

		output[i*nChannels    ] = outputs[0]; /* You may end up with lots of outputs. add them here */
		output[i*nChannels + 1] = outputs[1];

        if (NUM_CHANNELS == 4) {
            output[i*nChannels + 2] = outputs[2];
            output[i*nChannels + 3] = outputs[3];
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
	else if(name == "RADIUS")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		radius_multiplier = slider->getScaledValue();
	}
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
		ch1 -> setX(pad->getPercentValue().x*ofGetWidth());
		ch1 -> setY(pad->getPercentValue().y*ofGetHeight());
	}
    else if(name == "POSITION_CH2")
	{
		ofxUI2DPad *pad = (ofxUI2DPad *) e.widget;
		ch2 -> setX(pad->getPercentValue().x*ofGetWidth());
		ch2 -> setY(pad->getPercentValue().y*ofGetHeight());
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

    for(int i=0; i<channels.size(); i++){
        midiIn.removeListener(channels.at(i));
    }
}

//--------------------------------------------------------------
void testApp::setupAVUgens(){
    ch1 = new msp::avUgen();

    ch1->setX(width/2);
    ch1->setY(height/2);
    ch1->setRadius(100);
    ch1->setSpeed(10);
    ch1->setRandomResolution();
    ch1->setColor(*new ofColor(233, 52, 70, msp::avUgen::LIGHT_ALPHA));

    ch1->setAudioEngine(msp::avUgen::MONO);

    ch1->midiChannel = 14;
    ch1->midiControlNumber = 100;

    channels.push_back(ch1);


    ch2 = new msp::avUgen();

    ch2->setX(width/2 - 100);
    ch2->setY(height/2 - 100);
    ch2->setRadius(100);
    ch2->setSpeed(30);
//    ch2->setColor(*new ofColor(0, 0, 0, msp::avUgen::HEAVY_ALPHA));


    ch2->setFrequency(101);

    ch2->midiChannel = 14;
    ch2->midiControlNumber = 101;

    channels.push_back(ch2);

    ch3 = new msp::avUgen();
    ch3->setAudioEngine(msp::avUgen::MONO);
    ch3->setFrequency(102);
    channels.push_back(ch3);

    ch4 = new msp::avUgen();
    ch4->setFrequency(103);
    ch4->setRandomResolution();
    channels.push_back(ch4);

    // a/v state
//    ch1->switchOffAudio();
//    ch1->switchOffVisual();
//    ch2->switchOffAudio();
//    ch2->switchOffVisual();
    ch3->switchOffAudio();
    ch3->switchOffVisual();
    ch4->switchOffAudio();
    ch4->switchOffVisual();
    solo = 4;
    solo = solo - 1;

    
}

//--------------------------------------------------------------
void testApp::setupMIDI(){
    // ofxMidi
	// print input ports to console
	midiIn.listPorts(); // via instance
	//ofxMidiIn::listPorts(); // via static as well

	// open port by number (you may need to change this)
	// midiIn.openPort(0);
	midiIn.openPort("USB Uno MIDI Interface");	// by name
	//midiIn.openVirtualPort("ofxMidiIn Input");	// open a virtual port

	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);

    // add testApp as a listener
    //	midiIn.addListener(this);
    // add each avUgen as a midi listener
    for(int i=0; i < channels.size(); i++){
        midiIn.addListener(channels.at(i));
    }

	// print received messages to the console
	midiIn.setVerbose(true);
}

//--------------------------------------------------------------
void testApp::setupUI(){
    // OF UI
    float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 320-xInit;
    radius_multiplier = 50;

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
    gui->addSlider("RED", 0.0, 255.0, rgbHsb.at(0)->r, length,dim);
	gui->addSlider("GREEN", 0.0, 255.0, rgbHsb.at(0)->g, length,dim);
    gui->addSlider("BLUE", 0.0, 255.0, rgbHsb.at(0)->b, length,dim);
    gui->addSlider("ALPHA", 0.0, 255.0, rgbHsb.at(0)->a, length,dim);
    gui->addSlider("RADIUS", 0.0, 600.0, radius_multiplier, length,dim);
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

    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);

    gui->loadSettings("GUI/guiSettings.xml");
    gui->toggleVisible();

}

//--------------------------------------------------------------
void testApp::setupSound() {
    // Maximillian
    sampleRate 			= 44100;
    initialBufferSize	= 512;

    soundStream.listDevices();
    // soundStream.setDeviceID(5);
    soundStream.setup(this, NUM_CHANNELS, 0, sampleRate, initialBufferSize, 4); /* Call this last ! */
}

void testApp::drawMIDI() {

    for(int i=0; i<channels.size(); i++){
        // draw the last recieved message contents to the screen
        text << "Received: " << ofxMidiMessage::getStatusString(channels.at(i) -> midiMessage.status);
        ofDrawBitmapString(text.str(), 20, 20);
        text.str(""); // clear

        text << "channel: " << channels.at(i) -> midiMessage.channel;
        ofDrawBitmapString(text.str(), 20, 34);
        text.str(""); // clear

        text << "pitch: " << channels.at(i) -> midiMessage.pitch;
        ofDrawBitmapString(text.str(), 20, 48);
        text.str(""); // clear
        ofRect(20, 58, ofMap(channels.at(i) -> midiMessage.pitch, 0, 127, 0, ofGetWidth()-40), 20);

        text << "velocity: " << channels.at(i) -> midiMessage.velocity;
        ofDrawBitmapString(text.str(), 20, 96);
        text.str(""); // clear
        ofRect(20, 105, ofMap(channels.at(i) -> midiMessage.velocity, 0, 127, 0, ofGetWidth()-40), 20);

        text << "control: " << channels.at(i) -> midiMessage.control;
        ofDrawBitmapString(text.str(), 20, 144);
        text.str(""); // clear
        ofRect(20, 154, ofMap(channels.at(i) -> midiMessage.control, 0, 127, 0, ofGetWidth()-40), 20);

        text << "value: " << channels.at(i) -> midiMessage.value;
        ofDrawBitmapString(text.str(), 20, 192);
        text.str(""); // clear
        if(channels.at(i) -> midiMessage.status == MIDI_PITCH_BEND) {
            ofRect(20, 202, ofMap(channels.at(i) -> midiMessage.value, 0, MIDI_MAX_BEND, 0, ofGetWidth()-40), 20);
        }
        else {
            ofRect(20, 202, ofMap(channels.at(i) -> midiMessage.value, 0, 127, 0, ofGetWidth()-40), 20);
        }
        
        text << "delta: " << channels.at(i) -> midiMessage.deltatime;
        ofDrawBitmapString(text.str(), 20, 240);
        text.str(""); // clear
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key)
    {
        case 'g':
        {
            gui->toggleVisible();
        }
            break;
		case 'l':
			midiIn.listPorts();
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