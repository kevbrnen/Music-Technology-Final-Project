/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor(NewProjectAudioProcessor& p, juce::AudioProcessorValueTreeState& Gvts, juce::AudioProcessorValueTreeState& Fvts): AudioProcessorEditor (&p), homeScreenComponent(Fvts), audioProcessor (p), globalComponent(Gvts, homeScreenComponent)
{
    addAndMakeVisible(globalComponent);
    
    addAndMakeVisible(homeScreenComponent);
    
    setSize (1000, 600);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void NewProjectAudioProcessorEditor::resized()
{
    float globalComponentHeight = getHeight()/8;
    float effectHeight = getHeight() - globalComponentHeight;
    
    globalComponent.setBounds(0, 0, getWidth(), globalComponentHeight);
    
    homeScreenComponent.setBounds(0, globalComponentHeight, getWidth(), effectHeight);
}

void NewProjectAudioProcessorEditor::showComponent()
{
    
}

void NewProjectAudioProcessorEditor::hideComponent()
{
    
}
