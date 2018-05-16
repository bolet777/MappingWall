#include "ofMain.h"
#include "ofApp.h"

int main(){
	ofSetLogLevel(OF_LOG_SILENT);
	ofSetupOpenGL(1024,768,OF_WINDOW);
	ofRunApp(new ofApp());
}
