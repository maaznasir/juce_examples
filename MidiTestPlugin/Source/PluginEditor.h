/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class MidiTestPluginAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                            private juce::Slider::Listener,
                                            private juce::Timer
{
public:
    MidiTestPluginAudioProcessorEditor (MidiTestPluginAudioProcessor&);
    ~MidiTestPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void timerCallback() override;

private:
    void sliderValueChanged (juce::Slider* slider) override;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiTestPluginAudioProcessor& audioProcessor;
    
    double lastTimeProcessorChecked;

    juce::Slider midiVolume;
    juce::TextEditor messageBox;
    
    std::unique_ptr<SliderAttachment> volumeSliderAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiTestPluginAudioProcessorEditor)
};
