# Approximating Pi

Approximating pi is a multichannel audio-visual piece written by Clarence Barlow. This repo is an implementation of this piece for a multichannel playback of arbitrary length and up to 16 channels.

The current implementation is written for Openframeworks on OSX10.11. 

Usage
=====
Clone this repo into the `src` directory of a fresh openframeworks project. Make sure the openframeworks project has the following addons. 
 * [ofxDatGui](https://github.com/braitsch/ofxDatGui)
 * ofxOSC

Add the follwing assets to the `bin/data` folder:
 * [ofxbraitsch](https://github.com/braitsch/ofxDatGui/tree/master/ofxbraitsch)
 * [pi.png](https://drive.google.com/file/d/0B7ReoGDIXK3RdnVFeHZzR2lhYjg/view?usp=sharing)
 * [Futura-Medium.ttf](https://drive.google.com/file/d/0B7ReoGDIXK3RMFIxNFZtZjVLRHM/view?usp=sharing)
 
Modify the `main.cpp` file to this:
```
#include "ofMain.h"
#include "ApproximatingPi/ApproxPi_SingleWindow.h"

int main( ){
    ofSetupOpenGL(ApproxPi_SingleWindow::defaultWindowWidth(), ApproxPi_SingleWindow::defaultWindowHeight(), OF_WINDOW);
    ApproxPi_SingleWindow* app = new ApproxPi_SingleWindow();
    ofRunApp(app);
}
```

 Note: This app also requires that the assets are bundled within the `Contents/Resources/data` folder of the built app. You can either move it manually, or use a script to do this automatically on every build.

In XCode, you would typically add this line to the end of the run script within the `Build Phases` tab to achieve this.
```
cp -r bin/data "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/Resources";
``` 

------------
##### Authors:   
-Akshay Cadambi (akshay19.92 at g-mail dot com)   
-Matthias Wagner (Github: matzewagner)  
**For:** Clarence Barlow.  
**Last performed:** March 28 2016 at SBCAST, Santa Barbara (6 Channel, 2 hour version)
