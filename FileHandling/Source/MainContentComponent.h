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

 name:             FileHandling
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Reads and displays a text file.

 dependencies:     juce_core, juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics
 exporters:        xcode_mac, vs2019, linux_make

 type:             Component
 mainClass:        MainContentComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/

#include <JuceHeader.h>

#pragma once

enum eFileReadyType
{
    FileWhole = 1,
    FileByLine,
    FileByByte,
    FileReadXMLValueTree,
    FileWriteXMLValueTree,
    FileTotalReadTypes
};

//==============================================================================
class MainContentComponent   : public juce::Component,
                               public juce::FilenameComponentListener
{
public:

    MainContentComponent();

    void resized() override;

    void filenameComponentChanged (juce::FilenameComponent* fileComponentThatHasChanged) override;

    // MN TODO: These array helper functions should probably be moved into a separate lib or header include somewhere
    
    /** Check if value tree node is an array */
    bool IsArray(juce::ValueTree node);

    /** Super handy function to automatically insert arrays into the value tree in a "safe" way. */
    void AddVarArrayUnderValueTreeNode(juce::ValueTree nodeToInsert, const juce::Identifier& identifier, juce::Array<juce::var>& array);
    
    /** This does the converse of the above, and converts a value tree array node back to a var array so it can be manipilated */
    bool ConvertValueTreeNodeToArray(juce::ValueTree arrayNode, juce::Array<juce::var>& array);
    
    /** Update text box when combo box is changed to run hard coded value tree logic */
    void readTypeSelectionChanged();
    
    /** Ready entire file as string */
    void readFile (const juce::File& fileToRead);
    
    /** Ready file line by line */
    void readFileByLine (const juce::File& fileToReßad);
    
    static juce::Colour getRandomColour (float minBrightness);
    
    static juce::String readUpToNextSpace (juce::FileInputStream& inputStream);
    
    /** Ready file byte by byte*/
    void readFileByBytes (const juce::File& fileToRead);

private:

    std::unique_ptr<juce::FilenameComponent> fileComp;
    std::unique_ptr<juce::TextEditor>        textContent;
    std::unique_ptr<juce::ComboBox>          readTypeSelection;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
