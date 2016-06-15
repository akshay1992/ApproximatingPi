#include "ofAppPi.h"

//--------------------------------------------------------------
//        ofAppPi_SingleWindow
//--------------------------------------------------------------

ofAppPi::ofAppPi(void) {
    for(int i=0; i<NCHANNELS; i++)
    {
        approximator[i].main_app = this;
        approximator[i].setTransposeFactor(TRANSPOSITION_FACTOR[i]);
        approximator[i].wNum = i+1;
        approximator[i].fbo.allocate(ofGetWindowWidth(), ofGetWindowHeight());
    }
}

//--------------------------------------------------------------
void ofAppPi::setup(){
    ofSetDataPathRoot("/Resources/data/");
    
    receiver.setup(RECEIVE_PORT);
    sender.setup(SEND_HOST, SEND_PORT);
    self_sender.setup(RECEIVE_HOST, RECEIVE_PORT);
    
    ofSoundStreamSetup(NCHANNELS, 0);
}

//--------------------------------------------------------------
void ofAppPi::update(){
    
    for (int i=0; i<NCHANNELS; ++i)
    {
        ofSetBackgroundColor(approximator[i].bgColor);
        
        // write to frame buffer object
        approximator[i].fbo.begin();
        ofTranslate(approximator[i].fontSize,  approximator[i].verticalFontMargin);
        ofClear(0, 0, 0, 0);
        ofPushMatrix();
        if (status == 2) {
            approximator[i].drawDigits();
        } else if (status == 1) {
            approximator[i].drawStatus();
        } else if (status == 0) {
            approximator[i].drawBlack();
        }
        ofPopMatrix();
        approximator[i].fbo.end();
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

//--------------------------------------------------------------
void ofAppPi::draw(){
    int x = 0;
    int y = 0;
    for (int i=0; i<NCHANNELS; ++i)
    {
        ofPushMatrix();
        if (!approximator[i].hasEnded())
        {
            float scaleX = ofGetWindowWidth()*0.33;
            float scaleY = ofGetWindowHeight()*0.5;
            ofTranslate(x*scaleX, y*scaleY);
            approximator[i].scaleContent(scaleX, scaleY);
            approximator[i].fbo.draw(0,0);
        }
        else
        {
            approximator[i].drawBlack();
        }
        ofPopMatrix();
        
        ++x;
        if (x%3 == 0)
        {
            ++y;
            x = 0;
        }
    }
}

//--------------------------------------------------------------
void ofAppPi::toggleMute() {
    for(int i=0; i<NCHANNELS; i++) {
        approximator[i].toggleMute();
    }
}

//--------------------------------------------------------------
void ofAppPi::stopPlayback() {
    for(int i=0; i<NCHANNELS; i++) {
        approximator[i].playing = false;
        approximator[i].Reset();
    }
}

//--------------------------------------------------------------
void ofAppPi::togglePlay(void) {
    for(int i=0; i<NCHANNELS; i++)
        approximator[i].playing = !approximator[i].playing;
}

//--------------------------------------------------------------

bool ofAppPi::isPlaying(void) { // Returns false if any one of them is not playing
    bool return_value;
    for(int i=0; i<NCHANNELS; i++)
        return_value = ( return_value && approximator[i].isPlaying() );
    return return_value;
}

//--------------------------------------------------------------
void ofAppPi::keyPressed(int key){
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

//--------------------------------------------------------------
void ofAppPi::audioOut( float * output, int bufferSize, int nChannels ) {
    for (int i=0; i<bufferSize * nChannels; i+=nChannels)
    {
        
        for (int chan=0; chan<nChannels; chan++)
        {
            // check for when the piece should end
            if (approximator[chan].sampleCounter >= END_TIME_IN_MINUTES*60.0*SR && (approximator[chan].sampleCounter%4410) == 0)
            {
                if (!approximator[chan].hasEnded() && endFlag )
                {
                    approximator[chan].end();
                    endFlag = false;
                }
            }
            
            if (approximator[chan].isPlaying() && !approximator[chan].hasEnded())
            {
                    output[i+chan] = approximator[chan].tick();
            }
        }
        
        if (approximator[0].sampleCounter >= END_TIME_IN_MINUTES*60.0*SR)
        {
            endFlag = true;
        }
    }
}

//--------------------------------------------------------------
void ofAppPi::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofAppPi::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofAppPi::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofAppPi::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofAppPi::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofAppPi::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofAppPi::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofAppPi::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofAppPi::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofAppPi::dragEvent(ofDragInfo dragInfo){ 
    
}
//--------------------------------------------------------------
void ofAppPi::exit(void) {
    ofSoundStreamClose();
}

//--------------------------------------------------------------
//            ofPiApproximator
//--------------------------------------------------------------


ofPiApproximator::ofPiApproximator(void)
{
    NumberFont.load("Futura-Medium.ttf", fontSize);
    StatusFont.load("Futura-Medium.ttf", fontSize*0.25);
    piSymbol.load("pi.png");
    verticalFontMargin = fontSize*0.15;
    ofBackground(0,0,0);
};

//--------------------------------------------------------------
void ofPiApproximator::drawDigits(void)
{
    float number = currentApprox;
    int lineIndex = 1;
    int place = 1;
    float letterWidthScaler = 0.8;
    
    piSymbol.draw(-fontSize*0.5, verticalFontMargin);
    
    int digits[APPROXIMATOR_PRECISION];
    getDigits(number, digits);
    
    // draw the equal sign
    ofPushMatrix();
    fontColor.setHsb(myHue, 255, 255);
    ofSetColor(fontColor);
    ofTranslate(0.75*fontSize, 0);
    ofScale(0.55, 1.0);
    NumberFont.drawString("=", 0, (fontSize + verticalFontMargin) * lineIndex);
    ofPopMatrix();
    
    // loop over the digits
    for (int i=0; i<APPROXIMATOR_PRECISION; i++)
    {
        float brightnessScaler;
        float saturationScaler;
        
        // offset the letters in the first line by half letter width
        ofPushMatrix();
        if (lineIndex == 1)
            ofTranslate(fontSize*0.4, 0);
        
        // Also draw the dot here
        if (i==1)
        {
            brightnessScaler = 255;
            saturationScaler = 255;
            fontColor.setHsb(myHue, saturationScaler, brightnessScaler);
            ofSetColor(fontColor);
            NumberFont.drawString(".", (place*fontSize*letterWidthScaler) + (NumberFont.getLetterSpacing()*fontSize*letterWidthScaler*0.25), (fontSize + verticalFontMargin) * lineIndex);
            place++;
        }
        
        // draw the digits
        brightnessScaler = (0.3 + digits[i]*0.1)*255;
        saturationScaler = (pow(digits[i]*0.1,2))*255;
        fontColor.setHsb(myHue, 255-saturationScaler, brightnessScaler);
        ofSetColor(fontColor);
        NumberFont.drawString(to_string(digits[i]), place*fontSize*letterWidthScaler, (fontSize + verticalFontMargin) * lineIndex);
        ofPopMatrix();
        ++place;
        
        // jump to the beginning of the next line
        if (i == 1 || i == 5)
        {
            ++lineIndex;
            place = 1;
        }
    }
}

//--------------------------------------------------------------
void ofPiApproximator::scaleContent(int w, int h) {
    float horizontalWindowScaleFactor = w / float(fontSize*6.0);
    float verticalWindowScaleFactor = h / float(fontSize*3.8);
    ofScale(horizontalWindowScaleFactor, verticalWindowScaleFactor);
}

//--------------------------------------------------------------

void ofPiApproximator::drawStatus(void)
{
    int lineSpacing = (fontSize+verticalFontMargin)*0.75;
    int xMargin = 100;
    
    // draw window number
    fontColor.setHsb(myHue, 255, 255);
    ofSetColor(fontColor);
    string wN = "Window: " + to_string(wNum);
    StatusFont.drawString(wN, xMargin, lineSpacing*1);
    
    // draw time elapsed status
    float seconds = sampleCounter / float(SR);
    int displayHours = std::floor(seconds/3600.0);
    int displayMinutes = std::floor(std::fmod((seconds/60.0),60.0));
    int displaySeconds = std::fmod(seconds,60);
    
    std::string time = ofToString(displayHours) + ":" + ofToString(displayMinutes) + ":" + ofToString(displaySeconds);
    StatusFont.drawString(time, xMargin, lineSpacing*2);
    
    // draw play/pause status
    if (isPlaying()) {
        ofSetColor(0, 255, 0);
        StatusFont.drawString("Play", xMargin, lineSpacing*3);
        
    } else {
        ofSetColor(255, 0, 0);
        StatusFont.drawString("Pause", xMargin, lineSpacing*3);
    }
    
    // draw mute status
    if (isMuted()) {
        ofSetColor(255, 0, 0);
        StatusFont.drawString("Mute", xMargin, lineSpacing*4);
    }
}