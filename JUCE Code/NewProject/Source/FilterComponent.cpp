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
    
//Toggle filt2
    filt2_Toggle.setClickingTogglesState(true);
    filt2_Toggle.setButtonText("OFF");
    filt2_Toggle.setEnabled(true);
    filt2_Toggle.setVisible(true);
    filt2_Toggle.onClick = [this, &vts]()
    {
        if(filt2_Toggle.getToggleState())
        {
            filt2_Toggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
            filt2_Toggle.setButtonText("ON");
            vts.state.setProperty("filter_toggle2", true, nullptr);
            
        }
        else
        {
            filt2_Toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            filt2_Toggle.setButtonText("OFF");
            vts.state.setProperty("filter_toggle2", false, nullptr);
        }
        
    };
    Filt2_OnOff_Attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(vts, "filter_toggle2", filt2_Toggle));
    addAndMakeVisible(filt2_Toggle);
    
    filt2Label.setText("Filter 2", juce::dontSendNotification);
    addAndMakeVisible(filt2Label);
    
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

    
//Cutoff Slider
    cutoffFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    cutoffFrequencyAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "cutoff_frequency", cutoffFrequencySlider));
    cutoffFrequencySlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::lightskyblue);
    addAndMakeVisible(cutoffFrequencySlider);
    
    cutoffFrequencyLabel.setText("Cutoff Frequency", juce::dontSendNotification);
    addAndMakeVisible(cutoffFrequencyLabel);
    
    cutoffFrequencySlider2.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    cutoffFrequencyAttachment2.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "cutoff_frequency2", cutoffFrequencySlider2));
    cutoffFrequencySlider2.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::lightskyblue);
    addAndMakeVisible(cutoffFrequencySlider2);
    
    cutoffFrequencyLabel2.setText("Cutoff Frequency", juce::dontSendNotification);
    addAndMakeVisible(cutoffFrequencyLabel2);
    
//Resonance Slider
    resonanceSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    resonanceAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "resonance", resonanceSlider));
    resonanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    resonanceSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::lightskyblue);
    addAndMakeVisible(resonanceSlider);
    
    resonanceLabel.setText("Resonance", juce::dontSendNotification);
    addAndMakeVisible(resonanceLabel);
    
    resonanceSlider2.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    resonanceAttachment2.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "resonance2", resonanceSlider2));
    resonanceSlider2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    resonanceSlider2.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::lightskyblue);
    addAndMakeVisible(resonanceSlider2);
    
    resonanceLabel2.setText("Resonance", juce::dontSendNotification);
    addAndMakeVisible(resonanceLabel2);
    
    
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
    
//Combobox
    TypeSelector.addItemList(Filter_Choices, 1);
    TypeSelector.setSelectedId(1);
    Type_attachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(vts, "filter_types", TypeSelector));
    addAndMakeVisible(TypeSelector);
    
    TypeSelector2.addItemList(Filter_Choices, 1);
    TypeSelector2.setSelectedId(1);
    Type_attachment2.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(vts, "filter_types2", TypeSelector2));
    addAndMakeVisible(TypeSelector2);
    
    
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
}

void FilterComponent::resized()
{
    LPF_Toggle.setBounds(getWidth()-60, 20, 50, 25);
    ToggleLabel.attachToComponent(&LPF_Toggle, true);
    
    Filter_LFO_Toggle.setBounds(getWidth()-60, 250, 50, 25);
    LFOLabel.attachToComponent(&Filter_LFO_Toggle, true);
    
    filterGainSlider.setBounds(getWidth() - 110, 60, 100, 100);
    filterGainLabel.attachToComponent(&filterGainSlider, true);
    
    cutoffFrequencySlider.setBounds(30, 35, 100, getHeight()-50);
    cutoffFrequencyLabel.setBounds(10, 30, 75, 50);
    
    resonanceSlider.setBounds(210, getHeight()/4, 150, 150);
    resonanceLabel.attachToComponent(&resonanceSlider, true);
    
    
    filt2_Toggle.setBounds(getWidth()-290, (4*getHeight()/6), 50, 25);
    filt2Label.attachToComponent(&filt2_Toggle, true);
    
    cutoffFrequencySlider2.setBounds(475, 35, 100, getHeight()-50);
    cutoffFrequencyLabel2.setBounds(455, 30, 75, 50);
    
    resonanceSlider2.setBounds(655, getHeight()/4, 150, 150);
    resonanceLabel2.attachToComponent(&resonanceSlider2, true);
    
    TypeSelector.setBounds(160, getHeight()/8, 200, 30);
    TypeSelector2.setBounds(605, getHeight()/8, 200, 30);
    
}
