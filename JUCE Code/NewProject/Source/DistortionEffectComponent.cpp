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
    PreGainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(198, 88, 245));
    PreGainSlider.setRange(1, 10.0);
    addAndMakeVisible(PreGainSlider);
    PreGainLabel.setText("Gain Factor", juce::dontSendNotification);
    PreGainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(PreGainLabel);

//Threshold Slider
    ThreshSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    threshAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "distortion_thresh", ThreshSlider));
    ThreshSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    ThreshSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(198, 88, 245));
    ThreshSlider.setRange(0, 1);
    addAndMakeVisible(ThreshSlider);
    ThreshLabel.setText("Threshold", juce::dontSendNotification);
    ThreshLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ThreshLabel);
    
    
//Cutoff Sliders
    PreCutoffSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    preCutoffAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "distortion_pre_cutoff", PreCutoffSlider));
    PreCutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    PreCutoffSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    PreCutoffSlider.setTextValueSuffix("Hz");
    PreCutoffSlider.setRange(20.0, 20000.0);
    addAndMakeVisible(PreCutoffSlider);
    PreCutoffLabel.setText("Pre Filter Frequency", juce::dontSendNotification);
    addAndMakeVisible(PreCutoffLabel);
    
    PostCutoffSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    postCutoffAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "distortion_post_cutoff", PostCutoffSlider));
    PostCutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    PostCutoffSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    PostCutoffSlider.setTextValueSuffix("Hz");
    PostCutoffSlider.setRange(20.0, 20000.0);
    addAndMakeVisible(PostCutoffSlider);
    PostCutoffLabel.setText("Post Filter Frequency", juce::dontSendNotification);
    addAndMakeVisible(PostCutoffLabel);
    
//Resonance Sliders
    PreResSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    preResAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "distortion_pre_resonance", PreResSlider));
    PreResSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    PreResSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    PreResSlider.setRange(0.0, 10.0);
    addAndMakeVisible(PreResSlider);
    
    PreResLabel.setText("Resonance", juce::dontSendNotification);
    addAndMakeVisible(PreResLabel);
    
    PostResSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    postResAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "distortion_post_resonance", PostResSlider));
    PostResSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    PostResSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    PostResSlider.setRange(0.0, 10.0);
    addAndMakeVisible(PostResSlider);
    
    PostResLabel.setText("Resonance", juce::dontSendNotification);
    addAndMakeVisible(PostResLabel);
    
//Filter Type Selectors
    PreSelector.addItemList(Filter_Choices, 1);
    PreSelector.setSelectedId(1);
    preType_attachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(vts, "pre_filter_type", PreSelector));
    addAndMakeVisible(PreSelector);
    
    PostSelector.addItemList(Filter_Choices, 1);
    PostSelector.setSelectedId(1);
    postType_attachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(vts, "post_filter_type", PostSelector));
    addAndMakeVisible(PostSelector);
    

}

DistortionEffectComponent::~DistortionEffectComponent()
{
}

void DistortionEffectComponent::paint (juce::Graphics& g)
{
    
    g.fillAll (juce::Colour(192, 151, 209));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 5);   // draw an outline around the component
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
    
    PreGainSlider.setBounds((2*getWidth()/8)-30, 150, 150, 150);
    PreGainLabel.attachToComponent(&PreGainSlider, false);
    PreGainLabel.setJustificationType(juce::Justification::centred);
    
    ThreshSlider.setBounds((3*getWidth()/8)-20, 150, 150, 150);
    ThreshLabel.attachToComponent(&ThreshSlider, false);
    ThreshLabel.setJustificationType(juce::Justification::centred);
    
    
    PreCutoffSlider.setBounds(50, 50, 150, 150);
    PreCutoffLabel.attachToComponent(&PreCutoffSlider, false);
    PreCutoffLabel.setJustificationType(juce::Justification::centred);
    
    PreResSlider.setBounds(50, 250, 150, 150);
    PreResLabel.attachToComponent(&PreResSlider, false);
    PreResLabel.setJustificationType(juce::Justification::centred);
    
    PostCutoffSlider.setBounds((4*getWidth()/8)+50, 50, 150, 150);
    PostCutoffLabel.attachToComponent(&PostCutoffSlider, false);
    PostCutoffLabel.setJustificationType(juce::Justification::centred);
    
    PostResSlider.setBounds((4*getWidth()/8)+50, 250, 150, 150);
    PostResLabel.attachToComponent(&PostResSlider, false);
    PostResLabel.setJustificationType(juce::Justification::centred);
    
    TypeSelector.setBounds(getWidth()-275, getHeight()*2/4 - 75, 200, 30);
    
    PreSelector.setBounds(25, 450, 200, 30);
    
    PostSelector.setBounds((4*getWidth()/8)+25, 450, 200, 30);
}
