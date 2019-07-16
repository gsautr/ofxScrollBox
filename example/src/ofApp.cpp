#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate( 60.0f );
    ofBackground(0);
    
    events = scrollBox.event.newListener([this](string & s){
        ofLog() << "[ofxScrollBox] Event: " << s;
    });
    
    scrollBox.outer.x = 100;
    scrollBox.outer.y = 100;
    scrollBox.outer.width = ofGetWidth() - 200;
    scrollBox.outer.height = ofGetHeight() - 200;
    
    scrollBox.inner.set( scrollBox.outer );
    scrollBox.inner.height = 1200;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);
    
    scrollBox.draw();
    
    for(int i = 0; i < 12; i ++) {
        ofRectangle r( 100, scrollBox.inner.y + ( i * 100 ), scrollBox.inner.width, 100 );
        ofNoFill();
        ofSetColor( 0, 255, 255 );
        ofDrawRectangle( r );
    }

    ofSetColor( 255, 255, 0 );
    ofDrawRectangle( scrollBox.outer );
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}
void ofApp::mouseScrolled( ofMouseEventArgs & e ) {
    scrollBox.scrolled(e);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    scrollBox.dragged( x, y );
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    scrollBox.pressed( x, y );
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    scrollBox.released( x, y );
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

    scrollBox.outer.width = ofGetWidth() - 200;
    scrollBox.outer.height = ofGetHeight() - 200;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
