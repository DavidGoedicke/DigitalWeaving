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
    void exit();

		void MakeNextSection(int param[8]);
	void	drawNextRow(int yPos,bool indentedLine,ofColor col);

std::vector<ofFbo> FrameBuffers;

const int SuperSample=2;
 int FboWidth;
 int FboHeight;
const int RowHeight=20;
const int TargetScreenHeight=1280;

const string OutPutPath= "./output/";
int FBCount;

int ActiveMarkersfound;


int ActiveFBO;

const int ScrollSpeed=65*4;
 unsigned int scrollDelta;


 ofxOscReceiver receiver;

 ofImage BackgroundImage;
 ofImage OverlayImage;
bool KeyPressed;
    
    
    ofShader shaderBlurX;
    ofShader shaderBlurY;
    bool doShader;
    const ofColor a = ofColor(101,88,80);
    const ofColor b = ofColor(235,205,165);
    const ofColor c = ofColor(62,71,34);
    const ofColor d = ofColor(219-40,241-40,236-40);
    
   // const ofColor green = ofColor(214,169,148);
    //const ofColor blue = ofColor(250,191,133);
    //const ofColor yellow = ofColor(214,86,97);
    const ofColor white = ofColor(200,200,200);
   // const ofColor black = ofColor(50,50,50);
    
    
    
    
    
//const ofColor red = ofColor(235,127,108);
//const ofColor green = ofColor(214,169,148);
//const ofColor blue = ofColor(250,191,133);
//const ofColor yellow = ofColor(214,86,97);
//const ofColor white = ofColor(200,200,200);
//const ofColor black = ofColor(50,50,50);


//const ofColor red = ofColor(75,206,224);
//const ofColor green = ofColor(69,197,247);
//const ofColor blue = ofColor(77,166,240);
//const ofColor yellow = ofColor(66,237,197);
//const ofColor white = ofColor(200,200,200);
//const ofColor black = ofColor(50,50,50);
};
