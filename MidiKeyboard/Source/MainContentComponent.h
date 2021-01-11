/*
  ==============================================================================

   This file is part of the JUCE tutorials.
   Copyright (c) 2020 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             HandlingMidiEventsTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Handles incoming midi events.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_processors, juce_audio_utils, juce_core,
                   juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2019, linux_make

 type:             Component
 mainClass:        MainContentComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/


#pragma once

#include <JuceHeader.h>

//==============================================================================
class MainContentComponent  : public juce::Component,
                              private juce::MidiInputCallback,
                              private juce::MidiKeyboardStateListener
{
public:
//! [initialiser]
    MainContentComponent();

    ~MainContentComponent() override;

    void paint (juce::Graphics& g) override;

    void resized() override;

private:
    static juce::String getMidiMessageDescription (const juce::MidiMessage& m);

    void logMessage (const juce::String& m);

    /** Starts listening to a MIDI input device, enabling it if necessary. */
    void setMidiInput (int index);

    // These methods handle callbacks from the midi device + on-screen keyboard..
    void handleIncomingMidiMessage (juce::MidiInput* source, const juce::MidiMessage& message) override;

    void handleNoteOn (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;

    void handleNoteOff (juce::MidiKeyboardState*, int midiChannel, int midiNoteNumber, float velocity) override;

    // This is used to dispach an incoming message to the message thread
    class IncomingMessageCallback   : public juce::CallbackMessage
    {
    public:
        IncomingMessageCallback (MainContentComponent* o, const juce::MidiMessage& m, const juce::String& s)
           : owner (o), message (m), source (s)
        {}

        void messageCallback() override
        {
            if (owner != nullptr)
                owner->addMessageToList (message, source);
        }

        Component::SafePointer<MainContentComponent> owner;
        juce::MidiMessage message;
        juce::String source;
    };

    void postMessageToList (const juce::MidiMessage& message, const juce::String& source);

    void addMessageToList (const juce::MidiMessage& message, const juce::String& source);

    //==============================================================================

    juce::AudioDeviceManager deviceManager;           // [1]
    juce::ComboBox midiInputList;                     // [2]
    juce::Label midiInputListLabel;
    int lastInputIndex = 0;                           // [3]
    bool isAddingFromMidiInput = false;               // [4]

    juce::MidiKeyboardState keyboardState;            // [5]
    juce::MidiKeyboardComponent keyboardComponent;    // [6]

    juce::TextEditor midiMessagesBox;
    double startTime;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
