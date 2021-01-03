/*
  ==============================================================================

    MainContentComponent.cpp
    Created: 1 Jan 2021 8:46:45pm
    Author:  Maaz

  ==============================================================================
*/

#include "MainContentComponent.h"

MainContentComponent::MainContentComponent()
: startTime (juce::Time::getMillisecondCounterHiRes() * 0.001)
{
    addAndMakeVisible (bassDrumButton);
    bassDrumButton.setButtonText ("Bass Drum (36)");
    bassDrumButton.onClick = [this] { setNoteNumber (36); };
    
    addAndMakeVisible (snareDrumButton);
    snareDrumButton.setButtonText ("Snare Drum (38)");
    snareDrumButton.onClick = [this] { setNoteNumber (38); };
    
    addAndMakeVisible (closedHiHatButton);
    closedHiHatButton.setButtonText ("Closed HH (42)");
    closedHiHatButton.onClick = [this] { setNoteNumber (42); };
    
    addAndMakeVisible (openHiHatButton);
    openHiHatButton.setButtonText ("Open HH (46)");
    openHiHatButton.onClick = [this] { setNoteNumber (46); };
    
    addAndMakeVisible (volumeLabel);
    volumeLabel.setText ("Volume (CC7)", juce::dontSendNotification);
    
    addAndMakeVisible (volumeSlider);
    volumeSlider.setRange (0, 127, 1);
    //! [onValueChange]
    volumeSlider.onValueChange = [this]
    {
        auto message = juce::MidiMessage::controllerEvent (10, 7, (int) volumeSlider.getValue());
        message.setTimeStamp (juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime);
        addMessageToBuffer (message);
    };
    //! [onValueChange]
    
    addAndMakeVisible (midiMessagesBox);
    midiMessagesBox.setMultiLine (true);
    midiMessagesBox.setReturnKeyStartsNewLine (true);
    midiMessagesBox.setReadOnly (true);
    midiMessagesBox.setScrollbarsShown (true);
    midiMessagesBox.setCaretVisible (false);
    midiMessagesBox.setPopupMenuEnabled (true);
    midiMessagesBox.setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x32ffffff));
    midiMessagesBox.setColour (juce::TextEditor::outlineColourId, juce::Colour (0x1c000000));
    midiMessagesBox.setColour (juce::TextEditor::shadowColourId, juce::Colour (0x16000000));
    
    setSize (800, 300);
    startTimer (1);
}

void MainContentComponent::paint (juce::Graphics&)
{
    
}

void MainContentComponent::resized()
{
    auto halfWidth = getWidth() / 2;
    
    auto buttonsBounds = getLocalBounds().withWidth (halfWidth).reduced (10);
    
    bassDrumButton   .setBounds (buttonsBounds.getX(), 10,  buttonsBounds.getWidth(), 20);
    snareDrumButton  .setBounds (buttonsBounds.getX(), 40,  buttonsBounds.getWidth(), 20);
    closedHiHatButton.setBounds (buttonsBounds.getX(), 70,  buttonsBounds.getWidth(), 20);
    openHiHatButton  .setBounds (buttonsBounds.getX(), 100, buttonsBounds.getWidth(), 20);
    volumeLabel      .setBounds (buttonsBounds.getX(), 190, buttonsBounds.getWidth(), 20);
    volumeSlider     .setBounds (buttonsBounds.getX(), 220, buttonsBounds.getWidth(), 20);
    
    midiMessagesBox.setBounds (getLocalBounds().withWidth (halfWidth).withX (halfWidth).reduced (10));
}

juce::String MainContentComponent::getMidiMessageDescription (const juce::MidiMessage& m)
{
    if (m.isNoteOn())           return "Note on "          + juce::MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
    if (m.isNoteOff())          return "Note off "         + juce::MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
    if (m.isProgramChange())    return "Program change "   + juce::String (m.getProgramChangeNumber());
    if (m.isPitchWheel())       return "Pitch wheel "      + juce::String (m.getPitchWheelValue());
    if (m.isAftertouch())       return "After touch "      + juce::MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) +  ": " + juce::String (m.getAfterTouchValue());
    if (m.isChannelPressure())  return "Channel pressure " + juce::String (m.getChannelPressureValue());
    if (m.isAllNotesOff())      return "All notes off";
    if (m.isAllSoundOff())      return "All sound off";
    if (m.isMetaEvent())        return "Meta event";
    
    if (m.isController())
    {
        juce::String name (juce::MidiMessage::getControllerName (m.getControllerNumber()));
        
        if (name.isEmpty())
            name = "[" + juce::String (m.getControllerNumber()) + "]";
        
        return "Controller " + name + ": " + juce::String (m.getControllerValue());
    }
    
    return juce::String::toHexString (m.getRawData(), m.getRawDataSize());
}

void MainContentComponent::addMessageToBuffer (const juce::MidiMessage& message)
{
    auto timestamp = message.getTimeStamp();
    auto sampleNumber =  (int) (timestamp * sampleRate);
    midiBuffer.addEvent (message, sampleNumber);
}

void MainContentComponent::setNoteNumber (int noteNumber)
{
    auto message = juce::MidiMessage::noteOn (1, noteNumber, (juce::uint8) 100);
    message.setTimeStamp (juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime);
    addMessageToBuffer (message);
    
    auto messageOff = juce::MidiMessage::noteOff (message.getChannel(), message.getNoteNumber());
    messageOff.setTimeStamp (message.getTimeStamp() + 0.1);
    addMessageToBuffer (messageOff);
}

void MainContentComponent::logMessage (const juce::String& m)
{
    midiMessagesBox.moveCaretToEnd();
    midiMessagesBox.insertTextAtCaret (m + juce::newLine);
}

void MainContentComponent::addMessageToList (const juce::MidiMessage& message)
{
    auto time = message.getTimeStamp();
    
    auto hours   = ((int) (time / 3600.0)) % 24;
    auto minutes = ((int) (time / 60.0)) % 60;
    auto seconds = ((int) time) % 60;
    auto millis  = ((int) (time * 1000.0)) % 1000;
    
    auto timecode = juce::String::formatted ("%02d:%02d:%02d.%03d",
                                             hours,
                                             minutes,
                                             seconds,
                                             millis);
    
    logMessage (timecode + "  -  " + getMidiMessageDescription (message));
}

void MainContentComponent::timerCallback()
{
    auto currentTime = juce::Time::getMillisecondCounterHiRes() * 0.001 - startTime;
    auto currentSampleNumber = (int) (currentTime * sampleRate);     // [4]
    
    for (const auto metadata : midiBuffer)                           // [5]
    {
        if (metadata.samplePosition > currentSampleNumber)           // [6]
            break;
        
        auto message = metadata.getMessage();
        message.setTimeStamp (metadata.samplePosition / sampleRate); // [7]
        addMessageToList (message);
    }
    
    midiBuffer.clear (previousSampleNumber, currentSampleNumber - previousSampleNumber); // [8]
    previousSampleNumber = currentSampleNumber;                                          // [9]
}
