#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
FboWidth=ofGetWidth();
FboHeight = (RowHeight*20);

 FBCount = ((int)ceilf(FboHeight/720))+1;

cout<<"I am gonna need this many framebuffers"<<FBCount<<endl;
for(int i =0; i<FBCount;i++){
  ofFbo temp;
  temp.allocate(FboWidth,FboHeight, GL_RGBA);
  temp.begin();
  ofClear(255,0,255,70);
  temp.end();
FrameBuffers.push_back(temp);

}

///

scrollDistance=0;
scrollDelta=0;
}

//--------------------------------------------------------------
void ofApp::update(){

}


void ofApp::MakeNextSection(int param[8]){

  int yPos= 0;
int RowDif = RowHeight;
NextActive->begin();
ofClear(255,255,255);
  drawNextRow(yPos,false, ofColor(0, 0,0));
  drawNextRow(yPos+=RowDif,true, ofColor(0, 0,0));
  drawNextRow(yPos+=RowDif,false, ofColor(250, 250,250));
//cout<<"Step A"<<endl;
  for(int i=0;i<8;i++){
    drawNextRow(yPos+=RowDif,true, ofColor(200, 0,0));
    drawNextRow(yPos+=RowDif,false, ofColor(250, 250,250));
//    cout<<"in the loop"<<i<<endl;
  }
//cout<<"Step B"<<endl;
drawNextRow(yPos+=RowDif,true, ofColor(0, 0,0));
  drawNextRow(yPos+=RowDif,false, ofColor(0, 0,0));
  cout<<yPos<<FboHeight<<endl;
NextActive->end();



if(NextActive==&fboA){
  NextActive=&fboB;
  cout<<"Switching to B"<<endl;
}else if(NextActive==&fboB){
  NextActive=&fboA;
  cout<<"Switching to A"<<endl;
}


scrollDelta+=FboHeight;

}



void ofApp::drawNextRow(int yPos,bool indentedLine,ofColor col){
  int columncount = 8;
  int columnWidth = FboWidth/(columncount+0.5);
  for( int i =0;i<columncount;i++){
      int xPos=((i+1)*columnWidth)-columnWidth/2;
      if(indentedLine){
        xPos+=columnWidth/2;
      }
    //  ofSetColor(col, 100);
    //  ofDrawEllipse(xPos,yPos, columnWidth,RowHeight);

      ofSetColor(col, 110);
      ofDrawEllipse(xPos,yPos, columnWidth,RowHeight);
      ofSetColor(col, 210);
      ofDrawEllipse(xPos,yPos, columnWidth*0.8 ,RowHeight*0.8);

}


}


//--------------------------------------------------------------
void ofApp::draw(){
//preparation

  if(scrollDelta>0){
    scrollDistance+=ScrollSpeed;
    scrollDelta-=ScrollSpeed;
  }

int height=ofGetHeight();


///Rendering
ofBackground(80);
ofSetColor(255,255,255);


for(int i =0; i<FBCount;i++){

FrameBuffers[i].draw(0,height-(scrollDistance+(i*FboHeight))%height);

}




//fboA.draw(0,height-(scrollDistance)%height);
//fboB.draw(0,height-(scrollDistance+FboHeight)%height);


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
  int param[8] = {1000, 2, 3, 17, 50};
  MakeNextSection(param);
  cout<<"GotOneKeyPress"<<endl;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
