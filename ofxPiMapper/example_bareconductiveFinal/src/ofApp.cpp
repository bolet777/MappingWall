#include "ofApp.h"

void ofApp::setup(){
	ofBackground(0);
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

void ofApp::keyPressed(int key) {

	switch (key) {
		case 'q':
			mapper.setPreset(0);
			break;

		case 'w':
			mapper.setPreset(1);
			break;

		case 'e':
			mapper.setPreset(2);
			break;

		case 'r':
			mapper.setPreset(3);
			break;

		case 't':
			mapper.setPreset(4);
			break;

		case 'y':
			mapper.setPreset(5);
			break;

		case 'u':
			mapper.setPreset(6);
			break;

		case 'i':
			mapper.setPreset(7);
			break;

		case 'o':
			mapper.setPreset(8);
			break;

		case 'p':
			mapper.setPreset(9);
			break;

		case 'a':
			mapper.setPreset(10);
			break;

		case 's':
			mapper.setPreset(11);
			break;

		default:
			mapper.keyPressed(key);
			break;
	}
	usleep(10);
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
