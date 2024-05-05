/*
  ==============================================================================

    ReverbEffectComponent.cpp
    Created: 1 May 2024 2:23:18pm
    Author:  Kevin Brennan

    Component showing any UI for the Reverb Effect
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
                setDAPComponents(false);
                setFDNComponents(false);
                setSchroederComponents(true);
            }
            else if(TypeSelector.getSelectedId() == 2)
            {
                setSchroederComponents(false);
                setFDNComponents(false);
                setDAPComponents(true);
            }
            else if(TypeSelector.getSelectedId() == 3)
            {
                setSchroederComponents(false);
                setDAPComponents(false);
                setFDNComponents(true);
            }
    };
    
//Pre Gain
    ReverbPreGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    preGainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "reverb_pre_gain", ReverbPreGainSlider));
    ReverbPreGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    ReverbPreGainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    ReverbPreGainSlider.setTextValueSuffix("dB");
    ReverbPreGainSlider.setRange(-48.0, 10.0);
    addAndMakeVisible(ReverbPreGainSlider);
    ReverbPreGainLabel.setText("Pre Gain", juce::dontSendNotification);
    ReverbPreGainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ReverbPreGainLabel);
    
//Pre Filter
    PreCutoffSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    preCutoffAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "reverb_pre_cutoff", PreCutoffSlider));
    PreCutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    PreCutoffSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    PreCutoffSlider.setTextValueSuffix("Hz");
    PreCutoffSlider.setRange(20.0, 20000.0);
    addAndMakeVisible(PreCutoffSlider);
    PreCutoffLabel.setText("Pre Filter Frequency", juce::dontSendNotification);
    addAndMakeVisible(PreCutoffLabel);
    
    PreResSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    preResAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "reverb_pre_resonance", PreResSlider));
    PreResSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    PreResSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    PreResSlider.setRange(0.0, 10.0);
    addAndMakeVisible(PreResSlider);
    
    PreResLabel.setText("Resonance", juce::dontSendNotification);
    addAndMakeVisible(PreResLabel);
    
    PreSelector.addItemList(Filter_Choices, 1);
    PreSelector.setSelectedId(1);
    preType_attachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(vts, "reverb_pre_filter_type", PreSelector));
    addAndMakeVisible(PreSelector);
    
    
    preTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    preTimeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "reverb_pre_delay", preTimeSlider));
    preTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    preTimeSlider.setTextValueSuffix("ms");
    preTimeSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    preTimeSlider.setRange(0, 2000);
    addAndMakeVisible(preTimeSlider);

    preTimeLabel.setText("Pre Delay", juce::dontSendNotification);
    preTimeLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(preTimeLabel);
    

//Schroeder
    APF1FDBKSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    APF1FDBKAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "apf1_fdbk", APF1FDBKSlider));
    APF1FDBKSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    APF1FDBKSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    APF1FDBKSlider.setRange(0, 1);
    addAndMakeVisible(APF1FDBKSlider);
    
    APF1FDBKLabel.setText("Allpass Feedback", juce::dontSendNotification);
    APF1FDBKLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(APF1FDBKLabel);
    
    
    CombTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    CombTimeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "comb_delay_time", CombTimeSlider));
    CombTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    CombTimeSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    CombTimeSlider.setTextValueSuffix("ms");
    CombTimeSlider.setRange(1, 2000);
    addAndMakeVisible(CombTimeSlider);
    
    CombTimeLabel.setText("Comb Filter Delay", juce::dontSendNotification);
    CombTimeLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(CombTimeLabel);
    
    
    
//Delaying Allpass
    DapTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    DapTimeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "reverb_dap_delay_time", DapTimeSlider));
    DapTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    DapTimeSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    DapTimeSlider.setRange(0, 1999);
    addAndMakeVisible(DapTimeSlider);
    
    DapTimeLabel.setText("Time", juce::dontSendNotification);
    DapTimeLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DapTimeLabel);
    
    
    DapGSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    DapGAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "reverb_dap_g", DapGSlider));
    DapGSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    DapGSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    DapGSlider.setTextValueSuffix("ms");
    DapGSlider.setRange(0, 0.999);
    addAndMakeVisible(DapGSlider);
    
    DapGLabel.setText("G", juce::dontSendNotification);
    DapGLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DapGLabel);
    

    
    setSchroederComponents(false);
    setDAPComponents(false);
    setFDNComponents(false);
}

ReverbEffectComponent::~ReverbEffectComponent()
{
}

void ReverbEffectComponent::paint (juce::Graphics& g)
{

    g.fillAll (juce::Colours::sienna);   // clear the background

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
    
    ReverbPreGainSlider.setBounds(250, 100, 150, 150);
    ReverbPreGainLabel.attachToComponent(&ReverbPreGainSlider, false);
    ReverbPreGainLabel.setJustificationType(juce::Justification::centred);
    
    preTimeSlider.setBounds(250, 300, 150, 150);
    preTimeLabel.attachToComponent(&preTimeSlider, false);
    preTimeLabel.setJustificationType(juce::Justification::centred);
    
    
    PreCutoffSlider.setBounds(50, 50, 150, 150);
    PreCutoffLabel.attachToComponent(&PreCutoffSlider, false);
    PreCutoffLabel.setJustificationType(juce::Justification::centred);
    
    PreResSlider.setBounds(50, 250, 150, 150);
    PreResLabel.attachToComponent(&PreResSlider, false);
    PreResLabel.setJustificationType(juce::Justification::centred);
    
    PreSelector.setBounds(25, 450, 200, 30);
    
    
    
    APF1FDBKSlider.setBounds(500, 50, 150, 150);
    APF1FDBKLabel.attachToComponent(&APF1FDBKSlider, false);
    APF1FDBKLabel.setJustificationType(juce::Justification::centred);
    
    CombTimeSlider.setBounds(500, 350, 150, 150);
    DapTimeLabel.attachToComponent(&CombTimeSlider, false);
    DapTimeLabel.setJustificationType(juce::Justification::centred);
    
    
    
    DapTimeSlider.setBounds(500, 50, 150, 150);
    DapTimeLabel.attachToComponent(&DapTimeSlider, false);
    DapTimeLabel.setJustificationType(juce::Justification::centred);
    
    DapGSlider.setBounds(500, 350, 150, 150);
    DapGLabel.attachToComponent(&DapGSlider, false);
    DapGLabel.setJustificationType(juce::Justification::centred);
    
}

void ReverbEffectComponent::setSchroederComponents(bool show)
{
    APF1FDBKSlider.setEnabled(show);
    APF1FDBKSlider.setVisible(show);
    APF1FDBKLabel.setVisible(show);
    
    CombTimeSlider.setEnabled(show);
    CombTimeSlider.setVisible(show);
    CombTimeLabel.setVisible(show);
}

void ReverbEffectComponent::setDAPComponents(bool show)
{
    DapTimeSlider.setEnabled(show);
    DapTimeSlider.setVisible(show);
    DapTimeLabel.setVisible(show);
    
    DapGSlider.setEnabled(show);
    DapGSlider.setVisible(show);
    DapGLabel.setVisible(show);
}

void ReverbEffectComponent::setFDNComponents(bool show)
{
    
}
