#include "ofApp.h"

const int N = 256;		//Number of bands in spectrum
float spectrum[ N ];	//Smoothed spectrum values
float Rad = 500;		//Cloud raduis parameter
float Vel = 0.1;		//Cloud points velocity parameter
int bandRad = 2;		//Band index in spectrum, affecting Rad value
int bandVel = 100;		//Band index in spectrum, affecting Vel value

const int n = 300;		//Number of cloud points

//Offsets for Perlin noise calculation for points
float tx[n], ty[n];

ofPoint p[n];			//Cloud's points positions

float time0 = 0;		//Time value, used for dt computing
//--------------------------------------------------------------

void ofApp::setup(){
    //Set up sound sample
    sound.loadSound( "EXO.mp3" );
    background.load("");
    sound.setLoop( true );
    sound.play();
    
    //Set spectrum values to 0
    for (int i=0; i<N; i++) {
        spectrum[i] = 0.0f;
    }
    
    //Initialize points offsets by random numbers
    for ( int j=0; j<n; j++ ) {
        tx[j] = ofRandom( 0, 1000 );
        ty[j] = ofRandom( 0, 1000 );
    }

}

//--------------------------------------------------------------
void ofApp::update(){
    ofSoundUpdate();
    
    //Get current spectrum with N bands
    float *val = ofSoundGetSpectrum( N );
    
    //Slowly falling peaks in spectrum
    for ( int i=0; i<N; i++ ) {
        spectrum[i] *= 0.97;	//Slow decreasing
        spectrum[i] = max( spectrum[i], val[i] );
    }
    
    //Update particles using spectrum values
    
    //Computing dt as a time between the last
    float time = ofGetElapsedTimef();
    float dt = time - time0;
    dt = ofClamp( dt, 0.0, 0.1 );
    time0 = time; //Store the current time
    
    Rad = ofMap( spectrum[ bandRad ], 1, 3, 400, 800, true );
    Vel = ofMap( spectrum[ bandVel ], 0, 0.1, 0.05, 0.5 );
    
    //Update particles positions
    for (int j=0; j<n; j++) {
        tx[j] += Vel * dt;	//move offset
        ty[j] += Vel * dt;	//move offset
        
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
    
    //Move center of coordinate system to the screen center
    ofPushMatrix();
    ofTranslate( ofGetWidth() / 2, ofGetHeight() / 2 );
    
    //Draw points
    ofSetColor( 51, 0, 51 );
    ofFill();
    for (int i=0; i<n; i++) {
        ofCircle( p[i], 2 );
    }
    
    //Draw lines between near points
    float dist = 80;	//Threshold parameter of distance
    
    for (int j=0; j<n; j++) {
        for (int k=j+1; k<n; k++) {
            if ( ofDist( p[j].x, p[j].y, p[k].x, p[k].y )
                < dist ) {
                ofLine( p[j], p[k] );
            }
        }
    }
    
    //Restore coordinate system
    ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
