#pragma once
#include "PiSettings.h"
#include "ofPiApproximator.h"
#include "../../Gamma/Gamma/Recorder.h"
#include "../../Gamma/Gamma/SoundFile.h"

#include "ofMain.h"
#include <vector>

/*
 This app renders the piece for the desired duration into video frames and audio files. 
 
 Setting up XCode:
 =================
 
 
 Set the duration, nChannels etc, in the setup() function.
 
 Console shows progress in percentage as it runs. 
 
 Make sure to have the following folders inside bin/
 * output
 * output/frames

 The output will be stored in the output/ folder. 
 
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
    void tick_audio(gam::SoundFile& sf);
    bool endFlag = false;
    
    std::string audio_output_path, frames_output_path;
    
    gam::SoundFile sf;

};
