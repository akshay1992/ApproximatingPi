#pragma once

#include "ofPiApproximator.h"
#include "ofApproxPiSetupGUI.h"

#include "ofMain.h"
#include "ofxOsc.h"

#include <vector>

#define DEFAULT_OSC_SETTINGS
#ifdef DEFAULT_OSC_SETTINGS
    #define SEND_PORT 12334
    #define SEND_HOST "localhost"
    #define RECEIVE_PORT 12345
    #define RECEIVE_HOST "localhost"
#endif

//--------------------------------------------------------------
// Unsupported/Testing: OSC synchronization between computers.

//#define AKSHAY
//#define MATZE

//#ifdef AKSHAY
//    #define SEND_PORT 12345
//    #define SEND_HOST "matze.local"
//    #define RECEIVE_PORT 12334
//    #define RECEIVE_HOST "Akshay.local"
//#endif
//
//#ifdef MATZE
//    #define SEND_PORT 12334
//    #define SEND_HOST "Akshay.local"
//    #define RECEIVE_PORT 12345
//    #define RECEIVE_HOST "matze.local"
//#endif

//--------------------------------------------------------------

/*
 Openframeworks app that opens Approximating Pi in a single window. Currently supports up to 8 channels.
 
 Depends on the following addons:
 * ofxDatGui (https://github.com/braitsch/ofxDatGui)
 * ofxOSC
 
 Also requires the follwoing assets to be present in the data folder:
 * ofxbraitsch (Available here: https://github.com/braitsch/ofxDatGui/tree/master/ofxbraitsch)
 * pi.png
 * Futura-Medium.ttf
 
 It also requires that the assets are bundled within the `Contents/Resources/data` folder of the built app. You can either move it manually, or use a script to do this automatically on every build.
 
 */

class ApproxPi_SingleWindow : public ofBaseApp {
public:
    ApproxPi_SingleWindow(PiSettings AppSettings);
    ApproxPi_SingleWindow(void);
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