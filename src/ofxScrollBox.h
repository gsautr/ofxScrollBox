#pragma once

struct ofxScrollAnim {
    float length;
    float from;
    float to;
    float timeline;
    string direction;
    bool active;
};

#define SCROLL_NOWT 1
#define SCROLL_CHECK 2
#define SCROLL_PASSED 3

struct ofxScrollBoxSettings {
    float elasticDistance;
    float elasticSpeed;
    float elasticBalance;
    float barRounded;
    float barOuterWidth;
    float barWidth;
    float barPadding;
    float barHoverSpeed;
    bool customDraw;
    ofColor barInnerColor;
    ofColor barOuterColor;
    
    ofxScrollBoxSettings() {
        
        elasticDistance = 3;
        elasticSpeed = 15;
        elasticBalance = 0.3;
        barRounded = 1;
        barWidth = 6;
        barPadding = 4;
        barOuterWidth = 16;
        barHoverSpeed = 0.4;
        customDraw = false;
        barInnerColor.set(255);
        barOuterColor.set(255,255,255,80);
    }
};

class ofxScrollBox {
    
private:
    
    ofxScrollBoxSettings s;
    float velocity;
    float activeTimestamp;
    float timestamp;
    float timeline;
    ofxScrollAnim a;
    int scrollState; // NOWT, CHECK, PASSED
    int lastDirection;
    ofRectangle scrollBar;
    ofRectangle scrollBarOuter;
    float lastInnerHeight;
    float lastOuterHeight;
    float lastInnerWidth;
    float lastOuterWidth;
    float hoverTimestamp;
    float hoverOpacity;
    float hoverBarTimestamp;
    float hoverScale;
    bool lastInside;
    bool lastInsideBar;
    
    float easeOut(float v, float minIn, float maxIn, float minOut, float maxOut) {
        float t = v - minIn;
        float c = maxOut - minOut;
        float d = maxIn - minIn;
        float b = minOut;
        return -c *(t/=d)*(t-2) + b;
    }
    
    float easeInOut(float v, float minIn, float maxIn, float minOut, float maxOut) {
        float t = v - minIn;
        float c = maxOut - minOut;
        float d = maxIn - minIn;
        float b = minOut;
        if ((t/=d/2) < 1) return c/2*t*t + b;
        return -c/2 * ((--t)*(t-2) - 1) + b;
    }
    
    void setScrollBar() {
        
        float oY = outer.y;
        float oH = outer.height;
        float iY = inner.y;
        float iH = inner.height;
        
        
        scrollBarOuter.x = outer.getTopRight().x - s.barOuterWidth;
        scrollBarOuter.y = outer.y;
        scrollBarOuter.width = s.barOuterWidth;
        scrollBarOuter.height = outer.height;
        
        
        scrollBar.set( scrollBarOuter );
        scrollBar.height = ofMap( oH, 0, iH, 0, oH);
        
        
        float fromA = oY;
        float fromB = (oH - iH) + oY;
        float toA = oY;
        float toB = oY + oH - scrollBar.height;
        
        scrollBar.y = ofMap( iY, fromB, fromA, toB, toA );
        
        if (scrollBar.y < oY) {
            scrollBar.height -= oY - scrollBar.y;
            scrollBar.y = oY;
        }
        float ibr = scrollBar.getBottomRight().y;
        float obr = outer.getBottomRight().y;
        if (ibr > obr) {
            scrollBar.height -= ibr - obr;
        }
        
        float boop = s.barOuterWidth - ( s.barPadding * 2 );
        scrollBar.width = ( boop * hoverScale ) + ( s.barWidth * (1-hoverScale) );
        scrollBar.x = outer.getTopRight().x - scrollBar.width - s.barPadding;
        scrollBar.y += s.barPadding;
        scrollBar.height -= s.barPadding * 2;
        
    }
    
