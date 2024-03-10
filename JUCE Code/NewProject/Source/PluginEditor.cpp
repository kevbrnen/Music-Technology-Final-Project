/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor(NewProjectAudioProcessor& p, juce::AudioProcessorValueTreeState& Gvts, juce::AudioProcessorValueTreeState& Fvts): AudioProcessorEditor (&p), audioProcessor (p), globalComponent(Gvts), filtComponent(Fvts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be
    
    addAndMakeVisible(globalComponent);
    addAndMakeVisible(filtComponent);
    
    setSize (500, 700);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.drawText("with Slider", 0, 0, 100, 20, juce::Justification(36));
}

void NewProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    filtComponent.setBounds(0, 0, getWidth()/2, getHeight()/2);
    globalComponent.setBounds(getWidth()/2, getHeight()/2, getWidth()/2, getHeight()/2);
    
    //***//
    //***//
}
