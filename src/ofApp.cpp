#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


ActiveMarkersfound=0;



FboWidth=ofGetWidth();
FboHeight = (RowHeight*20);

 FBCount =  (int)  ((ceil((float)TargetScreenHeight/(float)FboHeight))+3);
ActiveFBO=0;
cout<<"I am gonna need this many framebuffers: "<<FBCount<<endl;
for(int i =0; i<FBCount;i++){
  ofFbo temp;
  temp.allocate(FboWidth*SuperSample,FboHeight*SuperSample, GL_RGBA);
  temp.begin();
  ofClear(0,0,0,0);
////  ofSetColor(255,i*75,0);
//  ofDrawRectangle(1,1,FboWidth-1,FboHeight-1);
//  ofDrawBitmapString(i,FboWidth/2,FboHeight/2);
  temp.end();
  FrameBuffers.push_back(temp);


BackgroundImage.load("Background.png");
 OverlayImage.load("Overlay.png");

 ofSetVerticalSync(true);
ofSetFrameRate(35);

  }
//
// fboA.allocate(100,100, GL_RGB);
// fboA.begin();
// ofClear(255,255,255);
// ofNoFill();
// ofSetColor(255,255,0);
// ofDrawRectangle(1,1,99,99);
// fboA.end();

///720.0

scrollDelta=0;

receiver.setup(PORT);
}

//--------------------------------------------100------------------
void ofApp::update(){

  while(receiver.hasWaitingMessages()){
    ofxOscMessage m;
    receiver.getNextMessage(m);

    		if(m.getAddress() == "/pattern"){

          int param[8] ;//{ofRandom(100), 2, 3, 17, 50};
          for(int i=0;i<8;i++){
            param[i]=  m.getArgAsInt32(i);
          }
          MakeNextSection(param);

        }else if(m.getAddress() == "/count"){

          ActiveMarkersfound = m.getArgAsInt32(0);
        }
      }

}


void ofApp::MakeNextSection(int param[8]){

if(scrollDelta>FboHeight){
  ofLog()<<"Discarding Input too much in the queue"<<endl;
  return;
}
ActiveFBO--;
if(ActiveFBO<0){
  ActiveFBO=FBCount-1;
}
scrollDelta+=FboHeight;
int yPos= 0;
int RowDif = RowHeight;

ofEnableAntiAliasing();

FrameBuffers[ActiveFBO].begin();

ofClear(0,0,0,0);
  drawNextRow(yPos,false, black);
  drawNextRow(yPos+=RowDif,true, black);
  drawNextRow(yPos+=RowDif,false, black);//white);
//cout<<"Step A"<<endl;
  for(int i=0;i<8;i++){
ofColor col;
switch(param[i]){

  case 0:
  col=red;
  break;
  case 1:
    col=green;
  break;
  case 2:
    col=blue;
  break;
  case 3:
  col=yellow ;
  break;
  default:
    col=white;
  break;

}

    drawNextRow(yPos+=RowDif,true, col);
    drawNextRow(yPos+=RowDif,false, col);   //white;
//    cout<<"in the loop"<<i<<endl;
  }
//cout<<"Step B"<<endl;;
drawNextRow(yPos+=RowDif,true,black);
  drawNextRow(yPos+=RowDif,false, black);

FrameBuffers[ActiveFBO].end();

if(!  KeyPressed){
    ofPixels pixels;
    FrameBuffers[ActiveFBO].readToPixels(pixels);
	 ofSaveImage(pixels,OutPutPath+ofGetTimestampString()+".png");
  }
  else{
    KeyPressed=false;
  }

}



void ofApp::drawNextRow(int yPos,bool indentedLine,ofColor col){
  int columncount = 8;

  int columnWidth = FboWidth/(columncount+0.5);
  if(!indentedLine){columncount++;}
  for( int i =0;i<columncount;i++){
      int xPos=((i+1)*columnWidth)-columnWidth/2;
      if(indentedLine){
        xPos+=columnWidth/2;
          //  cout<< i <<"  "<<xPos<<endl;
      }
      xPos-=columnWidth/4;
    //  ofSetColor(col, 100);columnWidth
    //  ofDrawEllipse(xPos,yPos, columnWidth,RowHeight);

  //  ofSetColor(col, 89);
  //  ofDrawEllipse(xPos*SuperSample,yPos*SuperSample, columnWidth*1.1,RowHeight*1.1);
    ofSetColor(col, 255);
  //  if(!indentedLine){
      //ofDrawEllipse(xPos*SuperSample,yPos*SuperSample,  columnWidth*SuperSample,RowHeight*SuperSample);
    //  }else{
    ofDrawEllipse(xPos*SuperSample,yPos*SuperSample,  0.9*columnWidth*SuperSample,0.9*RowHeight*SuperSample);
    const float scaller=0.75;
        ofDrawRectRounded((xPos*SuperSample)-(scaller*columnWidth*SuperSample)/2,(yPos*SuperSample)-(scaller*RowHeight*SuperSample)/2,  scaller*columnWidth*SuperSample,scaller*RowHeight*SuperSample,5*SuperSample);
  //    }
  //    ofSetColor(col, 255);
//      ofDrawEllipse(xPos*SuperSample,yPos*SuperSample,  columnWidth*0.8 ,RowHeight*0.8);

}


}


//-------------------FboHeight-------------------------------------------
void ofApp::draw(){
//preparation
ofEnableAlphaBlending();
  if(scrollDelta>0){
    if(ofGetFrameNum()%(RowHeight*2)>=RowHeight && scrollDelta<=FboHeight){

      scrollDelta-=ScrollSpeed;
    }else if(scrollDelta>FboHeight){

        scrollDelta-=ScrollSpeed;
    }
  }
  if(scrollDelta<0){
    scrollDelta=0;
  }

int height=ofGetHeight();


///Rendering
ofBackground(127);
BackgroundImage.draw(0,0);
ofSetColor(255,255,255);

int hVar = height+scrollDelta;
int itter = ActiveFBO-1;

for(int i =0; i<FBCount;i++){

if(itter<0){
  itter=FBCount-1;
}else if(itter>=FBCount){
  itter=0;
}

//cout<<itter<<hVar<<endl;
FrameBuffers[itter].draw(0,hVar-75,FboWidth,FboHeight);

//FrameBuffers[i].draw(0,0);,(
//FrameBuffers[ActiveActiveScActiveFBOActiveFBOreenActiveScActiveFBOActiveFBOreenScreenitter].draw(0,(scrollDistance+(i*FboHeight))%(TargetScreenHeight+FboHeight));

hVar-=FboHeight;
itter++;

}
OverlayImage.draw(0,0);
//height-(scrollDistance+(i*FboHeight))%height

//fboA.draw(1280/2,720/2);

//fboA.draw(0,height-(scrollDistance)%height);
//fboB.draw(0,height-(scrollDistance+FboHeight)%height);

ofDrawBitmapString(ActiveMarkersfound,10,10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
  int param[8] ;//{ofRandom(100), 2, 3, 17, 50};
  for(int i=0;i<8;i++){
    param[i]=((int)ofRandom(100))%4;
  }
    KeyPressed=true;
  MakeNextSection(param);

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
