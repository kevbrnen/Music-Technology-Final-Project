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
    addAndMakeVisible(globalComponent);
    
    //Filter Component
    addAndMakeVisible(filtComponent); //Add it
    filtComponent.setEnabled(false); //Deactivate it and make it invisible
    filtComponent.setVisible(false);
    Filter_show_button.setButtonText("Filter"); //Create a button to toggle the filter component on and off
    Filter_show_button.setClickingTogglesState(true); //All of this can be moved to the "Home Screen" When created
    Filter_show_button.onClick = [this](){ //Enables/disables depending on button state
        if(Filter_show_button.getToggleState()){
            filtComponent.setEnabled(true);
            filtComponent.setVisible(true);
        }
        else{
            filtComponent.setEnabled(false);
            filtComponent.setVisible(false);
        }
    };
    addAndMakeVisible(Filter_show_button);
    
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
    float globalHeight = getHeight()/8;
    globalComponent.setBounds(0, 0, getWidth(), globalHeight);
    filtComponent.setBounds(0, globalHeight, getWidth()/2, getHeight()/2);
    
    
    Filter_show_button.setBounds(getWidth()/2 + 10, globalHeight + 10, 50, 25);
    
    //***//
    //***//
}
