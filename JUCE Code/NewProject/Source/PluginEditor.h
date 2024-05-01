/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GlobalControlsComponent.h"
#include "HomeScreenComponent.h"
#include "ProcessingChainSelectionComponent.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    //Set up Editor with value trees from audio processor.
    //Send each value tree to the corresponding effect component
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p, juce::AudioProcessorValueTreeState& Gvts, juce::AudioProcessorValueTreeState& Fvts, juce::AudioProcessorValueTreeState& ProcChain, juce::AudioProcessorValueTreeState& Dvts, juce::AudioProcessorValueTreeState& Cvts, juce::AudioProcessorValueTreeState& Xvts, juce::AudioProcessorValueTreeState& DGvts, juce::AudioProcessorValueTreeState& Pvts, juce::AudioProcessorValueTreeState& Rvts);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void toggleChainMenu();
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NewProjectAudioProcessor& audioProcessor;
    
    //Other components to show in main
    GlobalControlsComponent globalComponent; //Always Shown
    
    HomeScreenComponent homeScreenComponent;
    
    ProcessingChainSelectionComponent procChainComponent;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
