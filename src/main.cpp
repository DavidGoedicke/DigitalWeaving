#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

//	ofSetupOpenGL(1080,1920, OF_FULLSCREEN);			// <-------- setup the GL context
	ofSetupOpenGL(720,1280, OF_FULLSCREEN);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
