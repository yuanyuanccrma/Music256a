/*
  ==============================================================================

    This file was auto-generated by the Jucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "include/SineWave.h"
#include "include/ADSR.h"
#include "include/BlitSaw.h"
#include "include/BlitSquare.h"


AudioProcessor* JUCE_CALLTYPE createPluginFilter();

using namespace stk;

//==============================================================================
/** A demo synth sound that's just a basic sine wave.. */
class SineWaveSound : public SynthesiserSound
{
public:
    SineWaveSound() {}

    bool appliesToNote (int /*midiNoteNumber*/) override  { return true; }
    bool appliesToChannel (int /*midiChannel*/) override  { return true; }
};

//==============================================================================
/** A simple demo synth voice that just plays a sine wave.. */
class SineWaveVoice  : public SynthesiserVoice
{
public:
    SineWaveVoice()
    :  carrierFrequency(440.0),
    index(0.0),
    level(0.0),
    envelope(0.0),
    onOff (false),
    tailOff(true){
        Stk::setSampleRate(getSampleRate());
    };
    
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<SineWaveSound*> (sound) != nullptr;
    }

    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound* /*sound*/,
                    int /*currentPitchWheelPosition*/) override
    {
        carrierFrequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        
        // we don't want an ugly sweep when the note begins...
        smooth[0].setSmooth(0);
        smooth[0].tick(carrierFrequency);
        
        // standard smoothing...
        for(int i=0; i<2; i++){
            smooth[i].setSmooth(0.999);
        }
        
        level = velocity;
        // level = pow(velocity,2); // if we want linear dynamic
        
        // tells the note to begin!
        onOff = true;
        
        // These parameters could be controlled with UI elements and could
        // be assigned to specific MIDI controllers. If you do so,
        // don't forget to smooth them!
        modulator.setFrequency(1.4 * carrierFrequency);
        index = 150;
        adsr.keyOn();
    }
    
    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        onOff = false; // end the note
        level = 0; // ramp envelope to 0 if tail off is allowed
        
        tailOff = allowTailOff;
        adsr.keyOff();
    }

    void pitchWheelMoved (int /*newValue*/) override
    {
        // can't be bothered implementing this for the demo!
    }

    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override
    {
        // not interested in controllers in this case.
    }
    
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        if(envelope != 0 || onOff){
            while (--numSamples >= 0){
                adsr.setAllTimes(0.2, 1.9, 0.1, 0.1);
                envelope = smooth[1].tick(level); // here we use a smoother as an envelope generator
                carrier.setFrequency(smooth[0].tick(carrierFrequency) + modulator.tick()*index);
                const float currentSample = (float) carrier.tick() * envelope * adsr.tick();
                for (int i = outputBuffer.getNumChannels(); --i >= 0;){
                    outputBuffer.addSample (i, startSample, currentSample);
                }
                ++startSample;
                
                // if tail off is disabled, we end the note right away, otherwise, we wait for envelope
                // to reach a safe value
                if(!onOff && (envelope < 0.001 || !tailOff)){
                    envelope = 0;
                    clearCurrentNote();
                }
            }
        }
    }
private:
    SineWave carrier, modulator;
    Smooth smooth[2];
    ADSR adsr;
    double carrierFrequency, index, level, envelope;
    bool onOff, tailOff;
};
///////////////////////////////////////////////
class SquareWaveSound : public SynthesiserSound
{
public:
    SquareWaveSound() {}
    
    bool appliesToNote (int /*midiNoteNumber*/) override  { return true; }
    bool appliesToChannel (int /*midiChannel*/) override  { return true; }
};

//==============================================================================
class SquareWaveVoice  : public SynthesiserVoice
{
public:
    SquareWaveVoice()
    :  carrierFrequency(440.0),
    index(0.0),
    level(0.0),
    envelope(0.0),
    onOff (false),
    tailOff(true){
        Stk::setSampleRate(getSampleRate());
    };
    
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<SquareWaveSound*> (sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound* /*sound*/,
                    int /*currentPitchWheelPosition*/) override
    {
        carrierFrequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        
        smooth[0].setSmooth(0);
        smooth[0].tick(carrierFrequency);
        
        for(int i=0; i<2; i++){
            smooth[i].setSmooth(0.999);
        }
        
        level = velocity;
        onOff = true;
        modulator.setFrequency(1.4 * carrierFrequency);
        index = 150;
        adsr.keyOn();
    }
    
    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        onOff = false; // end the note
        level = 0; // ramp envelope to 0 if tail off is allowed
        
        tailOff = allowTailOff;
        adsr.keyOff();
    }
    
    void pitchWheelMoved (int /*newValue*/) override
    {
        // can't be bothered implementing this for the demo!
    }
    
    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override
    {
        // not interested in controllers in this case.
    }
    
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        if(envelope != 0 || onOff){
            while (--numSamples >= 0){
                adsr.setAllTimes(0.2, 0.9, 0.1, 0.4);
                envelope = smooth[1].tick(level); // here we use a smoother as an envelope generator
                carrier.setFrequency(smooth[0].tick(carrierFrequency) + modulator.tick()*index);
                const float currentSample = (float) carrier.tick() * envelope * adsr.tick();
                for (int i = outputBuffer.getNumChannels(); --i >= 0;){
                    outputBuffer.addSample (i, startSample, currentSample);
                }
                ++startSample;
                if(!onOff && (envelope < 0.001 || !tailOff)){
                    envelope = 0;
                    clearCurrentNote();
                }
            }
        }
    }
