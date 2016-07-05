#include "ApproxPi_SingleWindow_Offline.h"


//--------------------------------------------------------------
void ApproxPi_SingleWindow_Offline::setup(){
    
    audio_output_path = "../../../output/";
    frames_output_path = "../output/frames/";
    
    settings.nChannels = 6;
    settings.windowWidth = 1280;
    settings.windowHeight = 1024;
    settings.audioDeviceID = 4;
    settings.sample_rate = 44100;
    settings.dur_in_mins = 1;
    
    settings.doneSetup = true;
    settings.appliedSettings = true;
    
    for(int i=0; i<settings.nChannels; i++)
        approximator.push_back(new ofPiApproximator(settings));
    
    for(int i=0; i<settings.nChannels; i++)
    {
        approximator[i]->main_app = this;
        approximator[i]->setTransposeFactor(TRANSPOSITION_FACTOR[i]);
        approximator[i]->wNum = i+1;
        approximator[i]->fbo.allocate(settings.windowWidth, settings.windowHeight);
        
        
        approximator[i]->playing = true;
    }

    ofSetWindowShape(settings.windowWidth, settings.windowHeight);
    output_fbo.allocate(settings.windowWidth, settings.windowHeight, GL_RGB);
    
    int fps = 30;
    int fps_maxCount = ((float) settings.sample_rate/fps);
    int fps_counter = 1;
    int frameCounter = 0;
    
    int numFrames = settings.dur_in_mins*60.0*fps;

    sf.channels(6);
    sf.frameRate(44100);
    sf.path(audio_output_path+"audio.wav");
    sf.openWrite();
    
    
    do
    {
        tick_audio(sf);
        
        fps_counter--;
        if(fps_counter == 0)
        {
            frameCounter++;
            fps_counter = fps_maxCount;

            draw_everything();
            output_fbo.readToPixels(img);
            img.save( frames_output_path+"frame_"+to_string(frameCounter)+".jpeg");
            cout << frameCounter*100.0/numFrames << "%" << endl;
        }

    } while( endFlag == false);
    
    sf.close();
}

//--------------------------------------------------------------
void ApproxPi_SingleWindow_Offline::update(){
    ofExit();
}

//--------------------------------------------------------------
void ApproxPi_SingleWindow_Offline::draw(){
}

//----
void ApproxPi_SingleWindow_Offline::draw_everything(void)
{
    /// EACH INDIVIDUAL DRAW
    for (int i=0; i<settings.nChannels; ++i)
    {
        ofSetBackgroundColor(approximator[i]->bgColor);
        
        // write to frame buffer object
        approximator[i]->fbo.begin();
        ofTranslate(approximator[i]->fontSize,  approximator[i]->verticalFontMargin);
        ofClear(0, 0, 0, 0);
        ofPushMatrix();
        approximator[i]->drawDigits();
        ofPopMatrix();
        approximator[i]->fbo.end();
    }
    
    /// FULL SCREEN TILED DRAW
    output_fbo.begin(); ///
    
    ofClear(0, 0, 0);
    
    int x = 0;
    int y = 0;
    
    if(settings.nChannels<4)
    {
        for (int i=0; i<settings.nChannels; ++i)
        {
            ofPushMatrix();
            if (!approximator[i]->hasEnded())
            {
                float scaleX = ofGetWindowWidth()*(1.0f/settings.nChannels);
                float scaleY = ofGetWindowHeight();
                ofTranslate(x*scaleX, y*scaleY);
                approximator[i]->scaleContent(scaleX, scaleY);
                approximator[i]->fbo.draw(0,0);
            }
            else
            {
                approximator[i]->drawBlack();
            }
            ofPopMatrix();
            ++x;
        }
    }
    
    else if(settings.nChannels>=4)  // Make two rows
    {
        for (int i=0; i<settings.nChannels; ++i)
        {
            ofPushMatrix();
            if (!approximator[i]->hasEnded())
            {
                float scaleX = ofGetWindowWidth()*(2.0f/settings.nChannels);
                float scaleY = ofGetWindowHeight()*0.5;
                ofTranslate(x*scaleX, y*scaleY);
                approximator[i]->scaleContent(scaleX, scaleY);
                approximator[i]->fbo.draw(0,0);
            }
            else
            {
                approximator[i]->drawBlack();
            }
            ofPopMatrix();
            
            ++x;
            if (x%(settings.nChannels/2) == 0)
            {
                ++y;
                x = 0;
            }
        }
    }
    
    output_fbo.end(); ////
}

void ApproxPi_SingleWindow_Offline::tick_audio(gam::SoundFile& sf)
{
    float* frame = (float*) calloc(settings.nChannels, sizeof(float));
    for (int chan=0; chan<settings.nChannels; chan++)
    {
        // check for when the piece should end
        if (approximator[chan]->sampleCounter >= settings.dur_in_mins*60.0*settings.sample_rate && (approximator[chan]->sampleCounter%(settings.sample_rate/10)) == 0)
        {
            if (!approximator[chan]->hasEnded() && endFlag )
            {
                approximator[chan]->end();
                endFlag = false;
            }
        }
        
        if (approximator[chan]->isPlaying() && !approximator[chan]->hasEnded())
        {
            frame[chan] = approximator[chan]->tick();
        }
    }
    
    sf.write(frame, 1);
    
    if (approximator[0]->sampleCounter >= settings.dur_in_mins*60.0*settings.sample_rate)
    {
        endFlag = true;
    }
    free(frame);
}
