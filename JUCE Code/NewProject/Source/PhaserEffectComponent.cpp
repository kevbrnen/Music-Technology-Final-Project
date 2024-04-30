/*
  ==============================================================================

    PhaserEffectComponent.cpp
    Created: 30 Apr 2024 11:45:29am
    Author:  Kevin Brennan

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


}

PhaserEffectComponent::~PhaserEffectComponent()
{
}

void PhaserEffectComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

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
}
