#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate( 60.0f );
    ofBackground(0);
    
    /*-- always do this: define 2 rectangles  --*/
    
    scrollBox.outer.x = 100;
    scrollBox.outer.y = 100;
    scrollBox.outer.width = ofGetWidth() - 200;
    scrollBox.outer.height = ofGetHeight() - 200;
    
    scrollBox.inner.set( scrollBox.outer );
    scrollBox.inner.height = 1200;
    
    /*-- extra: log events  --*/
    
    events = scrollBox.event.newListener([this](string & s){
        ofLog() << "[ofxScrollBox] Event: " << s;
    });
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    /*-- always do this: --*/
    
    ofSetColor(255);
    scrollBox.draw();
    
    /*-- extra: draw inner boxes
     --*/
    
    for(int i = 0; i < 23; i ++) {
        ofRectangle r( 100, scrollBox.inner.y + ( i * 50 ), scrollBox.inner.width, 100 );
        ofNoFill();
        ofSetColor( 0, 255, 255 );
        ofDrawRectangle( r );
        ofSetColor( 255 );
        if (i < rows.size()) ofDrawBitmapString( rows[i], r.x + 20, r.getCenter().y - 20 );
    }

    /*-- extra: draw outer box --*/
    
    bool isInsideScrollBox = (scrollBox.getOuterBox().inside( ofGetMouseX(), ofGetMouseY() ));
    (isInsideScrollBox) ? ofSetColor( 255 ) : ofSetColor( 255, 255, 0 );
    ofDrawRectangle( scrollBox.outer );
    
}


/*-- always do this: pass events --*/
/*-- dragged, pressed, released, scrolled --*/

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

void ofApp::mouseScrolled( ofMouseEventArgs & e ) {
    scrollBox.scrolled(e);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ') {
        
        /*-- extra: generate some nonsense --*/
        
        rows.clear();
        vector<string> ladida = {"li", "la", "di", "da", "di"};
        for (int i = 0; i < 25; i++) {
            string scrolli = "Scrol";
            for (int ii = 0; ii < ofRandom( 0, 7); ii++ ) {
                int rand = (ii == 0) ? ofRandom( 0, 2) : ofRandom(0, ladida.size());
                scrolli += ladida[ rand ];
            }
            rows.push_back(scrolli);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

    scrollBox.inner.width = ofGetWidth() - 200;
    scrollBox.outer.width = ofGetWidth() - 200;
    scrollBox.outer.height = ofGetHeight() - 200;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
