#pragma once

#include "ofMain.h"
#include "PiApproximator.h"
#include "ofxOsc.h"
#include <vector>
#include "PiSettings.h"

#define AKSHAY
//#define MATZE

#ifdef AKSHAY
#define SEND_PORT 12345
#define SEND_HOST "matze.local"
#define RECEIVE_PORT 12334
#define RECEIVE_HOST "Akshay.local"
#endif

#ifdef MATZE
#define SEND_PORT 12334
#define SEND_HOST "Akshay.local"
#define RECEIVE_PORT 12345
#define RECEIVE_HOST "matze.local"
#endif


#define SR 44100
//#define NCHANNELS 2

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 1024

//#define END_TIME_IN_MINUTES 0.2

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
    ofColor fontColor = ofColor::fromHsb(50*(255/360), 96, 99);
    ofColor bgColor = ofColor(0, 0, 0);
};


//--------------------------------------------------------------

class ofAppPi : public ofBaseApp{
public:
    ofAppPi(PiSettings AppSettings);
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    
    void audioOut( float * output, int bufferSize, int nChannels );
    void stopPlayback(void);
    void togglePlay(void);
    void toggleMute(void);
    bool isPlaying(void);
    void toggleStatus(int stat) { status = stat; }
    
    std::string windowNumbers[16] = {"I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X", "XI", "XII", "XIII", "XIV", "XV", "XVI"};

    void setNumChannels(int value);
    std::vector<ofPiApproximator*> approximator;
    int status = 1;
    
    ofxOscSender sender, self_sender;
    ofxOscReceiver receiver;
    
    bool endFlag;
    
    PiSettings settings;
    ofSoundStream soundStream;
};