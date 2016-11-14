#include "ofApp.h"

const int N = 256;		//Number of bands in spectrum
float spectrum[ N ];
float Rad = 500;		//raduis
float Vel = 0.1;		//velocity

int bandRad = 2;		//band index
int bandVel = 100;

const int n = 300;		//points

float tx[n], ty[n];

ofPoint p[n];

float time0 = 0;
//--------------------------------------------------------------

void ofApp::setup(){
    //Set up sound sample
    sound.loadSound( "EXO.mp3" );
    sound.setLoop( true );
    sound.play();
    
    synth.load("synth.wav");
    synth.setVolume(0.85f);

    for (int i=0; i<N; i++) {
        spectrum[i] = 0.0f;
    }
    
    //Initialize
    for ( int j=0; j<n; j++ ) {
        tx[j] = ofRandom( 0, 1000 );
        ty[j] = ofRandom( 0, 1000 );
    }

}

//--------------------------------------------------------------
void ofApp::update(){
    ofSoundUpdate();
    float *val = ofSoundGetSpectrum( N );
    for ( int i=0; i<N; i++ ) {
        spectrum[i] *= 0.98;
        spectrum[i] = max( spectrum[i], val[i] );
    }
    //Update particles using spectrum
    float time = ofGetElapsedTimef();
    float dt = time - time0;
    
    dt = ofClamp( dt, 0.0, 0.1 );
    time0 = time;
    
    Rad = ofMap( spectrum[ bandRad ], 1, 3, 400, 800, true );
    Vel = ofMap( spectrum[ bandVel ], 0, 0.1, 0.05, 0.5 );
    
    //Update particles positions
    for (int j=0; j<n; j++) {
        tx[j] += Vel * dt;
        ty[j] += Vel * dt;
        
        p[j].x = ofSignedNoise( tx[j] ) * Rad;		
        p[j].y = ofSignedNoise( ty[j] ) * Rad;	
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofColor colorOne(255,228,225);
    ofColor colorTwo(255,192,203);
    ofBackgroundGradient(colorOne, colorTwo, OF_GRADIENT_LINEAR);
    
    //Draw cloud
    ofPushMatrix();
    ofTranslate( ofGetWidth() / 2, ofGetHeight() / 2 );
    
    
    
    float hue = fmodf(ofGetElapsedTimef()*10,255);
    ofColor color = ofColor::fromHsb(hue,
                                     ofMap( mouseX, 0,ofGetWidth(), 0,255 ),
                                     ofMap( mouseY, ofGetHeight(),0, 0,255 ) );

    ofSetColor( color );
    ofFill();
    for (int i=0; i<n; i++) {
        ofCircle( p[i], 2 );
    }
    
    float dist = 80;	//distance
    
    for (int j=0; j<n; j++) {
        for (int k=j+1; k<n; k++) {
            if ( ofDist( p[j].x, p[j].y, p[k].x, p[k].y )
                < dist ) {
                ofLine( p[j], p[k] );
            }
        }
    }
    ofPopMatrix();
    
   
    ofSetColor( color );
    ofFill();
    ofDrawCircle( mouseX, mouseY, 10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f') ofToggleFullscreen();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    float widthStep = ofGetWidth();
    if (x < widthStep){
        synth.play();
        synth.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*10);
        synth.setPan(ofMap(x, 0, widthStep, -1, 1, true));
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