    void set() {
        
        if (inner.y != scrollY) {
            inner.y = scrollY;
            setScrollBar();
            string a = "scrolled";
            ofNotifyEvent(event, a);
        }
    }
    
public:
    
    ofEvent<string &> event;
    ofRectangle outer, inner;
    float scrollY;
    
    ofxScrollBox() {
        lastDirection = 9;
        scrollY = 999999;
        velocity = 0;
        timestamp = 0;
        activeTimestamp = 0;
        scrollState = SCROLL_PASSED;
        lastInside = false;
        hoverOpacity = 0;
        hoverBarTimestamp = 0;
        hoverScale = 0;
    }
    
    ofRectangle & getOuterBox() {
        return outer;
    }
    ofRectangle & getInnerBox() {
        return outer;
    }
    
    float getScrollY() {
        return scrollY;
    }
    
    ofRectangle & getScrollBar() {
        return scrollBar;
    }
    
    void draw() {
        
        if (inner.height < outer.height) return;
        
        if (scrollY == 999999) {
            scrollY = inner.y;
            set();
        }
        
        float t = ofGetElapsedTimef();
        
        if (scrollState == SCROLL_CHECK) {
            if (t - timestamp > 0.1) {
                scrollState = SCROLL_PASSED;
                ofLogVerbose("[ofxScrollBox]") << "Native timeout PASSED";
            }
        }
        
        if (a.active) {
            
            float l = a.length;
            float split = s.elasticBalance;
            
            if (a.timeline < l * split) {
                scrollY = easeOut(a.timeline, 0, l * split, a.from, a.to);
            } else {
                scrollY = easeInOut(a.timeline, l * split, l, a.to, a.from);
            }
            
            float fr = 1.0/ofGetFrameRate();
            a.timeline += fr;
        
            if (a.timeline >= a.length) {
                ofLogVerbose("[ofxScrollBox]") << "Animate END";
                a.active = false;
                a.timeline = 0;
            }
            set();
        }
        
        bool innerDiff = lastInnerHeight != inner.height || lastInnerWidth != inner.width;
        bool outerDiff = lastOuterHeight != outer.height || lastOuterWidth != outer.width;
        
        if (innerDiff || outerDiff) {
            
            if (innerDiff) {
                lastInnerHeight = inner.height;
                lastInnerWidth = inner.width;
                string a = "innerBoxResized";
                ofNotifyEvent(event, a);
            }
            if (outerDiff) {
                lastOuterHeight = outer.height;
                lastOuterWidth = outer.width;
                string a = "outerBoxResized";
                ofNotifyEvent(event, a);
            }
            setScrollBar();
        }
        
        
        bool isInsideBar = scrollBarOuter.inside( ofGetMouseX(), ofGetMouseY() );
        if (isInsideBar != lastInsideBar) hoverBarTimestamp = t;
        lastInsideBar = isInsideBar;
        
        bool isInside = outer.inside( ofGetMouseX(), ofGetMouseY() );
        if (isInside != lastInside) hoverTimestamp = t;
        lastInside = isInside;
        
        bool drawOuter = (isInsideBar || (t < hoverBarTimestamp + s.barHoverSpeed) || isDragged);
        bool drawBar = (isInside || (t < hoverTimestamp + s.barHoverSpeed) || isDragged);
        
        if (drawOuter) {
            
            if (isInsideBar) hoverScale = easeInOut(t, hoverBarTimestamp, hoverBarTimestamp + s.barHoverSpeed, hoverScale, 1);
            if (!isInsideBar && !isDragged) hoverScale = easeInOut(t, hoverBarTimestamp, hoverBarTimestamp + s.barHoverSpeed, hoverScale, 0);
            
            setScrollBar();
        }
        
        if (drawBar) {
            
            if (isInside) hoverOpacity = easeInOut(t, hoverTimestamp, hoverTimestamp + s.barHoverSpeed, hoverOpacity, 255);
            if (!isInside && !isDragged) hoverOpacity = easeInOut(t, hoverTimestamp, hoverTimestamp + s.barHoverSpeed, hoverOpacity, 0);
            
        }
        
        if ((drawOuter || drawBar)&&(!s.customDraw)) {
            
            ofEnableAlphaBlending();
            ofFill();
            ofSetColor(s.barOuterColor, hoverScale * s.barOuterColor[3]);
            ofDrawRectangle( scrollBarOuter );
            ofFill();
            ofSetColor(s.barInnerColor, hoverOpacity);
            ofDrawRectRounded( scrollBar , s.barRounded * scrollBar.width);
        }
        
        if (s.customDraw) {
            string a = "drawing";
            ofNotifyEvent( event, a );
        }
        
    }
    void scrolled( ofMouseEventArgs & e ) {
        
        if (inner.height < outer.height) return;
        if (!outer.inside( ofGetMouseX(), ofGetMouseY() )) return;
        
        float t = ofGetElapsedTimef();
        float d = ( e.scrollY * -3 );
        
        bool directionUp = (e.scrollY > 0);
        bool directionDown = (e.scrollY < 0);
        
        
        bool isOpposite = (directionDown && lastDirection == -1)||(directionUp && lastDirection == 1);
        if (isOpposite) scrollState = SCROLL_PASSED;
        
        lastDirection = 0;
        if (directionUp) lastDirection = -1;
        if (directionDown) lastDirection = 1;
        
        if (!a.active) {
            
            bool isStateful = (scrollState == SCROLL_PASSED);
            
            if (isStateful) {
                scrollY = scrollY - d;
                set();
            }
            
            float outerY = outer.y;
            float outerH = outer.height;
            float innerH = inner.height;
            
            float bottomInner = inner.getBottomRight().y;
            float bottomOuter = outer.getBottomRight().y;
            
            bool isLess = bottomInner < bottomOuter;
            bool isLessThan = bottomOuter - bottomInner > 4;
            
            bool isMore = scrollY > outerY;
            bool isMoreThan = scrollY - outerY > 4;
            
            float length = ( t - timestamp ) * s.elasticSpeed;
            if (length > 2) length = 1;
            float distance = d * s.elasticDistance;
            
            if (isLess && isLessThan && isStateful) {
                
                scrollState = SCROLL_CHECK;
                ofLogVerbose("[ofxScrollBox]") << "Animate UP" << length << a.from << a.to;
                a.direction = "UP";
                a.timeline = 0;
                a.active = true;
                a.length = length;
                a.from = outer.y - (innerH - outerH);
                a.to = outer.y - (innerH - outerH) - (distance);
            }
            
            if (isMore && isMoreThan && isStateful) {

                scrollState = SCROLL_CHECK;
                ofLogVerbose("[ofxScrollBox]") << "Animate DOWN" << length << a.from << a.to;
                a.direction = "DOWN";
                a.timeline = 0;
                a.active = true;
                a.length = length;
                a.from = outer.y;
                a.to = outer.y - (distance);
            }
            
        }
        
        timestamp = t;
        
    }
    
    bool isDragged;
    ofPoint dragOrigin;
    ofRectangle dragOriginBar;
    
    void pressed( int x, int y ) {
        
        if (scrollBarOuter.inside(x, y)) {
            
            dragOrigin.set(x, y);
            dragOriginBar = scrollBar;
            isDragged = true;
        }
    }
    void dragged( int x, int y ) {
        if (isDragged) {
            
            float start = ( dragOrigin.y - dragOriginBar.y ) + outer.y;
            float end = ( outer.y + outer.height ) - ( (dragOriginBar.y + dragOriginBar.height) - dragOrigin.y);
            scrollY = ofMap( y, start, end, outer.y, ( outer.height - inner.height ) + outer.y, true);
            hoverTimestamp = ofGetElapsedTimef();
            hoverBarTimestamp = hoverTimestamp;
            set();
        }
        
    }
    void released( int x, int y ) {
        isDragged = false;
        
    }
    
};
