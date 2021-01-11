/*
  ==============================================================================

    ScalesKeyboardComponent.h
    Created: 10 Jan 2021 4:57:14pm
    Author:  Maaz

  ==============================================================================
*/

#include <JuceHeader.h>

#pragma once

using namespace juce;

class ScalesKeyboardComponent : public MidiKeyboardComponent
{
public:
    ScalesKeyboardComponent(MidiKeyboardState& state,
                            MidiKeyboardComponent::Orientation orientation);
    
};
