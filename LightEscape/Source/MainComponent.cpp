// Music 256a / CS 476a | fall 2016
// CCRMA, Stanford University
//
// Author: Romain Michon (rmichonATccrmaDOTstanfordDOTedu)
// Description: Simple synth demonstrating how to use the STK with JUCE.

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "stk/SineWave.h" // STK sine osc
#include "stk/BlitSaw.h" // STK sawtooth osc
#include "stk/BlitSquare.h" // STK square osc
#include "ADSR.h"
#include "Smooth.h"


using namespace stk; // required for the STK to work

class MainContentComponent :
public AudioAppComponent,
private Slider::Listener,
private ToggleButton::Listener
{
public:
    
    MainContentComponent()
    :   currentSampleRate (0.0),
    sineOnOff(0.0),
    sawOnOff(0.0),
    squareOnOff(0.0)
    
    {
        addAndMakeVisible (frequencySlider);
        frequencySlider.setRange (50.0, 5000.0);
        frequencySlider.setSkewFactorFromMidPoint (1000.0);
        frequencySlider.setValue(1000);
        frequencySlider.setSliderStyle(Slider::LinearBar);
        frequencySlider.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
        frequencySlider.addListener (this);
        
        addAndMakeVisible(frequencyLabel);
        frequencyLabel.setText ("Frequency", dontSendNotification);
        frequencyLabel.attachToComponent (&frequencySlider, true);
        
        addAndMakeVisible (gainSlider1);
        gainSlider1.setRange (0.0, 0.5);
        gainSlider1.setValue(0.25);
        gainSlider1.setSliderStyle(Slider::Rotary);
        gainSlider1.setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
        gainSlider1.addListener (this);

        addAndMakeVisible (gainSlider2);
        gainSlider2.setRange (0.0, 0.3);
        gainSlider2.setValue(0);
        gainSlider2.setSliderStyle(Slider::Rotary);
        gainSlider2.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
        gainSlider2.addListener (this);
        
        addAndMakeVisible (gainSlider3);
        gainSlider3.setRange (0.0, 0.3);
        gainSlider3.setValue(0);
        gainSlider3.setSliderStyle(Slider::Rotary);
        gainSlider3.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
        gainSlider3.addListener (this);
        
        
        
        addAndMakeVisible(sineButton);
        sineButton.addListener(this);
        
        addAndMakeVisible(sawButton);
        sawButton.addListener(this);
        
        addAndMakeVisible(squareButton);
        squareButton.addListener(this);
        
        addAndMakeVisible(sineLabel);
        sineLabel.setText ("Sine", dontSendNotification);
        sineLabel.attachToComponent (&sineButton, true);
        
        addAndMakeVisible(sawLabel);
        sawLabel.setText ("Saw", dontSendNotification);
        sawLabel.attachToComponent (&sawButton, true);
        
        addAndMakeVisible(squareLabel);
        squareLabel.setText ("Square", dontSendNotification);
        squareLabel.attachToComponent (&squareButton, true);
        
  
        
        setSize (600, 600);
        setAudioChannels (0, 2); // no inputs, one output
    }
    
    ~MainContentComponent()
    {
        shutdownAudio();
    }
    
    float ww = 100;
    float hh = 150;
    
    void paint(Graphics& g) override  // Paint the background and draw the play area
    {
        
       
       
        ColourGradient cg(Colours::lightcoral, xpaint + (400 / 6), xpaint + (400 / 4),
                                 Colours::mediumpurple, getRight(), 200 + (400 / 5), true);
        
        g.setGradientFill(cg);
        g.fillAll();
    
        
        
        float w = 400 / 18.0f;
        float h = 400 / 18.0f;
        g.setColour (Colours::lightyellow);
        for (int i = 0; i < 18; i++)  //draw the play area
        {
            g.drawLine(i * w + ww, hh, i * w + ww, (float)400+hh);
            g.drawLine(0.0f+ww, i * h+hh, (float)400+ww, i * h+hh);
            
        }
        
        g.setColour(Colours::burlywood);
        g.drawRect(100, 150, 400, 400, 4);
        
    }
    
    void resized() override
    {
        const int componentsHeight = 20;
        frequencySlider.setBounds (80, 10, getWidth() - 100, componentsHeight);
        gainSlider1.setBounds (100, 40, 80, 80);
        gainSlider2.setBounds (250, 40, 80, 80);
        gainSlider3.setBounds (400, 40, 80, 80);
        sineButton.setBounds (130, 120, 30, componentsHeight);
        sawButton.setBounds (280, 120, 30, componentsHeight);
        squareButton.setBounds (430, 120, 30, componentsHeight);

        
    }
//    
//     void mouseDown	(const MouseEvent & event) override{
//         std::cout<<"down"<<std::endl;
//       if (event.getScreenX() > 0) sineOnOff = 1.0;
 //   }
    
    float basefreq = 200;
    void mouseDrag (const MouseEvent & event) override
    {
        
        if ( 450 < event.getScreenX() && event.getScreenX() < 850 && 250<event.getScreenY() && event.getScreenY()< 650)
        {
            modulatorFrequency = event.getScreenX()*10+basefreq;
            index = event.getScreenY() / 16 + 50;
            xpaint= event.getScreenX()/2;
            ypaint =event.getScreenY() / 2;
            repaint();
            
        }
        else {
            modulatorFrequency = 0;
            index = 0;
            xpaint = 200;
            ypaint = 200;
            
        }
        std::cout<< index <<std::endl;
    }

   void mouseDown(const MouseEvent& event) override
    {
        if ( 450 < event.getScreenX() && event.getScreenX()< 850 && 250<event.getScreenY() && event.getScreenY()< 650)
        {
            modulatorFrequency = event.getScreenX()*10+basefreq;
            index = getScreenY() / 4;
            xpaint= event.getScreenX()/2;
            ypaint =event.getScreenY() / 2;
            adsr.keyOn();
        }
        else {
            adsr.keyOff();
            modulatorFrequency = 0;
            index = 0;
            xpaint = 200;
            ypaint = 200;
        }
        
    }

  void mouseUp(const MouseEvent& event) override
 {
     if ( 450 < event.getScreenX() && event.getScreenX() < 850&& 250<event.getScreenY() && event.getScreenY()< 650)
     {
         modulatorFrequency = event.getScreenX()*10+basefreq;
         xpaint= event.getScreenX()/2;
         ypaint =event.getScreenY() / 2;
     }
     else {
         modulatorFrequency = 0;
         xpaint = 200;
         ypaint = 200;
     }
 }

    
 
    
    void sliderValueChanged (Slider* slider) override
    {
        if (slider == &frequencySlider)
        {
            if (currentSampleRate > 0.0){
                carrierFrequency = frequencySlider.getValue();
                
                sine.setFrequency(carrierFrequency);
                saw.setFrequency(carrierFrequency);
                square.setFrequency(carrierFrequency);
            }
        }
        else if (slider == &gainSlider1)
        {
            if (currentSampleRate > 0.0){
                gain[0] = gainSlider1.getValue();
            }
        }
        else if (slider == &gainSlider2)
        {
            if (currentSampleRate > 0.0){
                gain[1] = gainSlider2.getValue();
            }
        }
        else if (slider == &gainSlider3)
        {
            if (currentSampleRate > 0.0){
                gain[2] = gainSlider3.getValue();
            }
        }
    }
    
    void buttonClicked (Button* button) override
    {
        if(button == &sineButton ){
            sineOnOff = sineButton.getToggleState();
        }
        if(button == &sawButton){
            sawOnOff =  sawButton.getToggleState();
        }
        if(button == &squareButton ){
            squareOnOff = squareButton.getToggleState();
        }
        if(sineOnOff||sawOnOff ||squareOnOff){
            carrierAdsr.keyOn();
        }
        else {
            carrierAdsr.keyOff();}
    }
    
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        currentSampleRate = sampleRate;
        Stk::setSampleRate(sampleRate); // STK must know the sampling rate
        
        // setting default values for STK parameters
        sine.setFrequency(carrierFrequency);
        saw.setFrequency(carrierFrequency);
        square.setFrequency(carrierFrequency);
        
        
        modulatorSine.setFrequency(50);
        modulatorSaw.setFrequency(50);
        modulatorSquare.setFrequency(50);
        
        adsr.setAllTimes(0.1, 0.1, 0.3, 0.9);
        carrierAdsr.setAllTimes(0.4, 0.1, 0.3, 0.9);
        for (int i = 0; i < 4; i++){
        smooth[i].setSmooth(0.999);
        }
    }
    
    void releaseResources() override
    {
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        float* const buffer = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        
        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            float sineSample = 0.0;
            float sawSample = 0.0;
            float squareSample = 0.0;
            
            if (sineOnOff||sawOnOff ||squareOnOff){
                sine.setFrequency(smooth[0].tick(carrierFrequency) + modulatorSine.tick()*smooth[3].tick(index)*adsr.tick());
                sineSample = sine.tick()* smooth[1].tick(gain[0])*sineOnOff; // computing next sine wave sample
           
                saw.setFrequency(smooth[0].tick(carrierFrequency) + modulatorSaw.tick()*smooth[3].tick(index)*adsr.tick());
                sawSample = saw.tick() * smooth[1].tick(gain[1])*sawOnOff;// computing next sawtooth sample
            
                square.setFrequency(smooth[0].tick(carrierFrequency) + modulatorSquare.tick()*smooth[3].tick(index)*adsr.tick());
                squareSample = square.tick()* smooth[1].tick(gain[2])*squareOnOff; // computing next sawtooth sample
              }
              buffer[sample] = sineSample*smooth[2].tick(sineOnOff)+sawSample*smooth[2].tick(sawOnOff)+squareSample*smooth[2].tick(squareOnOff);
              buffer[sample] *= carrierAdsr.tick();
              // filling output buffer
        }
    }
    
    
private:
    Slider frequencySlider ,aKnob, dKnob, sKnob, rKnob;
    Slider gainSlider1, gainSlider2, gainSlider3;
    ToggleButton sineButton, sawButton, squareButton;
    ADSR adsr, carrierAdsr;
    
    
    Label frequencyLabel, Attack, Decay, Sustain, Release;
    Label gainLabel;
    Label sineLabel, sawLabel, squareLabel;
    
    Smooth smooth[4];
    SineWave sine, modulatorSine; // sine osc
    BlitSaw saw, modulatorSaw;// sawtooth osc
    BlitSquare square, modulatorSquare;
 
    double currentSampleRate, gain[3], sineOnOff, sawOnOff, squareOnOff, carrierFrequency, modulatorFrequency, index, xpaint, ypaint; // [1]
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
