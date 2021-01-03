/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define MAX_MID_MSG_LOG_BUFFER_SIZE 30

//==============================================================================
/**
*/
class MidiTestPluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MidiTestPluginAudioProcessor();
    ~MidiTestPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState parameters;
    
    std::atomic<float>* scaledNoteVelocityParameter;
    
    void AddMidiMsgToLog(juce::String& msg);
    void ReadAndFlushMidiMsgLog(juce::Array<juce::String>& msgBufferToCopyTo);

private:
    
    juce::Array<juce::String> midiMsgLogBuffer;
    
    juce::CriticalSection midiMsgLock;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiTestPluginAudioProcessor)
};