private:
    BlitSquare carrier, modulator;
    Smooth smooth[2];
    ADSR adsr;
    double carrierFrequency, index, level, envelope;
    bool onOff, tailOff;
};

///++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class SawWaveSound : public SynthesiserSound
{
public:
    SawWaveSound() {}
    
    bool appliesToNote (int /*midiNoteNumber*/) override  { return true; }
    bool appliesToChannel (int /*midiChannel*/) override  { return true; }
};

//==============================================================================
/** A simple demo synth voice that just plays a sine wave.. */
class SawWaveVoice  : public SynthesiserVoice
{
public:
    SawWaveVoice()
    :  carrierFrequency(440.0),
    index(0.0),
    level(0.0),
    envelope(0.0),
    onOff (false),
    tailOff(true){
        Stk::setSampleRate(getSampleRate());
    };
    
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<SawWaveSound*> (sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound* /*sound*/,
                    int /*currentPitchWheelPosition*/) override
    {
        carrierFrequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        smooth[0].setSmooth(0);
        smooth[0].tick(carrierFrequency);
        
        // standard smoothing...
        for(int i=0; i<2; i++){
            smooth[i].setSmooth(0.999);
        }
        
        level = velocity;

        onOff = true;
        modulator.setFrequency(4.4 * carrierFrequency);
        index = 150;
        adsr.keyOn();
    }
    
    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        onOff = false; // end the note
        level = 0; // ramp envelope to 0 if tail off is allowed
        
        tailOff = allowTailOff;
        adsr.keyOff();
    }
    
    void pitchWheelMoved (int /*newValue*/) override
    {
        // can't be bothered implementing this for the demo!
    }
    
    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override
    {
        // not interested in controllers in this case.
    }
    
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        if(envelope != 0 || onOff){
            while (--numSamples >= 0){
                adsr.setAllTimes(0.2, 0.8, 0.1, 0.6);
                envelope = 0.8*smooth[1].tick(level); // here we use a smoother as an envelope generator
                carrier.setFrequency(smooth[0].tick(carrierFrequency) + modulator.tick()*index);
                const float currentSample = (float) carrier.tick() * envelope * adsr.tick();
                for (int i = outputBuffer.getNumChannels(); --i >= 0;){
                    outputBuffer.addSample (i, startSample, currentSample);
                }
                ++startSample;
                if(!onOff && (envelope < 0.001 || !tailOff)){
                    envelope = 0;
                    clearCurrentNote();
                }
            }
        }
    }
    
private:
    BlitSaw carrier, modulator;
    Smooth smooth[2];
    ADSR adsr;
    double carrierFrequency, index, level, envelope;
    bool onOff, tailOff;
};

//==============================================================================
JuceDemoPluginAudioProcessor::JuceDemoPluginAudioProcessor()
    : lastUIWidth (400),
      lastUIHeight (200),
      gainParam (nullptr),
      delayParam (nullptr),
      switchParam (nullptr),
      delayPosition (0)
{
    lastPosInfo.resetToDefault();

    // This creates our parameters. We'll keep some raw pointers to them in this class,
    // so that we can easily access them later, but the base class will take care of
    // deleting them for us.
    addParameter (gainParam  = new AudioParameterFloat ("gain",  "Gain",           0.0f, 1.0f, 0.9f));
    addParameter (delayParam = new AudioParameterFloat ("delay", "Delay Feedback", 0.0f, 1.0f, 0.5f));
    addParameter (switchParam = new AudioParameterFloat ("switch", "switch Feedback", 0.0f, 1.0f, 0.0f));
    initialiseSynth();
}

JuceDemoPluginAudioProcessor::~JuceDemoPluginAudioProcessor()
{
}

void JuceDemoPluginAudioProcessor::initialiseSynth()
{
    const int numVoices = 8;
    for (int i = numVoices; --i >= 0;)
        synth.addVoice (new SineWaveVoice());
    synth.addSound (new SineWaveSound());
    synth1.addVoice (new SquareWaveVoice());
    synth1.addSound (new SquareWaveSound());
    synth2.addVoice (new SawWaveVoice());
    synth2.addSound (new SawWaveSound());

}

//==============================================================================
void JuceDemoPluginAudioProcessor::prepareToPlay (double newSampleRate, int /*samplesPerBlock*/)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate (newSampleRate);
    
    synth1.setCurrentPlaybackSampleRate (newSampleRate);
    
    synth2.setCurrentPlaybackSampleRate (newSampleRate);
    
    
    keyboardState.reset();

    if (isUsingDoublePrecision())
    {
        delayBufferDouble.setSize (2, 12000);
        delayBufferFloat.setSize (1, 1);
    }
    else
    {
        delayBufferFloat.setSize (2, 12000);
        delayBufferDouble.setSize (1, 1);
    }
    reset();
}

void JuceDemoPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();
}

void JuceDemoPluginAudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.
    delayBufferFloat.clear();
    delayBufferDouble.clear();
}

template <typename FloatType>

void JuceDemoPluginAudioProcessor::process (AudioBuffer<FloatType>& buffer,
                                            MidiBuffer& midiMessages,
                                            AudioBuffer<FloatType>& delayBuffer)
{
    const int numSamples = buffer.getNumSamples();

    // apply our gain-change to the incoming data..
    applyGain (buffer, delayBuffer);

    // Now pass any incoming midi messages to our keyboard state object, and let it
    // add messages to the buffer if the user is clicking on the on-screen keys
    
    keyboardState.processNextMidiBuffer (midiMessages, 0, numSamples, true);

    // and now get our synth to process these midi events and generate its output.
    if(switchParam->get() == 0){
        synth.renderNextBlock (buffer, midiMessages, 0, numSamples);
    }
    if(switchParam->get() == 0.5){
        synth1.renderNextBlock (buffer, midiMessages, 0, numSamples);
    }
    else {
        synth2.renderNextBlock (buffer, midiMessages, 0, numSamples);
    }

    // Apply our delay effect to the new output..
    applyDelay (buffer, delayBuffer);

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, numSamples);
    // Now ask the host for the current time so we can store it to be displayed later...
    updateCurrentTimeInfoFromHost();
}

template <typename FloatType>
void JuceDemoPluginAudioProcessor::applyGain (AudioBuffer<FloatType>& buffer, AudioBuffer<FloatType>& delayBuffer)
{
    ignoreUnused (delayBuffer);
    const float gainLevel = *gainParam;
    for (int channel = 0; channel < getTotalNumInputChannels(); ++channel)
        buffer.applyGain (channel, 0, buffer.getNumSamples(), gainLevel);
}



template <typename FloatType>
void JuceDemoPluginAudioProcessor::applyDelay (AudioBuffer<FloatType>& buffer, AudioBuffer<FloatType>& delayBuffer)
{
    const int numSamples = buffer.getNumSamples();
    
    const float delayLevel = *delayParam;

    int delayPos = 0;

    for (int channel = 0; channel < getTotalNumInputChannels(); ++channel)
    {
        FloatType* const channelData = buffer.getWritePointer (channel);
        FloatType* const delayData = delayBuffer.getWritePointer (jmin (channel, delayBuffer.getNumChannels() - 1));
        delayPos = delayPosition;

        for (int i = 0; i < numSamples; ++i)
        {
            const FloatType in = channelData[i];
            channelData[i] += delayData[delayPos];
            delayData[delayPos] = (delayData[delayPos] + in) * delayLevel;

            if (++delayPos >= delayBuffer.getNumSamples())
                delayPos = 0;
        }
    }
    delayPosition = delayPos;
}

void JuceDemoPluginAudioProcessor::updateCurrentTimeInfoFromHost()
{
    if (AudioPlayHead* ph = getPlayHead())
    {
        AudioPlayHead::CurrentPositionInfo newTime;

        if (ph->getCurrentPosition (newTime))
        {
            lastPosInfo = newTime;  // Successfully got the current time from the host..
            return;
        }
    }

    // If the host fails to provide the current time, we'll just reset our copy to a default..
    lastPosInfo.resetToDefault();
}

//==============================================================================
AudioProcessorEditor* JuceDemoPluginAudioProcessor::createEditor()
{
    return new JuceDemoPluginAudioProcessorEditor (*this);
}

//==============================================================================
void JuceDemoPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // Here's an example of how you can use XML to make it easy and more robust:

    // Create an outer XML element..
    XmlElement xml ("MYPLUGINSETTINGS");

    // add some attributes to it..
    xml.setAttribute ("uiWidth", lastUIWidth);
    xml.setAttribute ("uiHeight", lastUIHeight);

    // Store the values of all our parameters, using their param ID as the XML attribute
    for (int i = 0; i < getNumParameters(); ++i)
        if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
            xml.setAttribute (p->paramID, p->getValue());

    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml, destData);
}

void JuceDemoPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr)
    {
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our last window size..
            lastUIWidth  = jmax (xmlState->getIntAttribute ("uiWidth", lastUIWidth), 400);
            lastUIHeight = jmax (xmlState->getIntAttribute ("uiHeight", lastUIHeight), 200);

            // Now reload our parameters..
            for (int i = 0; i < getNumParameters(); ++i)
                if (AudioProcessorParameterWithID* p = dynamic_cast<AudioProcessorParameterWithID*> (getParameters().getUnchecked(i)))
                    p->setValueNotifyingHost ((float) xmlState->getDoubleAttribute (p->paramID, p->getValue()));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceDemoPluginAudioProcessor();
}
