/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor(NewProjectAudioProcessor& p, juce::AudioProcessorValueTreeState& Gvts, juce::AudioProcessorValueTreeState& Fvts, juce::AudioProcessorValueTreeState& ProcChain, juce::AudioProcessorValueTreeState& Dvts, juce::AudioProcessorValueTreeState& Cvts): AudioProcessorEditor (&p), audioProcessor (p), globalComponent(Gvts, homeScreenComponent, procChainComponent), homeScreenComponent(Fvts, Dvts, Cvts), procChainComponent(ProcChain)
{
    addAndMakeVisible(globalComponent);
    
    addAndMakeVisible(homeScreenComponent);
    
    addAndMakeVisible(procChainComponent);
    procChainComponent.toFront(true);
    procChainComponent.setEnabled(false);
    procChainComponent.setVisible(false);

    
    
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
    
    procChainComponent.setBounds(0,  globalComponentHeight, getWidth()/2, effectHeight/2);
}

void NewProjectAudioProcessorEditor::toggleChainMenu()
{
    
}
