#pragma once

#include "PiSettings.h"

#include "ofxDatGui.h"
#include <string>
#include <vector>

class ofApproxPiSetupApp : public ofBaseApp {
    
public:
    ofApproxPiSetupApp(PiSettings& Settings) {settings = &Settings;}
    void setup();
    void update();
    void draw();
    void exit();
    int getSelectedIndex(std::string dropDownName);
    int getSelectedAudioDeviceID(void);
    
    PiSettings* settings;

    std::vector<int> nChannels_options_int = {1, 2, 4, 6, 8};
    std::vector<std::string> nChannels_options_str = {"1", "2", "4", "6", "8"};
    
    std::vector<int> widths_options_int = {1280};
    std::vector<int> heights_options_int = {1024};
    std::vector<std::string> screen_resolutions_options_str = {"1280x1024"};
    
    std::vector<int> app_type_options_int = {1, 2};
    std::vector<std::string> app_type_options_str = {"Single Window", "Multi Window"};
    
    std::vector<ofSoundDevice> audio_devices;
    std::vector<std::string> audio_device_options_str;
    std::vector<int> audio_device_options_int;
    
    void onDropDown(ofxDatGuiDropdownEvent e);
    void onTextInput(ofxDatGuiTextInputEvent e);
    void onButton(ofxDatGuiButtonEvent e);

    ofxDatGui* gui;
    
    std::string spacer = ":        ";

    static int getWindowWidth(void) {return 500;}
    static int getWindowHeight(void) {return 500;}
};