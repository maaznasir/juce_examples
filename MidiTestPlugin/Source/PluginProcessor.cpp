/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    params.add ( std::make_unique<juce::AudioParameterFloat> ("scaledNoteVelocity",            // parameterID
                                                              "scaledNoteVelocity",            // parameter name
                                                              0.0f,              // minimum value
                                                              1.0f,              // maximum value
                                                            0.0f));
    
    return params;
}

//==============================================================================
MidiTestPluginAudioProcessor::MidiTestPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
        parameters (*this, nullptr, juce::Identifier ("MidiTestPlugin"), createParameterLayout()),
        scaledNoteVelocityParameter(nullptr)
#endif
{
    scaledNoteVelocityParameter = parameters.getRawParameterValue ("scaledNoteVelocity");
}

MidiTestPluginAudioProcessor::~MidiTestPluginAudioProcessor()
{
}

//==============================================================================
const juce::String MidiTestPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiTestPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MidiTestPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MidiTestPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MidiTestPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MidiTestPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MidiTestPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MidiTestPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MidiTestPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void MidiTestPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MidiTestPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MidiTestPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiTestPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MidiTestPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();
    
    juce::MidiBuffer processedMidi;
    int time;
    juce::MidiMessage m;
    
    for (juce::MidiBuffer::Iterator i (midiMessages); i.getNextEvent (m, time);)
    {
        //Custom messaging
        if (m.isNoteOn())
        {
            float fScaled = (*scaledNoteVelocityParameter)*127.0f;
            juce::uint8 newVel = (juce::uint8)fScaled;
            m = juce::MidiMessage::noteOn(m.getChannel(), m.getNoteNumber(), newVel);
            
            //printf("Note On - Channel:%d Number:%d \n", m.getChannel(), m.getNoteNumber());
        }
        else if (m.isNoteOff())
        {
            //printf("Note Off - Channel:%d Number:%d \n", m.getChannel(), m.getNoteNumber());
        }
        else if (m.isAftertouch())
        {
        }
        else if (m.isPitchWheel())
        {
        }
        
        if (m.isController())
        {

        }
        
        juce::String msg = m.getDescription();
        if(m.isNoteOn() || m.isNoteOff())
        {
            msg += " [" + juce::String(m.getNoteNumber()) + "]";
        }
        
        AddMidiMsgToLog(msg);
        
        processedMidi.addEvent (m, time);
    }
    
    midiMessages.swapWith (processedMidi);
}

//==============================================================================
bool MidiTestPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiTestPluginAudioProcessor::createEditor()
{
    return new MidiTestPluginAudioProcessorEditor (*this);
}

//==============================================================================
void MidiTestPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    juce::ValueTree state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void MidiTestPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiTestPluginAudioProcessor();
}

void MidiTestPluginAudioProcessor::AddMidiMsgToLog(juce::String& msg)
{
    const juce::ScopedLock myScopedLock (midiMsgLock);
    
    if(midiMsgLogBuffer.size() >= MAX_MID_MSG_LOG_BUFFER_SIZE)
    {
        midiMsgLogBuffer.remove(0);
    }

    midiMsgLogBuffer.add(msg);
}

void MidiTestPluginAudioProcessor::ReadAndFlushMidiMsgLog(juce::Array<juce::String>& msgBufferToCopyTo)
{
    const juce::ScopedLock myScopedLock (midiMsgLock);
    
    msgBufferToCopyTo = midiMsgLogBuffer;
    midiMsgLogBuffer.clear();
}
