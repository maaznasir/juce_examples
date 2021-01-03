/*
  ==============================================================================

    MainContentComponent.cpp
    Created: 2 Jan 2021 4:18:17pm
    Author:  Maaz

  ==============================================================================
*/

#include "MainContentComponent.h"

MainContentComponent::MainContentComponent()
{
    // add items to the combo-box
    readTypeSelection.reset(new juce::ComboBox());
    
    addAndMakeVisible (readTypeSelection.get());
    readTypeSelection->addItem ("Read Whole File",  FileWhole);
    readTypeSelection->addItem ("Ready File by Line",   FileByLine);
    readTypeSelection->addItem ("Read File by Byte", FileByByte);
    readTypeSelection->addItem ("Read XML File into Value Tree", FileReadXMLValueTree);
    readTypeSelection->addItem ("Write XML File into Value Tree", FileWriteXMLValueTree);
    readTypeSelection->setSelectedId(FileWhole);
    
    readTypeSelection->onChange = [this] { readTypeSelectionChanged(); };
    
    
    fileComp.reset (new juce::FilenameComponent ("fileComp",
                                                 {},                       // current file
                                                 false,                    // can edit file name,
                                                 false,                    // is directory,
                                                 false,                    // is for saving,
                                                 {},                       // browser wildcard suffix,
                                                 {},                       // enforced suffix,
                                                 "Select file to open"));  // text when nothing selected
    addAndMakeVisible (fileComp.get());
    fileComp->addListener (this);
    //! [constructor]
    
    //! [setup]
    textContent.reset (new juce::TextEditor());
    addAndMakeVisible (textContent.get());
    textContent->setMultiLine (true);
    textContent->setReadOnly (true);
    textContent->setCaretVisible (false);
    
    setSize (600, 500);
}

void MainContentComponent::resized()
{
    readTypeSelection->setBounds(10, 10, getWidth() - 20, 20);
    fileComp->setBounds(10, 40, getWidth() - 20, 20);
    textContent->setBounds(10, 70, getWidth() - 20, getHeight() - 80);
}

void MainContentComponent::filenameComponentChanged (juce::FilenameComponent* fileComponentThatHasChanged)
{
    if (fileComponentThatHasChanged == fileComp.get())
    {
        switch(readTypeSelection->getSelectedId())
        {
            case FileReadXMLValueTree:
            case FileWriteXMLValueTree:
            {
                // Do nothing here when the file component has changed. The file reading is instead associated
                // with the combo box changing for the hard coded case
            }
                break;
                
            case FileWhole:
            {
                readFile (fileComp->getCurrentFile());
            }
                break;
                
            case FileByLine:
            {
                readFileByLine (fileComp->getCurrentFile());
            }
                break;
                
            case FileByByte:
            {
                readFileByBytes (fileComp->getCurrentFile());
            }
                break;
                
            default:
                break;
        }
    }
}

bool MainContentComponent::IsArray(juce::ValueTree node)
{
    static const juce::Identifier ArrayTypeId("Array");
    return node.hasProperty(ArrayTypeId);
}


void MainContentComponent::AddVarArrayUnderValueTreeNode(juce::ValueTree nodeToInsert, const juce::Identifier& identifier, juce::Array<juce::var>& array)
{
    static const juce::Identifier elementId("Element");
    static const juce::Identifier valueId("Value");
    static const juce::Identifier ArrayTypeId("Array");
    
    juce::ValueTree arrayNode(identifier);
    nodeToInsert.addChild(arrayNode, -1, nullptr);
    
    arrayNode.setProperty(ArrayTypeId, ArrayTypeId.toString(), nullptr);
    
    for (juce::var& elementValue : array)
    {
        juce::ValueTree elementNode(elementId);
        elementNode.setProperty(valueId, elementValue, nullptr);
        arrayNode.addChild(elementNode, -1, nullptr);
    }
}

bool MainContentComponent::ConvertValueTreeNodeToArray(juce::ValueTree arrayNode, juce::Array<juce::var>& array)
{
    static const juce::Identifier valueId("Value");
    
    if(IsArray(arrayNode))
    {
        const int iNumChildren = arrayNode.getNumChildren();
        for(int i=0; i<iNumChildren; i++)
        {
            juce::ValueTree childNode = arrayNode.getChild(i);
            if(childNode.hasProperty(valueId))
            {
                array.add(childNode.getProperty(valueId));
            }
        }
        
        return !array.isEmpty();
    }
    
    return false;
}

