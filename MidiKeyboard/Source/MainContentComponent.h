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
