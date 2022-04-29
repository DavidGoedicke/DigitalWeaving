#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    ActiveMarkersfound=0;
    
    BackgroundImage.load("Background.png");
    OverlayImage.load("Overlay.png");
    
    
    FboWidth=BackgroundImage.getWidth();
    FboHeight = (RowHeight*18); //(RowHeight*20);
    
    FBCount =  (int)  ((ceil((float)TargetScreenHeight/(float)FboHeight))+3);
    ActiveFBO=0;
    cout<<"I am gonna need this many framebuffers: "<<FBCount<<endl;
    for(int i =0; i<FBCount;i++){
        ofFbo temp;
        temp.allocate(FboWidth*SuperSample,FboHeight*SuperSample, GL_RGBA32F);
        temp.begin();
        ofClear(0,0,0,0);
        ////  ofSetColor(255,i*75,0);
        //  ofDrawRectangle(1,1,FboWidth-1,FboHeight-1);
        //  ofDrawBitmapString(i,FboWidth/2,FboHeight/2);
        temp.end();
        FrameBuffers.push_back(temp);
        
        
        
        
        ofSetVerticalSync(true);
        ofSetFrameRate(35);
        
    }
    
    
    scrollDelta=0;
    
    
    
#ifdef TARGET_OPENGLES
    shaderBlurX.load("shadersES2/shaderBlurX");
    shaderBlurY.load("shadersES2/shaderBlurY");
#else
    if(ofIsGLProgrammableRenderer()){
        shaderBlurX.load("shadersGL3/shaderBlurX");
        shaderBlurY.load("shadersGL3/shaderBlurY");
    }else{
        shaderBlurX.load("shadersGL2/shaderBlurX");
        shaderBlurY.load("shadersGL2/shaderBlurY");
    }
#endif
    
    doShader = true;
    
    
    receiver.setup(PORT);
    string s ="cd " +ofFilePath::getCurrentExeDir()+ "../../../../python && ./run.sh";
    //
    cout<<"about to call"<<s<<endl;
    system(s.c_str());
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
    drawNextRow(yPos,false, white);
   // drawNextRow(yPos+=RowDif,true, white);
   // drawNextRow(yPos+=RowDif,false, white);//white);
    //cout<<"Step A"<<endl;
    for(int i=0;i<8;i++){
        ofColor col;
        switch(param[i]){
                
            case 0:
                col=a;
                break;
            case 1:
                col=b;
                break;
            case 2:
                col=c;
                break;
            case 3:
                col=d ;
                break;
            default:
                col=white;
                break;
                
        }
        
        drawNextRow(yPos+=RowDif,true, col);
        drawNextRow(yPos+=RowDif,false, col);
        
    }
    //cout<<"Step B"<<endl;;
    drawNextRow(yPos+=RowDif,true,white);
    drawNextRow(yPos+=RowDif,false, white);
    
    FrameBuffers[ActiveFBO].end();
    
    if(!KeyPressed){
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
        ofFill();
        for(int j=0;j<50;j++){
            ofSetColor(col,ofClamp(ofMap(j,0,50,180,255),180,255) );
            
            //ofDrawEllipse(xPos*SuperSample,yPos*SuperSample,  0.9*columnWidth*SuperSample,0.9*RowHeight*SuperSample);
            
            ofDrawEllipse(xPos*SuperSample,yPos*SuperSample,
                          ofMap(j,0,50,1.13*columnWidth*SuperSample,0.6*columnWidth*SuperSample),
                          ofMap(j,0,50,1.13*RowHeight*SuperSample,0.6*RowHeight*SuperSample));
            
        }
        // ofSetColor(col, 255);
        // const float scaller=0.5;
        // ofDrawRectRounded((xPos*SuperSample)-(scaller*columnWidth*SuperSample)/2,(yPos*SuperSample)-(scaller*RowHeight*SuperSample)/2,  scaller*columnWidth*SuperSample,scaller*RowHeight*SuperSample,5*SuperSample);
        
    }
    
    
}


//-------------------FboHeight-------------------------------------------
void ofApp::draw(){
    
    
    int xPos = (ofGetWidth()-OverlayImage.getWidth())/2;
    int hvar = MIN(OverlayImage.getHeight(),ofGetHeight());
    //preparation
    ofEnableAlphaBlending();
    
    if(scrollDelta>0){
        if(ofGetFrameNum()%(RowHeight*2)>=RowHeight && scrollDelta<=FboHeight){
            
            scrollDelta-=ScrollSpeed*ofGetLastFrameTime();
        }else if(scrollDelta>FboHeight){
            
            scrollDelta-=ScrollSpeed*ofGetLastFrameTime();
        }
    }
    if(scrollDelta<0){
        scrollDelta=0;
    }
    
    int height=ofGetHeight();
    
    
    ///Rendering
    ofBackground(0);
    BackgroundImage.draw(xPos,0,OverlayImage.getWidth(),hvar);
    ofSetColor(255,255,255);
    
    if( doShader ){
        shaderBlurX.begin();
        //we want to pass in some varrying values to animate our type / color
        shaderBlurX.setUniform1f("blurAmnt", 2);
        
        shaderBlurY.begin();
        shaderBlurY.setUniform1f("blurAmnt", 2);
    }
    int hVar = height+scrollDelta;
    int itter = ActiveFBO-1;
    
    for(int i =0; i<FBCount;i++){
        
        if(itter<0){
            itter=FBCount-1;
        }else if(itter>=FBCount){
            itter=0;
        }
        FrameBuffers[itter].draw(xPos,hVar-75,FboWidth,FboHeight);
        
        hVar-=FboHeight;
        itter++;
    }
    
    
    if( doShader ){
        shaderBlurX.end();
        shaderBlurY.end();
    }
    
    OverlayImage.draw(xPos,0,OverlayImage.getWidth(),hvar);
    
    ofDrawBitmapString(ActiveMarkersfound,10,10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == ' '){
        
        int param[8] ;//{ofRandom(100), 2, 3, 17, 50};
        for(int i=0;i<8;i++){
            param[i]=((int)ofRandom(100))%4;
        }
        KeyPressed=true;
        MakeNextSection(param);
    }else if( key == 'b' ){
        doShader = !doShader;
    }
    
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

void ofApp::exit(){
    cout<<"Finishing up Just closing Python"<<endl;
    
    string s ="cd " +ofFilePath::getCurrentExeDir()+ "../../../../python && ./stop.sh";
    system(s.c_str());
}
