/*
  ==============================================================================

    ProcessingChainSelectionComponent.h
    Created: 4 Apr 2024 2:34:06pm
    Author:  Kevin Brennan

    UI for the processing chain menu
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ProcessingChainSelectionComponent  : public juce::Component
{
public:
    ProcessingChainSelectionComponent(juce::AudioProcessorValueTreeState& ProcChain)
    {
        addAndMakeVisible(Slot1);
        Slot1.addItemList(processorChoices, 1);
        Slot1.setSelectedId(1);
        Slot1_attachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(ProcChain, "slot1", Slot1));
        
        addAndMakeVisible(Slot2);
        Slot2.addItemList(processorChoices, 1);
        Slot2.setSelectedId(2);
        Slot2_attachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(ProcChain, "slot2", Slot2));
        
        addAndMakeVisible(Slot3);
        Slot3.addItemList(processorChoices, 1);
        Slot3.setSelectedId(3);
        Slot3_attachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(ProcChain, "slot3", Slot3));
        
        //addAndMakeVisible(Slot4);
        //Slot4.addItemList(processorChoices, 1);
        //Slot4.setSelectedId(4);
        //Slot4_attachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(ProcChain, "slot4", Slot4));

    }
    
    ~ProcessingChainSelectionComponent() override
    {
    }
    
    
    void paint (juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black);
        g.setColour (juce::Colours::white);
        g.drawRect (getLocalBounds(), 4);   // draw an outline around the component

        g.setColour (juce::Colours::white);
        g.setFont (14.0f);
        g.drawText ("ProcessingChainSelectionComponent", getLocalBounds(),
                    juce::Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
        Slot1.setBounds(10, 10, getLocalBounds().getWidth() - 30, 20);
        
        Slot2.setBounds(10, 10 + 20 + 10, getLocalBounds().getWidth() - 30, 20);
        
        Slot3.setBounds(10, 10 + 20 + 10 + 20 + 10, getLocalBounds().getWidth() - 30, 20);
        
        //Slot4.setBounds(10, 10 + 20 + 10 + 20 + 10 + 20 + 10, getLocalBounds().getWidth() - 30, 20);
        

    }

private:
    juce::StringArray processorChoices{"Empty", "Filter", "Delay", "Convolution", "Delay-Xpanse"}; //Available Processor choices, should match  choices in plugin editor
    
    juce::ComboBox Slot1, Slot2, Slot3, Slot4; //combo boxes
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>Slot1_attachment, Slot2_attachment, Slot3_attachment, Slot4_attachment; //Attachment to APVTS for processing chain choices
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessingChainSelectionComponent)
};