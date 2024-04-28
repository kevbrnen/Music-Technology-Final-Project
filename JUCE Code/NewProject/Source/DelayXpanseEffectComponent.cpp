/*
  ==============================================================================

    DelayXpanseEffectComponent.cpp
    Created: 18 Apr 2024 11:38:33pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DelayXpanseEffectComponent.h"

//==============================================================================
DelayXpanseEffectComponent::DelayXpanseEffectComponent(juce::AudioProcessorValueTreeState& vts)
{
//Toggle
    Xpanse_Toggle.setClickingTogglesState(true);
    Xpanse_Toggle.setButtonText("OFF");
    Xpanse_Toggle.setEnabled(true);
    Xpanse_Toggle.setVisible(true);
    Xpanse_Toggle.onClick = [this, &vts]()
    {
        if(Xpanse_Toggle.getToggleState())
        {
            Xpanse_Toggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
            Xpanse_Toggle.setButtonText("ON");
            vts.state.setProperty("xpanse_toggle", true, nullptr);
            
        }
        else
        {
            Xpanse_Toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            Xpanse_Toggle.setButtonText("OFF");
            vts.state.setProperty("xpanse_toggle", false, nullptr);
        }
    };
    Xpanse_OnOff_Attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(vts, "xpanse_toggle", Xpanse_Toggle));
    addAndMakeVisible(Xpanse_Toggle);
    
    ToggleLabel.setText("Delay-Xpanse Effect", juce::dontSendNotification);
    ToggleLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ToggleLabel);
    
//Convolution Reverb Wet Dry Amount Slider
    XpanseWDSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    XpanseWDAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "xpanse_wetdry", XpanseWDSlider));
    XpanseWDSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    XpanseWDSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    XpanseWDSlider.setRange(0.0, 1.0);
    addAndMakeVisible(XpanseWDSlider);
    
    XpanseWDLabel.setText("Wet/Dry Amount", juce::dontSendNotification);
    XpanseWDLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(XpanseWDLabel);
    
//Gain Slider
    XpanseGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "xpanse_gain", XpanseGainSlider));
    XpanseGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    XpanseGainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    XpanseGainSlider.setTextValueSuffix("dB");
    XpanseGainSlider.setRange(-48.0, 10.0);
    addAndMakeVisible(XpanseGainSlider);
    
    XpanseGainLabel.setText("Delay-Xpanse Effect Gain", juce::dontSendNotification);
    XpanseGainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(XpanseGainLabel);
    
    

//###### Ping Pong Delay #####
    pingPongDelaySlider_L.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    Delay_Attachment_L.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "pong_delay_L_time", pingPongDelaySlider_L));
    pingPongDelaySlider_L.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    pingPongDelaySlider_L.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    pingPongDelaySlider_L.setTextValueSuffix("ms");
    pingPongDelaySlider_L.setRange(0.0, 1999.0);
    addAndMakeVisible(pingPongDelaySlider_L);
    
    DelayLabel_L.setText("Left Delay Time", juce::dontSendNotification);
    DelayLabel_L.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DelayLabel_L);
    
    pingPongDelaySlider_R.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    Delay_Attachment_R.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "pong_delay_R_time", pingPongDelaySlider_R));
    pingPongDelaySlider_R.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    pingPongDelaySlider_R.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    pingPongDelaySlider_R.setTextValueSuffix("ms");
    pingPongDelaySlider_R.setRange(0.0, 1999.0);
    addAndMakeVisible(pingPongDelaySlider_R);
    
    DelayLabel_R.setText("Right Delay Time", juce::dontSendNotification);
    DelayLabel_R.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DelayLabel_R);
    
    
    pingPongFDBKSlider_L.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    FDBK_Attachment_L.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "pong_delay_L_fdbk", pingPongFDBKSlider_L));
    pingPongFDBKSlider_L.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    pingPongFDBKSlider_L.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    pingPongFDBKSlider_L.setRange(0.0, 0.999);
    addAndMakeVisible(pingPongFDBKSlider_L);
    
    FDBKLabel_L.setText("Left Feedback Amount", juce::dontSendNotification);
    FDBKLabel_L.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(FDBKLabel_L);
    
    pingPongFDBKSlider_R.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    FDBK_Attachment_R.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "pong_delay_R_fdbk", pingPongFDBKSlider_R));
    pingPongFDBKSlider_R.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    pingPongFDBKSlider_R.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    pingPongFDBKSlider_R.setRange(0.0, 0.999);
    addAndMakeVisible(pingPongFDBKSlider_R);
    
    FDBKLabel_R.setText("Right Feedback Amount", juce::dontSendNotification);
    FDBKLabel_R.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(FDBKLabel_R);
    
}

DelayXpanseEffectComponent::~DelayXpanseEffectComponent()
{
}

void DelayXpanseEffectComponent::paint (juce::Graphics& g)
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
    g.drawText ("DelayXpanseEffectComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void DelayXpanseEffectComponent::resized()
{
    Xpanse_Toggle.setBounds(getWidth()-60, 20, 50, 25);
    ToggleLabel.attachToComponent(&Xpanse_Toggle, true);
    
    XpanseWDSlider.setBounds(getWidth()-275, getHeight()*2/4, 250, 250);
    XpanseWDLabel.attachToComponent(&XpanseWDSlider, true);
    
    XpanseGainSlider.setBounds(getWidth() - 110, 60, 100, 100);
    XpanseGainLabel.attachToComponent(&XpanseGainSlider, true);
    
    
//Ping Pong Delay
    pingPongDelaySlider_L.setBounds(100, getHeight()/4, 150, 150);
    pingPongDelaySlider_R.setBounds(350, getHeight()/4, 150, 150);
    DelayLabel_L.attachToComponent(&pingPongDelaySlider_L, true);
    DelayLabel_R.attachToComponent(&pingPongDelaySlider_R, true);
    
    pingPongFDBKSlider_L.setBounds(100, 3*getHeight()/4, 100, 100);
    pingPongFDBKSlider_R.setBounds(350, 3*getHeight()/4, 100, 100);
    FDBKLabel_L.attachToComponent(&pingPongFDBKSlider_L, true);
    FDBKLabel_R.attachToComponent(&pingPongFDBKSlider_R, true);
    setPingPongComponents(false);
    
}

void DelayXpanseEffectComponent::setPingPongComponents(bool show)
{
    pingPongDelaySlider_L.setVisible(show);
    pingPongDelaySlider_L.setEnabled(show);
    pingPongDelaySlider_R.setVisible(show);
    pingPongDelaySlider_R.setEnabled(show);
    DelayLabel_L.setVisible(show);
    DelayLabel_R.setVisible(show);
    
    pingPongFDBKSlider_L.setVisible(show);
    pingPongFDBKSlider_L.setEnabled(show);
    pingPongFDBKSlider_R.setVisible(show);
    pingPongFDBKSlider_R.setEnabled(show);
    FDBKLabel_L.setVisible(show);
    FDBKLabel_R.setVisible(show);
}
