/*
  ==============================================================================

    DelayEffectComponent.cpp
    Created: 5 Apr 2024 10:54:36am
    Author:  Kevin Brennan

    Component showing any UI for the Delay Effect
  ==============================================================================
*/

#include <JuceHeader.h>
#include "DelayEffectComponent.h"


//==============================================================================
DelayEffectComponent::DelayEffectComponent(juce::AudioProcessorValueTreeState& vts)
{
//Toggle
    Delay_Toggle.setClickingTogglesState(true);
    Delay_Toggle.setButtonText("OFF");
    Delay_Toggle.setEnabled(true);
    Delay_Toggle.setVisible(true);
    Delay_Toggle.onClick = [this, &vts]()
    {
        if(Delay_Toggle.getToggleState())
        {
            Delay_Toggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
            Delay_Toggle.setButtonText("ON");
            vts.state.setProperty("delay_toggle", true, nullptr);
            
        }
        else
        {
            Delay_Toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            Delay_Toggle.setButtonText("OFF");
            vts.state.setProperty("delay_toggle", false, nullptr);
        }
    };
    Delay_OnOff_Attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(vts, "delay_toggle", Delay_Toggle));
    addAndMakeVisible(Delay_Toggle);
    
    ToggleLabel.setText("Delay Effect", juce::dontSendNotification);
    ToggleLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ToggleLabel);
    
    
//Delay Time Slider
    DelayTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    DelayTimeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "delay_time", DelayTimeSlider));
    DelayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    DelayTimeSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::grey);
    DelayTimeSlider.setTextValueSuffix("ms");
    DelayTimeSlider.setRange(0.0, 2999.0);
    addAndMakeVisible(DelayTimeSlider);
    
    
    DelayTimeLabel.setText("Delay Time", juce::dontSendNotification);
    DelayTimeLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DelayTimeLabel);
    
    
//Delay Feedback amount slider
    DelayFeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    DelayFeedbackAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "delay_fdbk", DelayFeedbackSlider));
    DelayFeedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    DelayFeedbackSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::grey);
    DelayFeedbackSlider.setRange(0.0, 0.999);
    addAndMakeVisible(DelayFeedbackSlider);
    
    DelayFeedbackLabel.setText("Feedback Amount", juce::dontSendNotification);
    DelayFeedbackLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DelayFeedbackLabel);
    

//Delay Wet Dry amount slider
    DelayWDSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    DelayWDAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "delay_wetdry", DelayWDSlider));
    DelayWDSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 35);
    DelayWDSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::grey);
    DelayWDSlider.setRange(0.0, 1.0);
    addAndMakeVisible(DelayWDSlider);
    
    DelayWDLabel.setText("Wet/Dry Amount", juce::dontSendNotification);
    DelayWDLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(DelayWDLabel);
    
    
//Gain Slider
    delayGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "delay_gain", delayGainSlider));
    delayGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    delayGainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    delayGainSlider.setTextValueSuffix("dB");
    delayGainSlider.setRange(-48.0, 10.0);
    addAndMakeVisible(delayGainSlider);
    
    delayGainLabel.setText("Delay Effect Gain", juce::dontSendNotification);
    delayGainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(delayGainLabel);
    
    
