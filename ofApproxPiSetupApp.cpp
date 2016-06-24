#include "ofApproxPiSetupApp.h"

//--------------------------------------------------------------
void ofApproxPiSetupApp::setup(){
    ofSetWindowPosition(0, 0);
    
    int x=20, y=20;
    
    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    gui->setWidth(400);
    gui->setPosition(x, y);
    gui->addLabel("Approx Pi Setup");
    
    gui->addDropdown("NChannels", nChannels_options_str);
    gui->getDropdown("NChannels")->select(0);
    gui->getDropdown("NChannels")->setLabel("NChannels"+spacer+"1");
    
    gui->addDropdown("App Type", app_type_options_str);
    gui->getDropdown("App Type")->select(0);
    gui->getDropdown("App Type")->setLabel("App Type"+spacer+"Single Window");
   
    gui->addDropdown("Resolution", screen_resolutions_options_str);
    gui->getDropdown("Resolution")->select(0);
    gui->getDropdown("Resolution")->setLabel("Resolution"+spacer+"1280x1024");
    
    audio_devices = ofSoundStreamListDevices();
    int default_device_index=-1;
    for(int i=0; i<audio_devices.size(); i++)
    {
        audio_device_options_str.push_back(audio_devices[i].name);
        audio_device_options_int.push_back(audio_devices[i].deviceID);
        if(audio_devices[i].isDefaultOutput)
            default_device_index = i;
    }
    gui->addDropdown("Audio Device", audio_device_options_str);
    gui->getDropdown("Audio Device")->select(default_device_index);
    gui->getDropdown("Audio Device")->setLabel("Audio Device"+spacer+audio_devices[default_device_index].name);

    gui->addTextInput("Duration (in mins)");
    
    gui->addButton("Save and Run");
    gui->addButton("Abort");
    
    gui->onDropdownEvent(this, &ofApproxPiSetupApp::onDropDown);
    gui->onButtonEvent(this, &ofApproxPiSetupApp::onButton);
    gui->onTextInputEvent(this, &ofApproxPiSetupApp::onTextInput);
    
}

//--------------------------------------------------------------
void ofApproxPiSetupApp::update(){
}

//--------------------------------------------------------------
void ofApproxPiSetupApp::draw(){
}

//--------------------------------------------------------------

void ofApproxPiSetupApp::onDropDown(ofxDatGuiDropdownEvent e)
{
    e.target->setLabel(e.target->getName() + spacer +e.target->getSelected()->getLabel());
}

//--------------------------------------------------------------
void ofApproxPiSetupApp::onTextInput(ofxDatGuiTextInputEvent e)
{
    try
    {
        settings->dur_in_mins = std::stof(e.target->getText());
        e.target->setText(std::to_string(settings->dur_in_mins));
    }
    catch(...)
    {
        e.target->setText("Invalid Entry! Try Again");
    }
}

//--------------------------------------------------------------

void ofApproxPiSetupApp::onButton(ofxDatGuiButtonEvent e)
{
    if(e.target->getName() == "Abort")
    {
        settings->exit = true;
        ofExit();
    }
    
    if(settings->dur_in_mins != -1)
    {
        settings->exit = false;
        ofExit();
    }
    else
    {
        gui->getTextInput("Duration (in mins)")->setText("*ENTER DURATION*");
    }
    
}

//--------------------------------------------------------------
void ofApproxPiSetupApp::exit(void)
{
    settings->nChannels = nChannels_options_int[getSelectedIndex("NChannels")];
    settings->appType = app_type_options_int[getSelectedIndex("App Type")];
    settings->windowWidth = widths_options_int[getSelectedIndex("Resolution")];
    settings->windowHeight = heights_options_int[getSelectedIndex("Resolution")];
    settings->audioDeviceID = audio_device_options_int[getSelectedIndex("Audio Device")];
}

//--------------------------------------------------------------
int ofApproxPiSetupApp::getSelectedIndex(std::string dropDownName)
{
    std::string label = (gui->getDropdown(dropDownName)->getSelected()->getLabel());
    int index = -1;
    for(int i=0; i<gui->getDropdown(dropDownName)->children.size(); i++)
    {
        if( label.find(gui->getDropdown(dropDownName)->getChildAt(i)->getLabel()) )
            index = i;
        else if (label == gui->getDropdown(dropDownName)->getChildAt(i)->getLabel())
            index = i;
    }
    return index;
}

