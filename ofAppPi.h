#pragma once

//#define SR 44100

#include "ApproximatingPi/ofPiApproximator.h"
#include "ApproximatingPi/ofApproxPiSetupGUI.h"

#include "ofMain.h"
#include "ofxOsc.h"

#include <vector>

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

//--------------------------------------------------------------

class ofAppPi : public ofBaseApp {
public:
    ofAppPi(PiSettings AppSettings);
    ofAppPi(void);
    void setup();
    void update();
    void draw();
    void exit();
    
    void applySettings(void);
    void soundSetup();
    void oscSetup();
    void windowSetup();
    void approximatorSetup();
    
    static int defaultWindowWidth(void) { return 1280;}
    static int defaultWindowHeight(void) { return 1024;}
    
    void keyPressed(int key);
    
    void audioOut( float * output, int bufferSize, int nChannels );
    void stopPlayback(void);
    void togglePlay(void);
    void toggleMute(void);
    bool isPlaying(void);
    void toggleStatus(int stat) { status = stat; }
    
    void setNumChannels(int value);
    std::vector<ofPiApproximator*> approximator;
    int status = 1;
    
    ofxOscSender sender, self_sender;
    ofxOscReceiver receiver;
    
    bool endFlag;
    
    PiSettings settings;
    ofApproxPiSetupGUI* setup_app;
    bool internal_setup;
    ofSoundStream soundStream;
};