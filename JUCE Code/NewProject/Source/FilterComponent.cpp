/*
  ==============================================================================

    FilterComponent.cpp
    Created: 9 Mar 2024 10:36:04pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& vts)
{
    //Toggle
    LPF_Toggle.setClickingTogglesState(true);
    //LPF_Toggle.setState(juce::Button::ButtonState(false));
    LPF_Toggle.setButtonText("OFF");
    LPF_Toggle.setEnabled(true);
    LPF_Toggle.setVisible(true);
    LPF_Toggle.onClick = [this, &vts]()
    {
        if(LPF_Toggle.getToggleState())
        {
            LPF_Toggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
            LPF_Toggle.setButtonText("ON");
            vts.state.setProperty("filter_toggle", true, nullptr);
            
        }
        else
        {
            LPF_Toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            LPF_Toggle.setButtonText("OFF");
            vts.state.setProperty("filter_toggle", false, nullptr);
        }
        
    };
    Filter_OnOff_Attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(vts, "filter_toggle", LPF_Toggle));
    addAndMakeVisible(LPF_Toggle);
    
    
    //Cutoff Slider
    cutoffFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    cutoffFrequencyAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "cutoff_frequency", cutoffFrequencySlider));
    addAndMakeVisible(cutoffFrequencySlider);
    
    cutoffFrequencyLabel.setText("Cutoff Frequency", juce::dontSendNotification);
    addAndMakeVisible(cutoffFrequencyLabel);
    
    
    //Gain
    filterGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "filter_gain", filterGainSlider));
    filterGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    filterGainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    filterGainSlider.setTextValueSuffix("dB");
    filterGainSlider.setRange(-48.0, 0.0);
    addAndMakeVisible(filterGainSlider);
    
    filterGainLabel.setText("Filter Gain", juce::dontSendNotification);
    filterGainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(filterGainLabel);
    
    
    setSize(150, 300);
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::blue);   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 5);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("FilterComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void FilterComponent::resized()
{
    LPF_Toggle.setBounds(getWidth()-60, 20, 50, 25);
    
    filterGainSlider.setBounds(getWidth() - 110, 60, 100, 100);
    filterGainLabel.attachToComponent(&filterGainSlider, true);
    
    cutoffFrequencySlider.setBounds(15, 35, 100, getHeight()-50);
    cutoffFrequencyLabel.setBounds(10, 30, 75, 50);

}
