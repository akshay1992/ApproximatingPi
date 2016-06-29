#ifndef ofPiApproximator_h
#define ofPiApproximator_h

#include "ofMain.h"

#include "PiApproximator.h"
#include "PiSettings.h"

class ofPiApproximator : public PiApproximator{
public:
    using PiApproximator::PiApproximator;
    ofPiApproximator(PiSettings &settings);
    
    int wNum;
    ofImage piSymbol;
    ofTrueTypeFont NumberFont, StatusFont;
    
    bool isPlaying(void) {return playing;}
    
    virtual void drawDigits(void) override;
    
    virtual void drawStatus(void) override;
    
    void scaleContent(int w, int h);
    
    void drawBlack(void) {};
    
    ofBaseApp* main_app;
    
    ofFbo fbo;
    
    bool playing = false;
    
    float verticalFontMargin;
    float myHue = 25;
    float fontSize;
    std::string windowNumbers[16] = {"I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X", "XI", "XII", "XIII", "XIV", "XV", "XVI"};
    ofColor fontColor = ofColor::fromHsb(50*(255/360), 96, 99);
    ofColor bgColor = ofColor(0, 0, 0);
    
    PiSettings* settings_ref;
};

#endif
