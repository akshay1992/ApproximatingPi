#include "ApproxPi_SingleWindow.h"

ApproxPi_SingleWindow::ApproxPi_SingleWindow(PiSettings AppSettings)
{
    settings = AppSettings;
    applySettings();
}

ApproxPi_SingleWindow::ApproxPi_SingleWindow(void)
{
    setup_app = new ofApproxPiSetupGUI(settings);
    setup_app->setup();
}

void ApproxPi_SingleWindow::setup(){
    ofSetDataPathRoot("../Resources/data/");
}

void ApproxPi_SingleWindow::windowSetup(void)
{
    ofSetWindowShape(settings.windowWidth, settings.windowHeight);
}

void ApproxPi_SingleWindow::soundSetup(void)
{
    soundStream.setDeviceID(settings.audioDeviceID);
    soundStream.setup(this, settings.nChannels, 0, settings.sample_rate, 256, 2);
}

void ApproxPi_SingleWindow::oscSetup(void)
{
    receiver.setup(RECEIVE_PORT);
    sender.setup(SEND_HOST, SEND_PORT);
    self_sender.setup(RECEIVE_HOST, RECEIVE_PORT);
}

void ApproxPi_SingleWindow::applySettings(void)
{
//    settings.print();
    approximatorSetup();
    oscSetup();
    soundSetup();
    windowSetup();
    settings.appliedSettings = true;
}

void ApproxPi_SingleWindow::approximatorSetup()
{
    for(int i=0; i<settings.nChannels; i++)
        approximator.push_back(new ofPiApproximator(settings));

    for(int i=0; i<settings.nChannels; i++)
    {
        approximator[i]->main_app = this;
        approximator[i]->setTransposeFactor(TRANSPOSITION_FACTOR[i]);
        approximator[i]->wNum = i+1;
        approximator[i]->fbo.allocate(settings.windowWidth, settings.windowHeight);
    }
}

void ApproxPi_SingleWindow::update(){
    if(settings.doneSetup && !settings.appliedSettings)
    {
        if(settings.exit)
            ofExit();
        else
            applySettings();
    }
    
    if(settings.appliedSettings)
    {
        for (int i=0; i<settings.nChannels; ++i)
        {
            ofSetBackgroundColor(approximator[i]->bgColor);
            
            // write to frame buffer object
            approximator[i]->fbo.begin();
            ofTranslate(approximator[i]->fontSize,  approximator[i]->verticalFontMargin);
            ofClear(0, 0, 0, 0);
            ofPushMatrix();
            if (status == 2) {
                approximator[i]->drawDigits();
            } else if (status == 1) {
                approximator[i]->drawStatus();
            } else if (status == 0) {
                approximator[i]->drawBlack();
            }
            ofPopMatrix();
            approximator[i]->fbo.end();
        }
        
        
        while( receiver.hasWaitingMessages() )
        {
            // get the next message
            ofxOscMessage m;
            receiver.getNextMessage(&m);
            if (m.getAddress() == "/togglePlay")
            {
                togglePlay();
            }
            if (m.getAddress() == "/reset")
            {
                stopPlayback();
            }
            if (m.getAddress() == "/toggleMute")
            {
                toggleMute();
            }
            if (m.getAddress() == "/toggleStatus")
            {
                int statusValue = m.getArgAsInt(0);
                toggleStatus(statusValue);
            }
        }
    }
}

void ApproxPi_SingleWindow::draw(){
    if(settings.appliedSettings)
    {
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
    }
}

void ApproxPi_SingleWindow::toggleMute() {
    for(int i=0; i<settings.nChannels; i++) {
        approximator[i]->toggleMute();
    }
}

void ApproxPi_SingleWindow::stopPlayback() {
    for(int i=0; i<settings.nChannels; i++) {
        approximator[i]->playing = false;
        approximator[i]->Reset();
    }
}

void ApproxPi_SingleWindow::togglePlay(void) {
    for(int i=0; i<settings.nChannels; i++)
        approximator[i]->playing = !approximator[i]->playing;
}

bool ApproxPi_SingleWindow::isPlaying(void) { // Returns false if any one of them is not playing
    bool return_value;
    for(int i=0; i<settings.nChannels; i++)
        return_value = ( return_value && approximator[i]->isPlaying() );
    return return_value;
}

void ApproxPi_SingleWindow::keyPressed(int key){
    if(settings.appliedSettings)
    {
        if(key == ' ')
        {
            ofxOscMessage m;
            m.setAddress("/togglePlay");
            sender.sendMessage( m );
            self_sender.sendMessage(m);
        }
        if(key == 'm')
        {
            ofxOscMessage m;
            m.setAddress("/toggleMute");
            sender.sendMessage( m );
            self_sender.sendMessage(m);
        }
        if(key == '1')
        {
            status = 0;
            ofxOscMessage m;
            m.setAddress("/toggleStatus");
            m.addIntArg(status);
            sender.sendMessage( m );
            self_sender.sendMessage(m);
        }
        if(key == '2')
        {
            status = 1;
            ofxOscMessage m;
            m.setAddress("/toggleStatus");
            m.addIntArg(status);
            sender.sendMessage( m );
            self_sender.sendMessage(m);
        }
        if(key == '3')
        {
            status = 2;
            ofxOscMessage m;
            m.setAddress("/toggleStatus");
            m.addIntArg(status);
            sender.sendMessage( m );
            self_sender.sendMessage(m);
        }
        if(key == '0')
        {
            ofxOscMessage m;
            m.setAddress("/reset");
            sender.sendMessage( m );
            self_sender.sendMessage(m);
        }
    }
}

void ApproxPi_SingleWindow::audioOut( float * output, int bufferSize, int nChannels ) {
    for (int i=0; i<bufferSize * nChannels; i+=nChannels)
    {
        for (int chan=0; chan<nChannels; chan++)
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
                    output[i+chan] = approximator[chan]->tick();
            }
        }
        
        if (approximator[0]->sampleCounter >= settings.dur_in_mins*60.0*settings.sample_rate)
        {
            endFlag = true;
        }
    }
}

void ApproxPi_SingleWindow::exit(void) {
    ofSoundStreamClose();
}

