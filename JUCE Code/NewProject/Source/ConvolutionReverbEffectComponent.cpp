/*
  ==============================================================================

    ConvolutionReverbEffectComponent.cpp
    Created: 15 Apr 2024 10:31:41am
    Author:  Kevin Brennan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ConvolutionReverbEffectComponent.h"

//==============================================================================
ConvolutionReverbEffectComponent::ConvolutionReverbEffectComponent(juce::AudioProcessorValueTreeState& vts)
{
//Toggle
    Convolution_Toggle.setClickingTogglesState(true);
    Convolution_Toggle.setButtonText("OFF");
    Convolution_Toggle.setEnabled(true);
    Convolution_Toggle.setVisible(true);
    Convolution_Toggle.onClick = [this, &vts]()
    {
        if(Convolution_Toggle.getToggleState())
        {
            Convolution_Toggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
            Convolution_Toggle.setButtonText("ON");
            vts.state.setProperty("convolution_toggle", true, nullptr);
            
        }
        else
        {
            Convolution_Toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            Convolution_Toggle.setButtonText("OFF");
            vts.state.setProperty("convolution_toggle", false, nullptr);
        }
    };
    Convolution_OnOff_Attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(vts, "convolution_toggle", Convolution_Toggle));
    addAndMakeVisible(Convolution_Toggle);
    
    ToggleLabel.setText("Convolution Reverb Effect", juce::dontSendNotification);
    ToggleLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ToggleLabel);
    
//Convolution Reverb Wet Dry Amount Slider
    ConvolutionWDSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    ConvolutionWDAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "convolution_wetdry", ConvolutionWDSlider));
    ConvolutionWDSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    ConvolutionWDSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    ConvolutionWDSlider.setRange(0.0, 1.0);
    addAndMakeVisible(ConvolutionWDSlider);
    
    ConvolutionWDLabel.setText("Wet/Dry Amount", juce::dontSendNotification);
    ConvolutionWDLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ConvolutionWDLabel);

}

ConvolutionReverbEffectComponent::~ConvolutionReverbEffectComponent()
{
}

void ConvolutionReverbEffectComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::lightgreen);   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 5);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("ConvolutionReverbEffectComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void ConvolutionReverbEffectComponent::resized()
{
    Convolution_Toggle.setBounds(getWidth()-60, 20, 50, 25);
    ToggleLabel.attachToComponent(&Convolution_Toggle, true);
    
    ConvolutionWDSlider.setBounds(100, getHeight()/4, 250, 250);
    ConvolutionWDLabel.attachToComponent(&ConvolutionWDSlider, true);
}
