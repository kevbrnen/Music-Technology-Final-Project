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
    
    ToggleLabel.setText("Delay Effect", juce::dontSendNotification);
    ToggleLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ToggleLabel);

//LFO Toggle
    Delay_LFO_Toggle.setClickingTogglesState(true);
    Delay_LFO_Toggle.setButtonText("OFF");
    Delay_LFO_Toggle.setEnabled(true);
    Delay_LFO_Toggle.setVisible(true);
    Delay_LFO_Toggle.onClick = [this, &vts]()
    {
        if(Delay_LFO_Toggle.getToggleState())
        {
            Delay_LFO_Toggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
            Delay_LFO_Toggle.setButtonText("ON");
            vts.state.setProperty("delay_LFO_toggle", true, nullptr);
            
        }
        else
        {
            Delay_LFO_Toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            Delay_LFO_Toggle.setButtonText("OFF");
            vts.state.setProperty("delay_LFO_toggle", false, nullptr);
        }
    };
    Delay_LFO_Toggle_Attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(vts, "delay_LFO_toggle", Delay_LFO_Toggle));
    addAndMakeVisible(Delay_LFO_Toggle);
    
    LFOLabel.setText("LFO", juce::dontSendNotification);
    LFOLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(LFOLabel);
    
//LFO Display
    addAndMakeVisible(LFOThumb);
    
    
//Delay Time Slider
    DelayTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    DelayTimeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "delay_time", DelayTimeSlider));
    DelayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    DelayTimeSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    DelayTimeSlider.setTextValueSuffix("ms");
    DelayTimeSlider.setRange(0.0, 2999.0);
    addAndMakeVisible(DelayTimeSlider);
    
    
    DelayTimeLabel.setText("Delay Time", juce::dontSendNotification);
    DelayTimeLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DelayTimeLabel);
    
    
//Delay Feedback amount slider
    DelayFeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    DelayFeedbackAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "delay_fdbk", DelayFeedbackSlider));
    DelayFeedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    DelayFeedbackSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    DelayFeedbackSlider.setRange(0.0, 0.999);
    addAndMakeVisible(DelayFeedbackSlider);
    
    DelayFeedbackLabel.setText("Feedback Amount", juce::dontSendNotification);
    DelayFeedbackLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DelayFeedbackLabel);
    

//Delay Wet Dry amount slider
    DelayWDSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    DelayWDAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "delay_wetdry", DelayWDSlider));
    DelayWDSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    DelayWDSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    DelayWDSlider.setRange(0.0, 1.0);
    addAndMakeVisible(DelayWDSlider);
    
    DelayWDLabel.setText("Wet/Dry Amount", juce::dontSendNotification);
    DelayWDLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DelayWDLabel);
    
    
//Gain Slider
    delayGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "delay_gain", delayGainSlider));
    delayGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    delayGainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    delayGainSlider.setTextValueSuffix("dB");
    delayGainSlider.setRange(-48.0, 10.0);
    addAndMakeVisible(delayGainSlider);
    
    delayGainLabel.setText("Delay Effect Gain", juce::dontSendNotification);
    delayGainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(delayGainLabel);
    

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
    ToggleLabel.attachToComponent(&Delay_Toggle, true);
    
    Delay_LFO_Toggle.setBounds(getWidth()-60, 250, 50, 25);
    LFOLabel.attachToComponent(&Delay_LFO_Toggle, true);
    
    delayGainSlider.setBounds(getWidth() - 110, 60, 100, 100);
    delayGainLabel.attachToComponent(&delayGainSlider, true);

    DelayTimeSlider.setBounds(100, getHeight()/4, 250, 250);
    DelayTimeLabel.attachToComponent(&DelayTimeSlider, true);
    
    DelayFeedbackSlider.setBounds(getWidth()/2, getHeight()/6, 150, 150);
    DelayFeedbackLabel.attachToComponent(&DelayFeedbackSlider, false);
    
    DelayWDSlider.setBounds(getWidth()/2, 4*(getHeight()/6), 150, 150);
    DelayWDLabel.attachToComponent(&DelayWDSlider, false);
    
    LFOThumb.setBounds(getWidth()-200, 300, 190, 75);
}