void MainContentComponent::readTypeSelectionChanged()
{
    if(readTypeSelection->getSelectedId() == FileReadXMLValueTree)
    {
        String testContents = String::fromUTF8 (BinaryData::read_xml, BinaryData::read_xmlSize);
        juce::ValueTree testValueTree;
        
        // reading
        if (auto xml = juce::XmlDocument::parse (testContents))
            testValueTree = juce::ValueTree::fromXml (*xml);
        
        if(testValueTree.isValid())
        {
            textContent->setText (testValueTree.toXmlString());
        }
    }
    else if(readTypeSelection->getSelectedId() == FileWriteXMLValueTree)
    {
        static const juce::Identifier catalogRoot("Catalog");
        static const juce::Identifier item11("Item1-1");
        static const juce::Identifier item12("Item1-2");
        static const juce::Identifier item21("Item2-1");
        static const juce::Identifier durationProperty("Duration");
        static const juce::Identifier chordsProperty("Chords");
        
        juce::ValueTree rootNode(catalogRoot);
        juce::ValueTree childNode;
        
        float fDuration;
        
        fDuration = 1.02f;
        childNode = juce::ValueTree(item11);
        
        childNode.setProperty(durationProperty, fDuration, nullptr);
        rootNode.addChild(childNode, -1, nullptr);
        
        fDuration = 1.56f;
        childNode = juce::ValueTree(item12);
        
        childNode.setProperty(durationProperty, fDuration, nullptr);
        
        juce::Array<juce::var> chordsArray;
        chordsArray.add("C#");
        chordsArray.add("Bb");
        chordsArray.add("F");
        chordsArray.add("G#");
        
        AddVarArrayUnderValueTreeNode(childNode, chordsProperty, chordsArray);
        
        rootNode.addChild(childNode, -1, nullptr);
        
        fDuration = 2.17f;
        childNode = juce::ValueTree(item21);
        
        childNode.setProperty(durationProperty, fDuration, nullptr);
        rootNode.addChild(childNode, -1, nullptr);
        
        juce::File newFile(juce::File(juce::File::getCurrentWorkingDirectory().getFullPathName()).getChildFile("write.xml"));
        
        if (auto xml = rootNode.createXml())
        {
            xml->writeTo (newFile);
            textContent->insertTextAtCaret(xml->toString());
            
            textContent->insertTextAtCaret(juce::newLine);
            textContent->insertTextAtCaret("-- Re-parsing and adding to Chords Array -- ");
            textContent->insertTextAtCaret(juce::newLine);
            
            juce::ValueTree readNode = juce::ValueTree::fromXml (*xml);
            juce::ValueTree itemNode = readNode.getChildWithName(item12);
            juce::ValueTree chordsNode = itemNode.getChildWithName(chordsProperty);
            
            if(chordsNode.isValid())
            {
                juce::Array<juce::var> readChordsArray;
                if(ConvertValueTreeNodeToArray(chordsNode, readChordsArray))
                {
                    readChordsArray.add("Ab");
                    itemNode.removeChild(itemNode.indexOf(chordsNode), nullptr);
                    AddVarArrayUnderValueTreeNode(itemNode, chordsProperty, readChordsArray);
                }
            }
            
            textContent->insertTextAtCaret(readNode.toXmlString());
        }
    }
    else
    {
        filenameComponentChanged(fileComp.get());
    }
}

void MainContentComponent::readFile (const juce::File& fileToRead)
{
    if (! fileToRead.existsAsFile()) // [1]
        return;
    
    auto fileText = fileToRead.loadFileAsString();
    
    textContent->setText (fileText);
}

void MainContentComponent::readFileByLine (const juce::File& fileToRead)
{
    if (! fileToRead.existsAsFile())
        return;  // file doesn't exist
    
    juce::FileInputStream inputStream (fileToRead); // [2]
    
    if (! inputStream.openedOk())
        return;  // failed to open
    
    textContent->clear();
    
    juce::Font normalFont = textContent->getFont();
    juce::Font titleFont = normalFont.withHeight (normalFont.getHeight() * 1.5f).boldened();
    juce::String asterix ("*");
    
    while (! inputStream.isExhausted()) // [3]
    {
        auto line = inputStream.readNextLine();
        
        if (line.startsWith (asterix))
        {
            line = line.removeCharacters (asterix);
            textContent->setFont (titleFont);
        }
        else
        {
            textContent->setFont (normalFont);
        }
        
        // append the text to the textContent
        textContent->insertTextAtCaret (line + juce::newLine);
    }
}

juce::Colour MainContentComponent::getRandomColour (float minBrightness)
{
    auto& random = juce::Random::getSystemRandom();
    juce::Colour colour ((juce::uint8) random.nextInt (256),
                         (juce::uint8) random.nextInt (256),
                         (juce::uint8) random.nextInt (256));
    
    return colour.getBrightness() >= minBrightness ? colour
    : colour.withBrightness (minBrightness);
}

juce::String MainContentComponent::readUpToNextSpace (juce::FileInputStream& inputStream)
{
    juce::MemoryBlock buffer (256);
    auto* data = static_cast<char*> (buffer.getData());
    size_t i = 0;
    
    while ((data[i] = inputStream.readByte()) != 0 && i < buffer.getSize())
        if (data[i++] == ' ')
            break;
    
    return juce::String::fromUTF8 (data, (int) i); // [4]
}

void MainContentComponent::readFileByBytes (const juce::File& fileToRead)
{
    if (! fileToRead.existsAsFile())
        return;  // file doesn't exist
    
    if (std::unique_ptr<juce::FileInputStream> inputStream { fileToRead.createInputStream() })
    {
        textContent->clear();
        
        while (! inputStream->isExhausted())
        {
            auto nextWord = readUpToNextSpace (*inputStream);
            textContent->setColour (juce::TextEditor::textColourId, getRandomColour (0.75f));
            textContent->insertTextAtCaret (nextWord);
        }
    }
}


