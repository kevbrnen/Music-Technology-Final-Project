/*
  ==============================================================================

    FilterComponent.cpp
    Created: 9 Mar 2024 10:36:04pm
    Author:  Kevin Brennan
    
    Component to show UI for Filter Effect
  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& vts)
{
//Toggle
    LPF_Toggle.setClickingTogglesState(true);
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
    
    ToggleLabel.setText("Filter Effect", juce::dontSendNotification);
    addAndMakeVisible(ToggleLabel);
    
//LFO Toggle
    Filter_LFO_Toggle.setClickingTogglesState(true);
    Filter_LFO_Toggle.setButtonText("OFF");
    Filter_LFO_Toggle.setEnabled(true);
    Filter_LFO_Toggle.setVisible(true);
    Filter_LFO_Toggle.onClick = [this, &vts]()
    {
        if(Filter_LFO_Toggle.getToggleState())
        {
            Filter_LFO_Toggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
            Filter_LFO_Toggle.setButtonText("ON");
            vts.state.setProperty("filter_LFO_toggle", true, nullptr);
            
        }
        else
        {
            Filter_LFO_Toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            Filter_LFO_Toggle.setButtonText("OFF");
            vts.state.setProperty("filter_LFO_toggle", false, nullptr);
        }
    };
    Filter_LFO_Toggle_Attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(vts, "filter_LFO_toggle", Filter_LFO_Toggle));
    addAndMakeVisible(Filter_LFO_Toggle);
    
    LFOLabel.setText("LFO", juce::dontSendNotification);
    LFOLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(LFOLabel);
    
//LFO Display
    addAndMakeVisible(LFOThumb);

    
//Cutoff Slider
    cutoffFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    cutoffFrequencyAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "cutoff_frequency", cutoffFrequencySlider));
    addAndMakeVisible(cutoffFrequencySlider);
    
    cutoffFrequencyLabel.setText("Cutoff Frequency", juce::dontSendNotification);
    addAndMakeVisible(cutoffFrequencyLabel);
    
//Resonance Slider
    resonanceSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    resonanceAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "resonance", resonanceSlider));
    addAndMakeVisible(resonanceSlider);
    
    resonanceLabel.setText("Resonance", juce::dontSendNotification);
    addAndMakeVisible(resonanceLabel);
    
    
//Gain Slider
    filterGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "filter_gain", filterGainSlider));
    filterGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    filterGainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    filterGainSlider.setTextValueSuffix("dB");
    filterGainSlider.setRange(-48.0, 10.0);
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
    g.fillAll (juce::Colours::lightblue);   // clear the background

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
    ToggleLabel.attachToComponent(&LPF_Toggle, true);
    
    Filter_LFO_Toggle.setBounds(getWidth()-60, 250, 50, 25);
    LFOLabel.attachToComponent(&Filter_LFO_Toggle, true);
    
    filterGainSlider.setBounds(getWidth() - 110, 60, 100, 100);
    filterGainLabel.attachToComponent(&filterGainSlider, true);
    
    cutoffFrequencySlider.setBounds(15, 35, 100, getHeight()-50);
    cutoffFrequencyLabel.setBounds(10, 30, 75, 50);
    
    resonanceSlider.setBounds(200, getHeight()/4, 200, 200);
    resonanceLabel.attachToComponent(&resonanceSlider, true);
    
    LFOThumb.setBounds(getWidth()-200, 300, 190, 75);
}
