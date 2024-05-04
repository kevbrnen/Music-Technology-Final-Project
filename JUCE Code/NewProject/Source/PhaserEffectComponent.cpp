/*
  ==============================================================================

    PhaserEffectComponent.cpp
    Created: 30 Apr 2024 11:45:29am
    Author:  Kevin Brennan
 
    Component showing any UI for the Phaser Effect
  ==============================================================================
*/

#include <JuceHeader.h>
#include "PhaserEffectComponent.h"

//==============================================================================
PhaserEffectComponent::PhaserEffectComponent(juce::AudioProcessorValueTreeState& vts)
{
//Toggle
    Phaser_Toggle.setClickingTogglesState(true);
    Phaser_Toggle.setButtonText("OFF");
    Phaser_Toggle.setEnabled(true);
    Phaser_Toggle.setVisible(true);
    Phaser_Toggle.onClick = [this, &vts]()
    {
        if(Phaser_Toggle.getToggleState())
        {
            Phaser_Toggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
            Phaser_Toggle.setButtonText("ON");
            vts.state.setProperty("phaser_toggle", true, nullptr);
            
        }
        else
        {
            Phaser_Toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            Phaser_Toggle.setButtonText("OFF");
            vts.state.setProperty("phaser_toggle", false, nullptr);
        }
    };
    Phaser_OnOff_Attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(vts, "phaser_toggle", Phaser_Toggle));
    addAndMakeVisible(Phaser_Toggle);
    
    ToggleLabel.setText("Phaser Effect", juce::dontSendNotification);
    ToggleLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ToggleLabel);
    
//Gain Slider
    phaserGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "phaser_gain", phaserGainSlider));
    phaserGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    phaserGainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    phaserGainSlider.setTextValueSuffix("dB");
    phaserGainSlider.setRange(-48.0, 10.0);
    addAndMakeVisible(phaserGainSlider);
    
    phaserGainLabel.setText("Phaser Effect Gain", juce::dontSendNotification);
    phaserGainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(phaserGainLabel);
    
//Phaser Wet Dry Amount Slider
    PhaserWDSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    PhaserWDAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "phaser_wet_dry", PhaserWDSlider));
    PhaserWDSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    PhaserWDSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    PhaserWDSlider.setRange(0.0, 1.0);
    addAndMakeVisible(PhaserWDSlider);
    
    PhaserWDLabel.setText("Wet/Dry Amount", juce::dontSendNotification);
    PhaserWDLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(PhaserWDLabel);
    
//Phaser LFO speed
    PhaserSpeedSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    PhaserSpeedAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "phaser_lfo_speed", PhaserSpeedSlider));
    PhaserSpeedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    PhaserSpeedSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::darkorange);
    PhaserSpeedSlider.setTextValueSuffix("Hz");
    PhaserSpeedSlider.setRange(0.0, 10.0);
    addAndMakeVisible(PhaserSpeedSlider);
    
    PhaserSpeedLabel.setText("Modulation Speed", juce::dontSendNotification);
    PhaserSpeedLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(PhaserSpeedLabel);
    
//Phaser Intensity
    PhaserIntensitySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    PhaserIntensityAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "phaser_intensity", PhaserIntensitySlider));
    PhaserIntensitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    PhaserIntensitySlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::darkorange);
    PhaserIntensitySlider.setRange(1.0, 4.0, 1);
    addAndMakeVisible(PhaserIntensitySlider);
    
    PhaserIntensityLabel.setText("Intensity", juce::dontSendNotification);
    PhaserIntensityLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(PhaserIntensityLabel);
    
//Phaser Q
    PhaserQSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    PhaserQAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "phaser_q", PhaserQSlider));
    PhaserQSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    PhaserQSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::darkorange);
    PhaserQSlider.setRange(0.0, 10.0);
    addAndMakeVisible(PhaserQSlider);
    
    PhaserQLabel.setText("Q", juce::dontSendNotification);
    PhaserQLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(PhaserQLabel);
    
//Phaser Mod Width
    PhaserModWidthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    PhaserModWidthAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "phaser_mod_width", PhaserModWidthSlider));
    PhaserModWidthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    PhaserModWidthSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::darkorange);
    PhaserModWidthSlider.setRange(0.0, 1.0);
    addAndMakeVisible(PhaserModWidthSlider);
    
    PhaserModWidthLabel.setText("Modulation Width", juce::dontSendNotification);
    PhaserModWidthLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(PhaserModWidthLabel);
    
//Phaser Feedback
    PhaserFDBKSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    PhaserFDBKAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "phaser_fdbk", PhaserFDBKSlider));
    PhaserFDBKSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    PhaserFDBKSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::darkorange);
    PhaserFDBKSlider.setRange(0.0, 1.0);
    addAndMakeVisible(PhaserFDBKSlider);
    
    PhaserFDBKLabel.setText("Feedback Amount", juce::dontSendNotification);
    PhaserFDBKLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(PhaserFDBKLabel);



}

PhaserEffectComponent::~PhaserEffectComponent()
{
}

void PhaserEffectComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::orange);   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 5);   // draw an outline around the component

}

void PhaserEffectComponent::resized()
{
    Phaser_Toggle.setBounds(getWidth()-60, 20, 50, 25);
    ToggleLabel.attachToComponent(&Phaser_Toggle, true);

    phaserGainSlider.setBounds(getWidth() - 110, 60, 100, 100);
    phaserGainLabel.attachToComponent(&phaserGainSlider, true);
    
    PhaserWDSlider.setBounds(getWidth() - 110, 200, 100, 100);
    PhaserWDLabel.attachToComponent(&PhaserWDSlider, true);
    
    PhaserSpeedSlider.setBounds(350, (getHeight()/4), 150, 150);
    PhaserSpeedLabel.attachToComponent(&PhaserSpeedSlider, false);
    PhaserSpeedLabel.setJustificationType(juce::Justification::centred);
    
    PhaserIntensitySlider.setBounds(550, (getHeight()/4), 150, 150);
    PhaserIntensityLabel.attachToComponent(&PhaserIntensitySlider, false);
    PhaserIntensityLabel.setJustificationType(juce::Justification::centred);
    
    PhaserModWidthSlider.setBounds(100, (getHeight()/8) , 150, 150);
    PhaserModWidthLabel.attachToComponent(&PhaserModWidthSlider, false);
    PhaserModWidthLabel.setJustificationType(juce::Justification::centred);
    
    PhaserQSlider.setBounds(100, 3*(getHeight()/8)+100, 150, 150);
    PhaserQLabel.attachToComponent(&PhaserQSlider, false);
    PhaserQLabel.setJustificationType(juce::Justification::centred);
    
    PhaserFDBKSlider.setBounds(450, 3*(getHeight()/8)+150, 150, 150);
    PhaserFDBKLabel.attachToComponent(&PhaserFDBKSlider, false);
    PhaserFDBKLabel.setJustificationType(juce::Justification::centred);
}
