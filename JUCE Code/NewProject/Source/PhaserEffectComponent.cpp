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
    PhaserSpeedSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    PhaserSpeedSlider.setTextValueSuffix("Hz");
    PhaserSpeedSlider.setRange(0.0, 10.0);
    addAndMakeVisible(PhaserSpeedSlider);
    
    PhaserSpeedLabel.setText("Speed", juce::dontSendNotification);
    PhaserSpeedLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(PhaserSpeedLabel);
    
//Phaser Intensity
    PhaserIntensitySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    PhaserIntensityAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "phaser_intensity", PhaserIntensitySlider));
    PhaserIntensitySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    PhaserIntensitySlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    PhaserIntensitySlider.setRange(1.0, 4.0, 1);
    addAndMakeVisible(PhaserIntensitySlider);
    
    PhaserIntensityLabel.setText("Intensity", juce::dontSendNotification);
    PhaserIntensityLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(PhaserIntensityLabel);


}

PhaserEffectComponent::~PhaserEffectComponent()
{
}

void PhaserEffectComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PhaserEffectComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PhaserEffectComponent::resized()
{
    Phaser_Toggle.setBounds(getWidth()-60, 20, 50, 25);
    ToggleLabel.attachToComponent(&Phaser_Toggle, true);

    phaserGainSlider.setBounds(getWidth() - 110, 60, 100, 100);
    phaserGainLabel.attachToComponent(&phaserGainSlider, true);
    
    PhaserWDSlider.setBounds(getWidth() - 110, 200, 100, 100);
    PhaserWDLabel.attachToComponent(&PhaserWDSlider, true);
    
    PhaserSpeedSlider.setBounds(50, getHeight()/4, 150, 150);
    PhaserSpeedLabel.attachToComponent(&PhaserSpeedSlider, true);
    
    PhaserIntensitySlider.setBounds(300, getHeight()/4, 150, 150);
    PhaserIntensityLabel.attachToComponent(&PhaserIntensitySlider, true);
}
