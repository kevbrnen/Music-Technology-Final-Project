/*
  ==============================================================================

    ReverbEffectComponent.cpp
    Created: 1 May 2024 2:23:18pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ReverbEffectComponent.h"

//==============================================================================
ReverbEffectComponent::ReverbEffectComponent(juce::AudioProcessorValueTreeState& vts)
{
//Toggle
    Reverb_Toggle.setClickingTogglesState(true);
    Reverb_Toggle.setButtonText("OFF");
    Reverb_Toggle.setEnabled(true);
    Reverb_Toggle.setVisible(true);
    Reverb_Toggle.onClick = [this, &vts]()
    {
        if(Reverb_Toggle.getToggleState())
        {
            Reverb_Toggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
            Reverb_Toggle.setButtonText("ON");
            vts.state.setProperty("reverb_toggle", true, nullptr);
            
        }
        else
        {
            Reverb_Toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            Reverb_Toggle.setButtonText("OFF");
            vts.state.setProperty("reverb_toggle", false, nullptr);
        }
    };
    Reverb_OnOff_Attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(vts, "reverb_toggle", Reverb_Toggle));
    addAndMakeVisible(Reverb_Toggle);
    ToggleLabel.setText("Reverb Effect", juce::dontSendNotification);
    ToggleLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ToggleLabel);
    
//Wet Dry Amount Slider
    ReverbWDSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    ReverbWDAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "reverb_wetdry", ReverbWDSlider));
    ReverbWDSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    ReverbWDSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    ReverbWDSlider.setRange(0.0, 1.0);
    addAndMakeVisible(ReverbWDSlider);
    ReverbWDLabel.setText("Wet/Dry Amount", juce::dontSendNotification);
    ReverbWDLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ReverbWDLabel);
    
//Gain Slider
    ReverbGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "reverb_gain", ReverbGainSlider));
    ReverbGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    ReverbGainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    ReverbGainSlider.setTextValueSuffix("dB");
    ReverbGainSlider.setRange(-48.0, 10.0);
    addAndMakeVisible(ReverbGainSlider);
    ReverbGainLabel.setText("Reverb Effect Gain", juce::dontSendNotification);
    ReverbGainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ReverbGainLabel);
    
//Combobox
    TypeSelector.addItemList(Reverb_Choices, 1);
    TypeSelector.setSelectedId(1);
    Type_attachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(vts, "reverb_type", TypeSelector));
    addAndMakeVisible(TypeSelector);
    TypeSelector.onChange = [this, &vts]()
    {
            if(TypeSelector.getSelectedId() == 1)
            {
                setCombComponents(false);
                setSchroederComponents(true);
            }
            else if(TypeSelector.getSelectedId() == 2)
            {
                setSchroederComponents(false);
                setCombComponents(true);
            }
    };


}

ReverbEffectComponent::~ReverbEffectComponent()
{
}

void ReverbEffectComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 5);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("ReverbEffectComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void ReverbEffectComponent::resized()
{
    Reverb_Toggle.setBounds(getWidth()-60, 20, 50, 25);
    ToggleLabel.attachToComponent(&Reverb_Toggle, true);
    
    ReverbWDSlider.setBounds(getWidth()-275, getHeight()*2/4, 250, 250);
    ReverbWDLabel.attachToComponent(&ReverbWDSlider, false);
    ReverbWDLabel.setJustificationType(juce::Justification::centred);
    
    ReverbGainSlider.setBounds(getWidth() - 110, 60, 100, 100);
    ReverbGainLabel.attachToComponent(&ReverbGainSlider, true);
    
    TypeSelector.setBounds(getWidth()-275, getHeight()*2/4 - 75, 200, 30);

}

void ReverbEffectComponent::setSchroederComponents(bool show)
{
    
}

void ReverbEffectComponent::setCombComponents(bool show)
{
    
}
