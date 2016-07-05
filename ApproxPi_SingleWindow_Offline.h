#pragma once
#include "PiSettings.h"
#include "ofPiApproximator.h"
#include "from_Gamma/SoundFile.h"

#include "ofMain.h"
#include <vector>

/*
 This app renders the piece for the desired duration into video frames and audio files. 
 
 Setting up XCode:
 =================
 
 - Clone this repo into the src/ folder of openframeworks project.
 - Checkout branch `offline_audio_rendering`
 - Add ALL the files from src/ApproximatingPi (including the ones in src/ApproximatingPi/from_Gamma/
 
 - Make sure to have the following folders inside bin/
    * output/
    * output/frames/
 
 (This is because the output will be stored in the output/ folder)
 

 Running:
 ============
 Build debug (not release).
 
 Set the duration, nChannels etc, in the setup() function.
 
 Console shows progress in percentage as it runs.
 
 Closes once it is finished. No window opens up. 
 
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
