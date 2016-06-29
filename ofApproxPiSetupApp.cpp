#include "ofApproxPiSetupApp.h"

//--------------------------------------------------------------
void ofApproxPiSetupApp::setup(){
    ofSetWindowPosition(0, 0);
    ofSetWindowShape(500,500);
    
    int x=20, y=20;
    
    gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
    gui->setWidth(400);
    gui->setPosition(x, y);
    gui->addLabel("Approx Pi Setup");
    
    gui->addDropdown("NChannels", nChannels_options_str);
    gui->getDropdown("NChannels")->setLabel("NChannels"+spacer+nChannels_options_str[0]);
        
    gui->addDropdown("Resolution", screen_resolutions_options_str);
    gui->getDropdown("Resolution")->setLabel("Resolution"+spacer+screen_resolutions_options_str[0]);

    
    audio_devices = ofSoundStreamListDevices();
    for(int i=0; i<audio_devices.size(); i++)
    {
        if(audio_devices[i].outputChannels >0)
            output_devices.push_back(audio_devices[i]);
    }
    int default_device_index=-1;
    for(int i=0; i<audio_devices.size(); i++)
    {
        if(audio_devices[i].outputChannels != 0){
            audio_device_options_str.push_back(audio_devices[i].name);
            audio_device_options_int.push_back(audio_devices[i].deviceID);
            
            if(audio_devices[i].isDefaultOutput)
                default_device_index = i;
        }
    }
    gui->addDropdown("Audio Device", audio_device_options_str);
    gui->getDropdown("Audio Device")->setLabel("Audio Device"+spacer+audio_device_options_str[0]);

    gui->addTextInput("Duration (in mins)");
    
    gui->addButton("Save and Run");
    gui->addButton("Abort");
    
    gui->onDropdownEvent(this, &ofApproxPiSetupApp::onDropDown);
    gui->onButtonEvent(this, &ofApproxPiSetupApp::onButton);
    gui->onTextInputEvent(this, &ofApproxPiSetupApp::onTextInput);
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
    }
    else
    {
        settings->exit = false;
        
        if(settings->dur_in_mins == -1)
        {
            gui->getTextInput("Duration (in mins)")->setText("*ENTER DURATION*");
            return;
        }
        
        int selected_channels = nChannels_options_int[getSelectedIndex("NChannels")];
        int available_channels = output_devices[getSelectedIndex("Audio Device")].outputChannels;
        if(selected_channels > available_channels)
        {
            gui->addLabel("***INSUFFICIENT CHANNELS ON OUTPUT DEVICE***");
            return;
        }
    }

    gui->setVisible(false);
    exit();
}

//--------------------------------------------------------------
void ofApproxPiSetupApp::exit(void)
{
    settings->nChannels = nChannels_options_int[getSelectedIndex("NChannels")];
    settings->windowWidth = widths_options_int[getSelectedIndex("Resolution")];
    settings->windowHeight = heights_options_int[getSelectedIndex("Resolution")];
    settings->audioDeviceID = audio_device_options_int[getSelectedIndex("Audio Device")];
    
    settings->doneSetup = true;
}

//--------------------------------------------------------------
int ofApproxPiSetupApp::getSelectedIndex(std::string dropDownName)
{
    std::string label = (gui->getDropdown(dropDownName)->getSelected()->getLabel());
    int index = -1;
    for(int i=0; i<gui->getDropdown(dropDownName)->children.size(); i++)
    {
        string childLabel = gui->getDropdown(dropDownName)->children[i]->getLabel();
        
        if((gui->getDropdown(dropDownName)->children[i]->getLabel() == label))
        {
            index = i;
            break;
        }
    }
    return index;
}