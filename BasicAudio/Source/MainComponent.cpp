// Music 256a / CS 476a | fall 2016
// CCRMA, Stanford University
//
// Author: Romain Michon (rmichonATccrmaDOTstanfordDOTedu)
// Description: Simple JUCE sine wave synthesizer

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED
#define Oscs 4

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sine.h"
#include "Smooth.h"

class MainContentComponent :
    public AudioAppComponent,
    private Slider::Listener,
    private ToggleButton::Listener
{
public:
    MainContentComponent() : onOffAll(0), samplingRate(0.0)
    { 
        for (int i = 0; i < Oscs; i++){
        // configuring frequency slider and adding it to the main window
        addAndMakeVisible (frequencySlider[i]);
        frequencySlider[i].setRange (50, 1000);
        frequencySlider[i].setSkewFactorFromMidPoint (500);
        frequencySlider[i].setValue(263-i*0.5); // will also set the default frequency of the sine osc
        frequencySlider[i].addListener (this);
        
        // configuring frequency label box and adding it to the main window
        addAndMakeVisible(frequencyLabel[i]);
        frequencyLabel[i].setText ("oscillator", dontSendNotification);
        frequencyLabel[i].attachToComponent (&frequencySlider[i], true);
        
        // configuring gain slider and adding it to the main window
        addAndMakeVisible (gainSlider[i]);
        gainSlider[i].setRange (0.0, 0.5);
        gainSlider[i].setValue(0.1 + i*(0.025)); // will alsi set the default gain of the sine osc
        gainSlider[i].addListener (this);
       
        // configuring gain label and adding it to the main window
        addAndMakeVisible(gainLabel[i]);
        gainLabel[i].setText ("Gain", dontSendNotification);
        gainLabel[i].attachToComponent (&gainSlider[i], true);
        
        // configuring Enhance button and adding it to the main window
        addAndMakeVisible(enhanceButton[i]);
        enhanceButton[i].addListener(this);

        
        // configuring Enhance label and adding it to the main window
        addAndMakeVisible(enhanceLabel[i]);
        enhanceLabel[i].setText ("Enhance", dontSendNotification);
        enhanceLabel[i].attachToComponent (&enhanceButton[i], true);
        
        }
        
        // configuring on/off button and adding it to the main window
        addAndMakeVisible(onOffButton);
        onOffButton.addListener(this);
        
        // configuring on/off label and adding it to the main window
        addAndMakeVisible(onLable);
        onLable.setText ("On/Off", dontSendNotification);
        onLable.attachToComponent (&onOffButton, true);
        
         // configuring frequency slider and adding it to the main window
        addAndMakeVisible (frequencyAllSlider);
        frequencyAllSlider.setRange (50, 600, 16.25);
        frequencyAllSlider.setSkewFactorFromMidPoint (500);
        frequencyAllSlider.setValue(263); // will also set the default frequency of the sine osc
        frequencyAllSlider.addListener (this);
        
        // configuring frequency label box and adding it to the main window
        addAndMakeVisible(frequencyAllLable);
        frequencyAllLable.setText ("Change Tone", dontSendNotification);
        frequencyAllLable.attachToComponent (&frequencyAllSlider, true);
        
        setSize (600, 500);
        nChans = 1;
        setAudioChannels (0, nChans); // no inputs, one output
    }
    
    ~MainContentComponent()
    {
        shutdownAudio();
    }
    
    void resized() override
    {
        // placing the UI elements in the main window
        // getWidth has to be used in case the window is resized by the user
        const int sliderLeft = 80, sliderRight = 80;
        for (int i = 0; i < Oscs; i++){
            frequencySlider[i].setBounds (sliderLeft, 10 + i*70, getWidth() - sliderLeft - 20, 20);
            gainSlider[i].setBounds (sliderLeft, 35 + i*70, getWidth() - sliderLeft - 20, 20);
            enhanceButton[i].setBounds (sliderLeft, 60 + i*70, getWidth() - sliderLeft - 20, 20);
            frequencyAllSlider.setBounds(sliderRight, 200 + Oscs*30, getWidth() - sliderLeft - 20, 20);
            onOffButton.setBounds(sliderLeft, 300 + Oscs*30, getWidth() - sliderLeft - 20, 20);
        }

    }
    
    void sliderValueChanged (Slider* slider) override
    {
        if (samplingRate > 0.0){
            
            for (int i = 0; i < Oscs; i++){
                    if (slider == &frequencySlider[i]){
                        sine[i].setFrequency(frequencySlider[i].getValue());
                    }
                    else if (slider == &frequencyAllSlider){
                          float freqs = frequencyAllSlider.getValue();
                          frequencySlider[0].setValue(freqs);
                          sine[0].setFrequency(freqs);
                          for (int i = 1; i < Oscs; i++){
                              sine[i].setFrequency(freqs+i*0.5);
                              frequencySlider[i].setValue(freqs+i*0.5);
                          }
                    }
                    else if (slider == &gainSlider[i]){
                        gain[i] = gainSlider[i].getValue();
                    }
                    
                //for (int i = 1; i < Oscs; i++){
                  //      sine[i].setFrequency(frequencySlider[i].getValue())
            }
        }
    }
      
    
    void buttonClicked (Button* button) override
    {
        for (int i = 0; i < Oscs; i++){
        onOff[i]=0;
        }
        // turns audio on or off
        for (int i = 0; i < Oscs; i++){
            if(button ==  &enhanceButton[i]){
                onOff[i] = enhanceButton[i].getToggleState();
            } else if (button ==  &onOffButton){
            onOffAll = onOffButton.getToggleState();
            }
        }
    }
    
    
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        samplingRate = sampleRate;
        for (int i = 0; i < Oscs; i++){
            sine[i].setSamplingRate(sampleRate);
            smooth[i].setSmooth(0.999);
        } 
    }
    
    void releaseResources() override
    {
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // getting the audio output buffer to be filled
        float* const buffer = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        
        // computing one block
        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            if(onOffAll == 1) {
                float temp = 0;
                for (int i = 0; i < Oscs; i++){      
                    temp += sine[i].tick() * smooth[i].tick(gain[i]);
                    if(onOff[i]) {
                        temp += onOff[i]* sine[i].tick() * gain[i];
                    }
                }
                buffer[sample] =temp;
            }
            else buffer[sample] = 0.0;
        }
    }
    
    
private:
    // UI Elements
    Slider frequencySlider[Oscs],gainSlider[Oscs],frequencyAllSlider;
    
    ToggleButton enhanceButton[Oscs],onOffButton;
    
    Label frequencyLabel[Oscs], frequencyAllLable, gainLabel[Oscs], enhanceLabel[Oscs], onLable;
    
    Sine sine[Oscs], sineAll; // the sine wave oscillator
    Smooth smooth[Oscs]; // the smooth function
 
    // Global Variables
    float gain[Oscs], temp;
    int onOff[Oscs], onOffAll, samplingRate, nChans;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
