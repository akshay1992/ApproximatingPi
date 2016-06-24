#ifndef PiSettings_h
#define PiSettings_h

#include <iostream>

class PiSettings {
public:
    int nChannels;
    int appType;    // 0 = single window; 1 = multi-window
    int windowWidth, windowHeight;
    int audioDeviceID;
    int sample_rate;
    float dur_in_mins = -1;
    bool exit; // If the user cancels running approx pi
    
    void print(void)
    {
        std::cout << "Settings" << std::endl;
        std::cout << "NChannels: " << nChannels << std::endl;
        std::cout << "App Type: " << (appType ? "Single Window" : "Multi Window") << std::endl;
        std::cout << "Resolution: " << windowWidth << "x" << windowHeight << std::endl;
        std::cout << "audioDeviceID: " << audioDeviceID << std::endl;
        std::cout << "Duration: " << dur_in_mins << " minutes"<< std::endl;
        std::cout << "Aborted: " << (exit ? "True" : "False" ) << std::endl;
    }
};

#endif 
