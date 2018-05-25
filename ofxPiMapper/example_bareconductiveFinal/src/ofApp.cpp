#include "ofApp.h"

void ofApp::setup(){
	preset = 0;
	ofBackground(0);
	mapper.setup();

    #ifdef TARGET_RASPBERRY_PI
        ofSetFullscreen(true);
    #endif
}

void ofApp::update() {
	std::ifstream input;
	input.open("/home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductiveFinal/bin/command.txt");
	int id;
	input >> id;
	//std::cout << id << endl;
	if (id != preset) {
		changePreset(id);
	}
	input.close();
	mapper.update();
}

void ofApp::draw(){
	mapper.draw();
}

void ofApp::keyPressed(int key) {
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

void ofApp::changePreset(int id) {
	if (id >= 0 && id <= 11) {
		preset = id;
		mapper.setPreset(id);
	}
}
