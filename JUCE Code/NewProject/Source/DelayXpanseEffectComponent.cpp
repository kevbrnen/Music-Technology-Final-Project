/*
  ==============================================================================

    DelayXpanseEffectComponent.cpp
    Created: 18 Apr 2024 11:38:33pm
    Author:  Kevin Brennan

    Component showing any UI for the Delay-Xpanse Effect
    Contains separate UI for ping-pong delay and spectral delay, but these components
    are hidden and disabled when not needed
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
    
//Wet Dry Amount Slider
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
    pingPongDelaySlider_L.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    pingPongDelaySlider_L.setTextValueSuffix("ms");
    pingPongDelaySlider_L.setRange(0.0, 1999.0);
    addAndMakeVisible(pingPongDelaySlider_L);
    DelayLabel_L.setText("Left Delay Time", juce::dontSendNotification);
    DelayLabel_L.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DelayLabel_L);

    pingPongDelaySlider_R.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    Delay_Attachment_R.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "pong_delay_R_time", pingPongDelaySlider_R));
    pingPongDelaySlider_R.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    pingPongDelaySlider_R.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    pingPongDelaySlider_R.setTextValueSuffix("ms");
    pingPongDelaySlider_R.setRange(0.0, 1999.0);
    addAndMakeVisible(pingPongDelaySlider_R);
    DelayLabel_R.setText("Right Delay Time", juce::dontSendNotification);
    DelayLabel_R.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DelayLabel_R);
    
    
    pingPongFDBKSlider_L.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    FDBK_Attachment_L.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "pong_delay_L_fdbk", pingPongFDBKSlider_L));
    pingPongFDBKSlider_L.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    pingPongFDBKSlider_L.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    pingPongFDBKSlider_L.setRange(0.0, 0.999);
    addAndMakeVisible(pingPongFDBKSlider_L);
    FDBKLabel_L.setText("Left Feedback Amount", juce::dontSendNotification);
    FDBKLabel_L.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(FDBKLabel_L);
    
    pingPongFDBKSlider_R.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    FDBK_Attachment_R.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "pong_delay_R_fdbk", pingPongFDBKSlider_R));
    pingPongFDBKSlider_R.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    pingPongFDBKSlider_R.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    pingPongFDBKSlider_R.setRange(0.0, 0.999);
    addAndMakeVisible(pingPongFDBKSlider_R);
    FDBKLabel_R.setText("Right Feedback Amount", juce::dontSendNotification);
    FDBKLabel_R.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(FDBKLabel_R);
    
    
//###### Spectral Delay #####
    Band1Gain.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    band1Gain_attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "spec_band1_gain", Band1Gain));
    Band1Gain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    Band1Gain.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    Band1Gain.setTextValueSuffix("dB");
    Band1Gain.setRange(-48.0, 10.0);
    addAndMakeVisible(Band1Gain);
    Band1GainLabel.setText("Band 1 Gain", juce::dontSendNotification);
    Band1GainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(Band1GainLabel);
    
    Band2Gain.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    band2Gain_attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "spec_band2_gain", Band2Gain));
    Band2Gain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    Band2Gain.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    Band2Gain.setTextValueSuffix("dB");
    Band2Gain.setRange(-48.0, 10.0);
    addAndMakeVisible(Band2Gain);
    Band2GainLabel.setText("Band 2 Gain", juce::dontSendNotification);
    Band2GainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(Band2GainLabel);
    
    Band3Gain.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    band3Gain_attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "spec_band3_gain", Band3Gain));
    Band3Gain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    Band3Gain.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    Band3Gain.setTextValueSuffix("dB");
    Band3Gain.setRange(-48.0, 10.0);
    addAndMakeVisible(Band3Gain);
    
    Band3GainLabel.setText("Band 3 Gain", juce::dontSendNotification);
    Band3GainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(Band3GainLabel);
    
    Band4Gain.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    band4Gain_attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "spec_band4_gain", Band4Gain));
    Band4Gain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    Band4Gain.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    Band4Gain.setTextValueSuffix("dB");
    Band4Gain.setRange(-48.0, 10.0);
    addAndMakeVisible(Band4Gain);
    
    Band4GainLabel.setText("Band 4 Gain", juce::dontSendNotification);
    Band4GainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(Band4GainLabel);
    
    
    Band1Time.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    band1Time_attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "spec_band1_time", Band1Time));
    Band1Time.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    Band1Time.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    Band1Time.setTextValueSuffix("ms");
    Band1Time.setRange(0.0, 1499.0);
    addAndMakeVisible(Band1Time);
    Band1TimeLabel.setText("Band 1 time", juce::dontSendNotification);
    Band1TimeLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(Band1TimeLabel);
    
    Band2Time.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    band2Time_attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "spec_band2_time", Band2Time));
    Band2Time.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    Band2Time.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    Band2Time.setTextValueSuffix("ms");
    Band2Time.setRange(0.0, 1499.0);
    addAndMakeVisible(Band2Time);
    Band2TimeLabel.setText("Band 2 time", juce::dontSendNotification);
    Band2TimeLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(Band2TimeLabel);
    
    Band3Time.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    band3Time_attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "spec_band3_time", Band3Time));
    Band3Time.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    Band3Time.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    Band3Time.setTextValueSuffix("ms");
    Band3Time.setRange(0.0, 1499.0);
    addAndMakeVisible(Band3Time);
    Band3TimeLabel.setText("Band 3 time", juce::dontSendNotification);
    Band3TimeLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(Band3TimeLabel);
    
    Band4Time.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    band4Time_attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "spec_band4_time", Band4Time));
    Band4Time.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    Band4Time.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    Band4Time.setTextValueSuffix("ms");
    Band4Time.setRange(0.0, 1499.0);
    addAndMakeVisible(Band4Time);
    Band4TimeLabel.setText("Band 4 time", juce::dontSendNotification);
    Band4TimeLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(Band4TimeLabel);
    
    
    Band1FDBK.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    band1FDBK_attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "spec_band1_fdbk", Band1FDBK));
    Band1FDBK.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    Band1FDBK.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    Band1FDBK.setRange(0.0, 0.999);
    addAndMakeVisible(Band1FDBK);
    Band1FDBKLabel.setText("Band 1 Feedback", juce::dontSendNotification);
    Band1FDBKLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(Band1FDBKLabel);
    
    Band2FDBK.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    band2FDBK_attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "spec_band2_fdbk", Band2FDBK));
    Band2FDBK.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    Band2FDBK.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    Band2FDBK.setRange(0.0, 0.999);
    addAndMakeVisible(Band2FDBK);
    Band2FDBKLabel.setText("Band 2 Feedback", juce::dontSendNotification);
    Band2FDBKLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(Band2FDBKLabel);
    
    Band3FDBK.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    band3FDBK_attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "spec_band3_fdbk", Band3FDBK));
    Band3FDBK.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    Band3FDBK.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    Band3FDBK.setRange(0.0, 0.999);
    addAndMakeVisible(Band3FDBK);
    Band3FDBKLabel.setText("Band 3 Feedback", juce::dontSendNotification);
    Band3FDBKLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(Band3FDBKLabel);
    
    Band4FDBK.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    band4FDBK_attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "spec_band4_fdbk", Band4FDBK));
    Band4FDBK.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    Band4FDBK.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    Band4FDBK.setRange(0.0, 0.999);
    addAndMakeVisible(Band4FDBK);
    Band4FDBKLabel.setText("Band 4 Feedback", juce::dontSendNotification);
    Band4FDBKLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(Band4FDBKLabel);
    
    
    specCutoff1.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    specCutoff1_attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "spec_cutoff1", specCutoff1));
    specCutoff1.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    specCutoff1.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    specCutoff1.setTextValueSuffix("Hz");
    specCutoff1.setRange(20.0, 20000.0);
    addAndMakeVisible(specCutoff1);
    specCutoff1Label.setText("Cutoff Frequency 1", juce::dontSendNotification);
    addAndMakeVisible(specCutoff1Label);
    
    specCutoff2.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    specCutoff2_attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "spec_cutoff2", specCutoff2));
    specCutoff2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    specCutoff2.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    specCutoff2.setTextValueSuffix("Hz");
    specCutoff2.setRange(20.0, 20000.0);
    addAndMakeVisible(specCutoff2);
    specCutoff2Label.setText("Cutoff Frequency 2", juce::dontSendNotification);
    addAndMakeVisible(specCutoff2Label);
    
    specCutoff3.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    specCutoff3_attachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "spec_cutoff3", specCutoff3));
    specCutoff3.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    specCutoff3.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(229, 83, 110));
    specCutoff3.setTextValueSuffix("Hz");
    specCutoff3.setRange(20.0, 20000.0);
    addAndMakeVisible(specCutoff3);
    specCutoff3Label.setText("Cutoff Frequency 3", juce::dontSendNotification);
    addAndMakeVisible(specCutoff3Label);
    
    
    setPingPongComponents(false);
    setSpectralComponents(false);
    
    
//Combobox
    TypeSelector.addItemList(Delay_Choices, 1);
    TypeSelector.setSelectedId(1);
    Type_attachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(vts, "xpanse_type", TypeSelector));
    addAndMakeVisible(TypeSelector);
    TypeSelector.onChange = [this, &vts]()
    {
        //Show/hide spectral/ping-pong components depending on which is selected
            if(TypeSelector.getSelectedId() == 1)
            {
                setSpectralComponents(false);
                setPingPongComponents(true);
            }
            else if(TypeSelector.getSelectedId() == 2)
            {
                setPingPongComponents(false);
                setSpectralComponents(true);
            }
    };
    
    
}

DelayXpanseEffectComponent::~DelayXpanseEffectComponent()
{
}

void DelayXpanseEffectComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour::fromHSV (0.0f, 0.5f, 0.7f, 1.0f));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 5);   // draw an outline around the component

}

void DelayXpanseEffectComponent::resized()
{
    Xpanse_Toggle.setBounds(getWidth()-60, 20, 50, 25);
    ToggleLabel.attachToComponent(&Xpanse_Toggle, true);
    
    XpanseWDSlider.setBounds(getWidth()-275, getHeight()*2/4, 250, 250);
    XpanseWDLabel.attachToComponent(&XpanseWDSlider, false);
    XpanseWDLabel.setJustificationType(juce::Justification::centred);
    
    XpanseGainSlider.setBounds(getWidth() - 110, 60, 100, 100);
    XpanseGainLabel.attachToComponent(&XpanseGainSlider, true);
    
    TypeSelector.setBounds(getWidth()-275, getHeight()*2/4 - 75, 200, 30);
    
    
//Ping Pong Delay
    pingPongDelaySlider_L.setBounds(100, 50, 200, 200);
    pingPongDelaySlider_R.setBounds(400, 50, 200, 200);
    DelayLabel_L.attachToComponent(&pingPongDelaySlider_L, false);
    DelayLabel_L.setJustificationType(juce::Justification::centred);
    DelayLabel_R.attachToComponent(&pingPongDelaySlider_R, false);
    DelayLabel_R.setJustificationType(juce::Justification::centred);
    
    pingPongFDBKSlider_L.setBounds(125, (3*getHeight()/4)-50, 150, 150);
    pingPongFDBKSlider_R.setBounds(425, (3*getHeight()/4)-50, 150, 150);
    FDBKLabel_L.attachToComponent(&pingPongFDBKSlider_L, false);
    FDBKLabel_L.setJustificationType(juce::Justification::centred);
    FDBKLabel_R.attachToComponent(&pingPongFDBKSlider_R, false);
    FDBKLabel_R.setJustificationType(juce::Justification::centred);
    
    
//Spectral Delay
    specCutoff1.setBounds(100, 10, 150, 150);
    specCutoff2.setBounds(100, getHeight()/3 + 10, 150, 150);
    specCutoff3.setBounds(100, 2*getHeight()/3 + 10, 150, 150);
    specCutoff1Label.attachToComponent(&specCutoff1, true);
    specCutoff2Label.attachToComponent(&specCutoff2, true);
    specCutoff3Label.attachToComponent(&specCutoff3, true);
    
    Band1Gain.setBounds(300, 25, 100, 100);
    Band2Gain.setBounds(300, getHeight()/4 + 20, 100, 100);
    Band3Gain.setBounds(300, 2*getHeight()/4 + 20, 100, 100);
    Band4Gain.setBounds(300, 3*getHeight()/4 + 20, 100, 100);
    Band1GainLabel.attachToComponent(&Band1Gain, false);
    Band2GainLabel.attachToComponent(&Band2Gain, false);
    Band3GainLabel.attachToComponent(&Band3Gain, false);
    Band4GainLabel.attachToComponent(&Band4Gain, false);
    Band1GainLabel.setJustificationType(juce::Justification::centred);
    Band2GainLabel.setJustificationType(juce::Justification::centred);
    Band3GainLabel.setJustificationType(juce::Justification::centred);
    Band4GainLabel.setJustificationType(juce::Justification::centred);
    
    Band1Time.setBounds(450, 25, 100, 100);
    Band2Time.setBounds(450, getHeight()/4 + 20, 100, 100);
    Band3Time.setBounds(450, 2*getHeight()/4 + 20, 100, 100);
    Band4Time.setBounds(450, 3*getHeight()/4 + 20, 100, 100);
    Band1TimeLabel.attachToComponent(&Band1Time, false);
    Band2TimeLabel.attachToComponent(&Band2Time, false);
    Band3TimeLabel.attachToComponent(&Band3Time, false);
    Band4TimeLabel.attachToComponent(&Band4Time, false);
    Band1TimeLabel.setJustificationType(juce::Justification::centred);
    Band2TimeLabel.setJustificationType(juce::Justification::centred);
    Band3TimeLabel.setJustificationType(juce::Justification::centred);
    Band4TimeLabel.setJustificationType(juce::Justification::centred);
    
    Band1FDBK.setBounds(600, 25, 100, 100);
    Band2FDBK.setBounds(600, getHeight()/4 + 20, 100, 100);
    Band3FDBK.setBounds(600, 2*getHeight()/4 + 20, 100, 100);
    Band4FDBK.setBounds(600, 3*getHeight()/4 + 20, 100, 100);
    Band1FDBKLabel.attachToComponent(&Band1FDBK, false);
    Band2FDBKLabel.attachToComponent(&Band2FDBK, false);
    Band3FDBKLabel.attachToComponent(&Band3FDBK, false);
    Band4FDBKLabel.attachToComponent(&Band4FDBK, false);
    Band1FDBKLabel.setJustificationType(juce::Justification::centred);
    Band2FDBKLabel.setJustificationType(juce::Justification::centred);
    Band3FDBKLabel.setJustificationType(juce::Justification::centred);
    Band4FDBKLabel.setJustificationType(juce::Justification::centred);
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

void DelayXpanseEffectComponent::setSpectralComponents(bool show)
{
    Band1Gain.setVisible(show);
    Band1Gain.setEnabled(show);
    Band2Gain.setVisible(show);
    Band2Gain.setEnabled(show);
    Band3Gain.setVisible(show);
    Band3Gain.setEnabled(show);
    Band4Gain.setVisible(show);
    Band4Gain.setEnabled(show);
    Band1GainLabel.setVisible(show);
    Band2GainLabel.setVisible(show);
    Band3GainLabel.setVisible(show);
    Band4GainLabel.setVisible(show);
    
    Band1Time.setVisible(show);
    Band1Time.setEnabled(show);
    Band2Time.setVisible(show);
    Band2Time.setEnabled(show);
    Band3Time.setVisible(show);
    Band3Time.setEnabled(show);
    Band4Time.setVisible(show);
    Band4Time.setEnabled(show);
    Band1TimeLabel.setVisible(show);
    Band2TimeLabel.setVisible(show);
    Band3TimeLabel.setVisible(show);
    Band4TimeLabel.setVisible(show);
    
    Band1FDBK.setVisible(show);
    Band1FDBK.setEnabled(show);
    Band2FDBK.setVisible(show);
    Band2FDBK.setEnabled(show);
    Band3FDBK.setVisible(show);
    Band3FDBK.setEnabled(show);
    Band4FDBK.setVisible(show);
    Band4FDBK.setEnabled(show);
    Band1FDBKLabel.setVisible(show);
    Band2FDBKLabel.setVisible(show);
    Band3FDBKLabel.setVisible(show);
    Band4FDBKLabel.setVisible(show);
    
    specCutoff1.setVisible(show);
    specCutoff1.setEnabled(show);
    specCutoff2.setVisible(show);
    specCutoff2.setEnabled(show);
    specCutoff3.setVisible(show);
    specCutoff3.setEnabled(show);
    specCutoff1Label.setVisible(show);
    specCutoff2Label.setVisible(show);
    specCutoff3Label.setVisible(show);
}

