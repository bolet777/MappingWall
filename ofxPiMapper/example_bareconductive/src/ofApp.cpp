#include "ofApp.h"
#include "ofxXmlSettings.h"

void ofApp::setup(){
	ofBackground(0);
	ofSetLogLevel(OF_LOG_SILENT);

	// Enable or disable audio for video sources globally
	// Set this to false to save resources on the Raspberry Pi
	ofx::piMapper::VideoSource::enableAudio = true;
	ofx::piMapper::VideoSource::useHDMIForAudio = false;

	// Register our sources.
	// This should be done before mapper.setup().
	//mapper.registerFboSource(photoSource);
	//piMapper.registerFboSource(nameSource);
	mapper.setup();

    #ifdef TARGET_RASPBERRY_PI
        ofSetFullscreen(true);
    #endif
}

void ofApp::update(){
	mapper.update();
}

void ofApp::draw(){
	mapper.draw();
}

void ofApp::keyPressed(int key){
	ofKeyEventArgs args;
	args.key = key;

	switch (args.key) {
		case 'w':
			mapper.loadProject("data/jeff_goodboi.xml");
			break;

		case 'e':
			mapper.loadProject("data/annie_gabriel.xml");
			break;

		case 'r':
			mapper.loadProject("data/karine_cournoyer.xml");
			break;
	}

	mapper.keyPressed(key);
}

void ofApp::keyReleased(int key){
	mapper.keyReleased(key);
}

void ofApp::mouseDragged(int x, int y, int button){
	mapper.mouseDragged(x, y, button);
}

void ofApp::mousePressed(int x, int y, int button){
	mapper.mousePressed(x, y, button);
}

void ofApp::mouseReleased(int x, int y, int button){
	mapper.mouseReleased(x, y, button);
}