//LFO components
    //LFO Toggle
    Delay_LFO_Toggle.setClickingTogglesState(true);
    Delay_LFO_Toggle.setButtonText("OFF");
    Delay_LFO_Toggle.setEnabled(true);
    Delay_LFO_Toggle.setVisible(true);
    Delay_LFO_Toggle.onClick = [this, &vts]()
    {
        if(Delay_LFO_Toggle.getToggleState())
        {
            Delay_LFO_Toggle.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
            Delay_LFO_Toggle.setButtonText("ON");
            vts.state.setProperty("delay_LFO_toggle", true, nullptr);
            
        }
        else
        {
            Delay_LFO_Toggle.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            Delay_LFO_Toggle.setButtonText("OFF");
            vts.state.setProperty("delay_LFO_toggle", false, nullptr);
        }
    };
    Delay_LFO_Toggle_Attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(vts, "delay_LFO_toggle", Delay_LFO_Toggle));
    addAndMakeVisible(Delay_LFO_Toggle);
    
    LFOLabel.setText("LFO", juce::dontSendNotification);
    LFOLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(LFOLabel);
    
    //LFO Selector
    LFOSelector.addItemList(LFO_Choices, 1);
    LFOSelector.setSelectedId(1);
    LFO_attachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(vts, "delay_lfo_type", LFOSelector));
    addAndMakeVisible(LFOSelector);
    LFOSelector.onChange = [this, &vts]()
    {
        if(LFOSelector.getSelectedId() == 4)
        {
            LFOSpeedSlider.setVisible(false);
            LFOSpeedSlider.setEnabled(false);
            LFOSpeedLabel.setVisible(false);
            
            LFOModWidthSlider.setVisible(false);
            LFOModWidthSlider.setEnabled(false);
            LFOModWidthLabel.setVisible(false);
        }
        else
        {
            LFOSpeedSlider.setVisible(true);
            LFOSpeedSlider.setEnabled(true);
            LFOSpeedLabel.setVisible(true);
            
            LFOModWidthSlider.setVisible(true);
            LFOModWidthSlider.setEnabled(true);
            LFOModWidthLabel.setVisible(true);
        }
    };

    setSize(150,300);
    
    
    //LFOSpeed
    LFOSpeedSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    LFOSpeedAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "delay_lfo_speed", LFOSpeedSlider));
    LFOSpeedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 15);
    LFOSpeedSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    LFOSpeedSlider.setTextValueSuffix("Hz");
    LFOSpeedSlider.setRange(0.0, 10.0);
    addAndMakeVisible(LFOSpeedSlider);
    
    LFOSpeedLabel.setText("LFO Speed", juce::dontSendNotification);
    LFOSpeedLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(LFOSpeedLabel);
    
    
    //LFO Mod Width
    LFOModWidthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    LFOModWidthAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "delay_lfo_modwidth", LFOModWidthSlider));
    LFOModWidthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 15);
    LFOModWidthSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    LFOModWidthSlider.setRange(0.0, 1.0);
    addAndMakeVisible(LFOModWidthSlider);
    
    LFOModWidthLabel.setText("Mod Width", juce::dontSendNotification);
    LFOModWidthLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(LFOModWidthLabel);
    
    
    //LFO Parameter Amount sliders
    TimeLFOSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    timeLFOAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "delay_time_lfo", TimeLFOSlider));
    TimeLFOSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 15);
    TimeLFOSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    TimeLFOSlider.setRange(0.0, 0.999);
    addAndMakeVisible(TimeLFOSlider);
    TimeLFOLabel.setText("Time", juce::dontSendNotification);
    TimeLFOLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(TimeLFOLabel);
    
    FDBKLFOSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    fdbkLFOAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "delay_fdbk_lfo", FDBKLFOSlider));
    FDBKLFOSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 15);
    FDBKLFOSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    FDBKLFOSlider.setRange(0.0, 0.999);
    addAndMakeVisible(FDBKLFOSlider);
    FDBKLFOLabel.setText("FDBK", juce::dontSendNotification);
    FDBKLFOLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(FDBKLFOLabel);
    
    WDLFOSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
    wdLFOAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "delay_wetdry_lfo", WDLFOSlider));
    WDLFOSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 15);
    WDLFOSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    WDLFOSlider.setRange(0.0, 0.999);
    addAndMakeVisible(WDLFOSlider);
    WDLFOLabel.setText("Wet/Dry", juce::dontSendNotification);
    WDLFOLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(WDLFOLabel);
    
}

DelayEffectComponent::~DelayEffectComponent()
{
}

void DelayEffectComponent::paint (juce::Graphics& g)
{

    g.fillAll(juce::Colours::silver);   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 5);   // draw an outline around the component
    
    g.setColour (juce::Colours::grey);
    g.drawRect (getWidth()-225, 185, getWidth()-210, getHeight()-185, 5);   // draw an outline around the component

}

void DelayEffectComponent::resized()
{
    Delay_Toggle.setBounds(getWidth()-60, 20, 50, 25);
    ToggleLabel.attachToComponent(&Delay_Toggle, true);
    
    Delay_LFO_Toggle.setBounds(getWidth()-60, 200, 50, 25);
    LFOLabel.attachToComponent(&Delay_LFO_Toggle, true);
    
    LFOSelector.setBounds(getWidth()-160, 230, 100, 30);
    
    LFOSpeedSlider.setBounds(getWidth()-200, 290, 75, 75);
    LFOSpeedLabel.attachToComponent(&LFOSpeedSlider, false);
    LFOSpeedLabel.setJustificationType(juce::Justification::centred);
    
    LFOModWidthSlider.setBounds(getWidth()-100, 290, 75, 75);
    LFOModWidthLabel.attachToComponent(&LFOModWidthSlider, false);
    LFOModWidthLabel.setJustificationType(juce::Justification::centred);
    
    TimeLFOSlider.setBounds(getWidth()-150, 390, 100, 30);
    TimeLFOLabel.attachToComponent(&TimeLFOSlider, true);
    
    FDBKLFOSlider.setBounds(getWidth()-150, 430, 100, 30);
    FDBKLFOLabel.attachToComponent(&FDBKLFOSlider, true);
    
    WDLFOSlider.setBounds(getWidth()-150, 470, 100, 30);
    WDLFOLabel.attachToComponent(&WDLFOSlider, true);
    
    
    
    delayGainSlider.setBounds(getWidth() - 110, 60, 100, 100);
    delayGainLabel.attachToComponent(&delayGainSlider, true);

    DelayTimeSlider.setBounds(100, getHeight()/4, 300, 300);
    DelayTimeLabel.attachToComponent(&DelayTimeSlider, true);
    
    DelayFeedbackSlider.setBounds((getWidth()/2)+25, 50, 200, 200);
    DelayFeedbackLabel.attachToComponent(&DelayFeedbackSlider, false);
    
    DelayWDSlider.setBounds((getWidth()/2)+25, 3*(getHeight()/6) + 50, 200, 200);
    DelayWDLabel.attachToComponent(&DelayWDSlider, false);
    
    

}
