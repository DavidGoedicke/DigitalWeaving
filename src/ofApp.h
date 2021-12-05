#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void MakeNextSection(int param[8]);
	void	drawNextRow(int yPos,bool indentedLine,ofColor col);

ofFbo* NextActive;

std::vector<ofFbo> FrameBuffers;

ofFbo fboA;
ofFbo fboB;
 int FboWidth;
 int FboHeight;
const int RowHeight=20;


int FBCount;


const int ScrollSpeed=2;
 unsigned long scrollDistance;
 unsigned int scrollDelta;

};
