/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiTestPluginAudioProcessorEditor::MidiTestPluginAudioProcessorEditor (MidiTestPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
    
    // these define the parameters of our slider object
    midiVolume.setSliderStyle (juce::Slider::LinearBarVertical);
    //midiVolume.setRange (0.0, 1.0f, 1.0f/127.0f);
    midiVolume.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    midiVolume.setPopupDisplayEnabled (true, false, this);
    
    volumeSliderAttachment.reset(new SliderAttachment (audioProcessor.parameters, "scaledNoteVelocity", midiVolume));
    
    // this function adds the slider to the editor
    addAndMakeVisible (&midiVolume);
    
    // add the listener to the slider
    midiVolume.addListener (this);
    
    addAndMakeVisible (messageBox);
    messageBox.setMultiLine (true);
    messageBox.setReturnKeyStartsNewLine (true);
    messageBox.setReadOnly (true);
    messageBox.setScrollbarsShown (true);
    messageBox.setCaretVisible (false);
    messageBox.setPopupMenuEnabled (true);
    messageBox.setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x32ffffff));
    messageBox.setColour (juce::TextEditor::outlineColourId, juce::Colour (0x1c000000));
    messageBox.setColour (juce::TextEditor::shadowColourId, juce::Colour (0x16000000));
    
    lastTimeProcessorChecked = juce::Time::getMillisecondCounterHiRes();
    
    startTimer(33);
}

MidiTestPluginAudioProcessorEditor::~MidiTestPluginAudioProcessorEditor()
{
}

//==============================================================================
void MidiTestPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);

    char buffer [50];
    sprintf (buffer, "Midi Volume - %.2f ", midiVolume.getValue());

    g.drawFittedText (buffer, 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void MidiTestPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    const int iHorizontalPadding = 20;
    const int iVerticalPadding = 30;
    const int iSliderWidth = 20;
    
    // sets the position and size of the slider with arguments (x, y, width, height)
    midiVolume.setBounds (iHorizontalPadding, iVerticalPadding, iSliderWidth, getHeight() - 2*iVerticalPadding);
    
    messageBox.setBounds (2*iHorizontalPadding + 20 , 30, getWidth() - (3*iHorizontalPadding) - iSliderWidth, getHeight() - 2*iVerticalPadding);
}

void MidiTestPluginAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    repaint();
}

void MidiTestPluginAudioProcessorEditor::timerCallback()
{
    juce::Array<juce::String> pendingMessages;
    audioProcessor.ReadAndFlushMidiMsgLog(pendingMessages);
    
    const int iNumMessages = pendingMessages.size();
    for(int i=0; i<iNumMessages; i++)
    {
        messageBox.insertTextAtCaret(pendingMessages[i]);
        messageBox.insertTextAtCaret("\n");
    }
}
