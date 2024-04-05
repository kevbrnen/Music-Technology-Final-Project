/*
  ==============================================================================

    DelayEffectComponent.cpp
    Created: 5 Apr 2024 10:54:36am
    Author:  Kevin Brennan

    Component showing any UI for the Delay Effect
  ==============================================================================
*/

#include <JuceHeader.h>
#include "DelayEffectComponent.h"

//==============================================================================
DelayEffectComponent::DelayEffectComponent(juce::AudioProcessorValueTreeState& vts)
{
//Toggle
    Delay_Toggle.setClickingTogglesState(true);
    Delay_Toggle.setButtonText("OFF");
    Delay_Toggle.setEnabled(true);
    Delay_Toggle.setVisible(true);
    Delay_Toggle.onClick = [this, &vts]()
    {
        if(Delay_Toggle.getToggleState())
        {
            Delay_Toggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
            Delay_Toggle.setButtonText("ON");
            vts.state.setProperty("delay_toggle", true, nullptr);
            
        }
        else
        {
            Delay_Toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            Delay_Toggle.setButtonText("OFF");
            vts.state.setProperty("delay_toggle", false, nullptr);
        }
    };
    Delay_OnOff_Attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(vts, "delay_toggle", Delay_Toggle));
    addAndMakeVisible(Delay_Toggle);
    
    
//Delay Time Slider
    DelayTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    DelayTimeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "delay_time", DelayTimeSlider));
    DelayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    DelayTimeSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    DelayTimeSlider.setTextValueSuffix("ms");
    DelayTimeSlider.setRange(0.0, 2000.0);
    addAndMakeVisible(DelayTimeSlider);
    
    
    DelayTimeLabel.setText("Delay Time", juce::dontSendNotification);
    DelayTimeLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DelayTimeLabel);
    
    
    setSize(150,300);
}

DelayEffectComponent::~DelayEffectComponent()
{
}

void DelayEffectComponent::paint (juce::Graphics& g)
{

    g.fillAll(juce::Colours::orange);   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 5);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("DelayEffectComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void DelayEffectComponent::resized()
{
    Delay_Toggle.setBounds(getWidth()-60, 20, 50, 25);

    DelayTimeSlider.setBounds(100, 50, 250, 250);
    DelayTimeLabel.attachToComponent(&DelayTimeSlider, true);
}
