#include "ofPiApproximator.h"

ofPiApproximator::ofPiApproximator(PiSettings& settings)
{
    settings_ref = &settings;
    
    fontSize = min(settings_ref->windowWidth, settings_ref->windowHeight)*0.25;
    NumberFont.load("Futura-Medium.ttf", fontSize);
    StatusFont.load("Futura-Medium.ttf", fontSize*0.25);
    piSymbol.load("pi.png");
    verticalFontMargin = fontSize*0.15;
    ofBackground(0,0,0);
};

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

void ofPiApproximator::scaleContent(int w, int h) {
    float horizontalWindowScaleFactor = w / float(fontSize*6.0);
    float verticalWindowScaleFactor = h / float(fontSize*3.8);
    ofScale(horizontalWindowScaleFactor, verticalWindowScaleFactor);
}

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
    float seconds = sampleCounter / float(settings_ref->sample_rate);
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