/*
  ==============================================================================

    DistortionEffectComponent.cpp
    Created: 2 May 2024 3:15:55pm
    Author:  Kevin Brennan

    Component showing any UI for the Distortion Effect
  ==============================================================================
*/

#include <JuceHeader.h>
#include "DistortionEffectComponent.h"

//==============================================================================
DistortionEffectComponent::DistortionEffectComponent(juce::AudioProcessorValueTreeState& vts)
{
//Toggle
    Distortion_Toggle.setClickingTogglesState(true);
    Distortion_Toggle.setButtonText("OFF");
    Distortion_Toggle.setEnabled(true);
    Distortion_Toggle.setVisible(true);
    Distortion_Toggle.onClick = [this, &vts]()
    {
        if(Distortion_Toggle.getToggleState())
        {
            Distortion_Toggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
            Distortion_Toggle.setButtonText("ON");
            vts.state.setProperty("distortion_toggle", true, nullptr);
            
        }
        else
        {
            Distortion_Toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            Distortion_Toggle.setButtonText("OFF");
            vts.state.setProperty("distortion_toggle", false, nullptr);
        }
    };
    Distortion_OnOff_Attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(vts, "distortion_toggle", Distortion_Toggle));
    addAndMakeVisible(Distortion_Toggle);
    ToggleLabel.setText("Distortion Effect", juce::dontSendNotification);
    ToggleLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ToggleLabel);
    
//Wet Dry Amount Slider
    DistortionWDSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    DistortionWDAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "distortion_wetdry", DistortionWDSlider));
    DistortionWDSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    DistortionWDSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    DistortionWDSlider.setRange(0.0, 1.0);
    addAndMakeVisible(DistortionWDSlider);
    DistortionWDLabel.setText("Wet/Dry Amount", juce::dontSendNotification);
    DistortionWDLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DistortionWDLabel);
    
//Gain Slider
    DistortionGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "distortion_gain", DistortionGainSlider));
    DistortionGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    DistortionGainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    DistortionGainSlider.setTextValueSuffix("dB");
    DistortionGainSlider.setRange(-48.0, 10.0);
    addAndMakeVisible(DistortionGainSlider);
    DistortionGainLabel.setText("Distortion Effect Gain", juce::dontSendNotification);
    DistortionGainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DistortionGainLabel);
    
//Combobox
    TypeSelector.addItemList(Distortion_Choices, 1);
    TypeSelector.setSelectedId(1);
    Type_attachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(vts, "distortion_type", TypeSelector));
    addAndMakeVisible(TypeSelector);
    
//Gain Slider
    PreGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    preGainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "distortion_pregain", PreGainSlider));
    PreGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    PreGainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    PreGainSlider.setRange(1, 10.0);
    addAndMakeVisible(PreGainSlider);
    PreGainLabel.setText("Gain Factor", juce::dontSendNotification);
    PreGainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(PreGainLabel);

//Gain Slider
    ThreshSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    threshAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "distortion_thresh", ThreshSlider));
    ThreshSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    ThreshSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    ThreshSlider.setRange(0, 1);
    addAndMakeVisible(ThreshSlider);
    ThreshLabel.setText("Threshold", juce::dontSendNotification);
    ThreshLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ThreshLabel);
}

DistortionEffectComponent::~DistortionEffectComponent()
{
}

void DistortionEffectComponent::paint (juce::Graphics& g)
{
    
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 5);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("DistortionEffectComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void DistortionEffectComponent::resized()
{
    Distortion_Toggle.setBounds(getWidth()-60, 20, 50, 25);
    ToggleLabel.attachToComponent(&Distortion_Toggle, true);
    
    DistortionWDSlider.setBounds(getWidth()-275, getHeight()*2/4, 250, 250);
    DistortionWDLabel.attachToComponent(&DistortionWDSlider, false);
    DistortionWDLabel.setJustificationType(juce::Justification::centred);
    
    DistortionGainSlider.setBounds(getWidth() - 110, 60, 100, 100);
    DistortionGainLabel.attachToComponent(&DistortionGainSlider, true);
    
    PreGainSlider.setBounds(100, 60, 150, 150);
    PreGainLabel.attachToComponent(&PreGainSlider, true);
    
    ThreshSlider.setBounds(100, 200, 150, 150);
    ThreshLabel.attachToComponent(&ThreshSlider, true);
    
    TypeSelector.setBounds(getWidth()-275, getHeight()*2/4 - 75, 200, 30);
}
