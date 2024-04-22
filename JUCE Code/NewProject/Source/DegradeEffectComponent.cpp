/*
  ==============================================================================

    DegradeEffectComponent.cpp
    Created: 20 Apr 2024 5:10:22pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DegradeEffectComponent.h"

//==============================================================================
DegradeEffectComponent::DegradeEffectComponent(juce::AudioProcessorValueTreeState& vts)
{
//Toggle
    Degrade_Toggle.setClickingTogglesState(true);
    Degrade_Toggle.setButtonText("OFF");
    Degrade_Toggle.setEnabled(true);
    Degrade_Toggle.setVisible(true);
    Degrade_Toggle.onClick = [this, &vts]()
    {
        if(Degrade_Toggle.getToggleState())
        {
            Degrade_Toggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
            Degrade_Toggle.setButtonText("ON");
            vts.state.setProperty("degrade_toggle", true, nullptr);
            
        }
        else
        {
            Degrade_Toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            Degrade_Toggle.setButtonText("OFF");
            vts.state.setProperty("degrade_toggle", false, nullptr);
        }
    };
    Degrade_OnOff_Attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(vts, "degrade_toggle", Degrade_Toggle));
    addAndMakeVisible(Degrade_Toggle);
    
    ToggleLabel.setText("Degrade Effect", juce::dontSendNotification);
    ToggleLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ToggleLabel);
    
//Convolution Reverb Wet Dry Amount Slider
    DegradeWDSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    DegradeWDAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "degrade_wetdry", DegradeWDSlider));
    DegradeWDSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    DegradeWDSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    DegradeWDSlider.setRange(0.0, 1.0);
    addAndMakeVisible(DegradeWDSlider);
    
    DegradeWDLabel.setText("Wet/Dry Amount", juce::dontSendNotification);
    DegradeWDLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DegradeWDLabel);
    
//Gain Slider
    DegradeGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "degrade_gain", DegradeGainSlider));
    DegradeGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    DegradeGainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    DegradeGainSlider.setTextValueSuffix("dB");
    DegradeGainSlider.setRange(-48.0, 10.0);
    addAndMakeVisible(DegradeGainSlider);
    
    DegradeGainLabel.setText("Degrade Effect Gain", juce::dontSendNotification);
    DegradeGainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DegradeGainLabel);
    
//Pre Filter Cutoff frequency
    DegradePRECutoff.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    PRECutoffAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "pre_cutoff_frequency", DegradePRECutoff));
    DegradePRECutoff.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    DegradePRECutoff.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    DegradePRECutoff.setTextValueSuffix("Hz");
    DegradePRECutoff.setRange(20.0, 20000.0);
    addAndMakeVisible(DegradePRECutoff);
    
    DegradePRECutoffLabel.setText("Pre-filter Frequency", juce::dontSendNotification);
    DegradePRECutoffLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DegradePRECutoffLabel);
    
//Post Filter Cutoff frequency
    DegradePOSTCutoff.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    POSTCutoffAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "post_cutoff_frequency", DegradePOSTCutoff));
    DegradePOSTCutoff.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    DegradePOSTCutoff.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    DegradePOSTCutoff.setTextValueSuffix("Hz");
    DegradePOSTCutoff.setRange(20.0, 20000.0);
    addAndMakeVisible(DegradePOSTCutoff);
    
    DegradePOSTCutoffLabel.setText("Post-filter Frequency", juce::dontSendNotification);
    DegradePOSTCutoffLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DegradePOSTCutoffLabel);
    
//Degrade frequency
    DegradeFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    FrequencyAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "degrade_frequency", DegradeFrequencySlider));
    DegradeFrequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    DegradeFrequencySlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    DegradeFrequencySlider.setTextValueSuffix("Hz");
    DegradeFrequencySlider.setRange(10.0, 48000.0);
    addAndMakeVisible(DegradeFrequencySlider);
    
    DegradeFrequencyLabel.setText("Degrade Frequency", juce::dontSendNotification);
    DegradeFrequencyLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DegradeFrequencyLabel);

}

DegradeEffectComponent::~DegradeEffectComponent()
{
}

void DegradeEffectComponent::paint (juce::Graphics& g)
{

    g.fillAll (juce::Colours::antiquewhite);   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 5);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("DegradeEffectComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void DegradeEffectComponent::resized()
{
    Degrade_Toggle.setBounds(getWidth()-60, 20, 50, 25);
    ToggleLabel.attachToComponent(&Degrade_Toggle, true);
    
    DegradeWDSlider.setBounds(getWidth() - 110, 200, 100, 100);
    DegradeWDLabel.attachToComponent(&DegradeWDSlider, true);
    
    DegradeGainSlider.setBounds(getWidth() - 110, 60, 100, 100);
    DegradeGainLabel.attachToComponent(&DegradeGainSlider, true);
    
    DegradePRECutoff.setBounds(getWidth()/2, getHeight()/6, 150, 150);
    DegradePRECutoffLabel.attachToComponent(&DegradePRECutoff, true);
    
    DegradePOSTCutoff.setBounds(getWidth()/2, 4*(getHeight()/6), 150, 150);
    DegradePOSTCutoffLabel.attachToComponent(&DegradePOSTCutoff, true);
    
    DegradeFrequencySlider.setBounds(100, getHeight()/4, 250, 250);
    DegradeFrequencyLabel.attachToComponent(&DegradeFrequencySlider, true);

}
