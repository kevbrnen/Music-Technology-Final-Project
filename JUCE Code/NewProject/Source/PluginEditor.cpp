/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor(NewProjectAudioProcessor& p, juce::AudioProcessorValueTreeState& Gvts, juce::AudioProcessorValueTreeState& Fvts, juce::AudioProcessorValueTreeState& ProcChain, juce::AudioProcessorValueTreeState& Dvts, juce::AudioProcessorValueTreeState& Cvts, juce::AudioProcessorValueTreeState& Xvts, juce::AudioProcessorValueTreeState& DGvts, juce::AudioProcessorValueTreeState& Pvts, juce::AudioProcessorValueTreeState& Rvts, juce::AudioProcessorValueTreeState& Dist_vts): AudioProcessorEditor (&p), audioProcessor (p), globalComponent(Gvts, homeScreenComponent, procChainComponent), homeScreenComponent(Fvts, Dvts, Cvts, Xvts, DGvts, Pvts, Rvts, Dist_vts), procChainComponent(ProcChain)
{
    //Add all components to editor and show them
    addAndMakeVisible(globalComponent);
    
    addAndMakeVisible(homeScreenComponent);
    
    addAndMakeVisible(procChainComponent);
    procChainComponent.toFront(true); //So the 'popup' appears on top of all other components
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
    //Set bounds of all components
    float globalComponentHeight = getHeight()/8;
    float effectHeight = getHeight() - globalComponentHeight;
    
    globalComponent.setBounds(0, 0, getWidth(), globalComponentHeight);
    
    homeScreenComponent.setBounds(0, globalComponentHeight, getWidth(), effectHeight);
    
    procChainComponent.setBounds(0,  globalComponentHeight, getWidth()/2, effectHeight/2);
}

void NewProjectAudioProcessorEditor::toggleChainMenu()
{
    
}
