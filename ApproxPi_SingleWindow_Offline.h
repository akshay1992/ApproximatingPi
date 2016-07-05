#pragma once
#include "PiSettings.h"
#include "ofPiApproximator.h"

#include "ofMain.h"
#include <vector>

/*
 This app renders the piece for the desired duration into video frames and audio files. 
 
 This is very bare-bones and hacked together,
 
 
 
 */

class ApproxPi_SingleWindow_Offline : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

    PiSettings settings;
    vector<ofPiApproximator*> approximator;
    
    ofFbo output_fbo;
    ofPixels output_pix;
    ofImage img;
    
    void draw_everything(void);
    void tick_audio(void);
    bool endFlag = false;

};
