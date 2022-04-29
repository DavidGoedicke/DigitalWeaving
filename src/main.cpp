#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

//	ofSetupOpenGL(1080,1920, OF_FULLSCREEN);			// <-------- setup the GL context
	ofSetupOpenGL(2880/2,1800, OF_WINDOW);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
