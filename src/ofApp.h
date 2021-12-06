#pragma once



#include "ofMain.h"
#include "ofxOsc.h"


#define PORT 12345


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

std::vector<ofFbo> FrameBuffers;


 int FboWidth;
 int FboHeight;
const int RowHeight=20;
const int TargetScreenHeight=720;

const string OutPutPath= "./output/";
int FBCount;


int ActiveFBO;

const int ScrollSpeed=2;
 unsigned int scrollDelta;


 ofxOscReceiver receiver;

 ofImage BackgroundImage;
bool KeyPressed;
};
