/*
  ==============================================================================

    ConvolutionReverbEffectComponent.cpp
    Created: 15 Apr 2024 10:31:41am
    Author:  Kevin Brennan

    Component showing any UI for the Convolution Reverb Effect
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
    ConvolutionWDSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::seagreen);
    ConvolutionWDSlider.setRange(0.0, 1.0);
    addAndMakeVisible(ConvolutionWDSlider);
    
    ConvolutionWDLabel.setText("Wet/Dry Amount", juce::dontSendNotification);
    ConvolutionWDLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ConvolutionWDLabel);
    
//PreGain Slider
    ConvolutionPreGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    preGainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "conv_pre_gain", ConvolutionPreGainSlider));
    ConvolutionPreGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    ConvolutionPreGainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::seagreen);
    ConvolutionPreGainSlider.setTextValueSuffix("dB");
    ConvolutionPreGainSlider.setRange(-48.0, 10.0);
    addAndMakeVisible(ConvolutionPreGainSlider);
    
    ConvolutionPreGainLabel.setText("Pre Gain", juce::dontSendNotification);
    ConvolutionPreGainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ConvolutionPreGainLabel);
    
    
//Gain Slider
    ConvolutionGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "conv_gain", ConvolutionGainSlider));
    ConvolutionGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    ConvolutionGainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    ConvolutionGainSlider.setTextValueSuffix("dB");
    ConvolutionGainSlider.setRange(-48.0, 10.0);
    addAndMakeVisible(ConvolutionGainSlider);
    
    ConvolutionGainLabel.setText("Convolution Effect Gain", juce::dontSendNotification);
    ConvolutionGainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(ConvolutionGainLabel);
    
//Pre Delay Time Slider
    PreDelayTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    PreDelayTimeAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "pre_delay_time", PreDelayTimeSlider));
    PreDelayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 25);
    PreDelayTimeSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::seagreen);
    PreDelayTimeSlider.setTextValueSuffix("ms");
    PreDelayTimeSlider.setRange(0.0, 249.0);
    addAndMakeVisible(PreDelayTimeSlider);
    
    
    PreDelayTimeLabel.setText("Pre Delay Time", juce::dontSendNotification);
    PreDelayTimeLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(PreDelayTimeLabel);
    
    
//Pre Delay Feedback amount slider
    PreDelayFeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    PreDelayFeedbackAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "pre_delay_fdbk", PreDelayFeedbackSlider));
    PreDelayFeedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 200, 25);
    PreDelayFeedbackSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::seagreen);
    PreDelayFeedbackSlider.setRange(0.0, 0.999);
    addAndMakeVisible(PreDelayFeedbackSlider);
    
    PreDelayFeedbackLabel.setText("Feedback Amount", juce::dontSendNotification);
    PreDelayFeedbackLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    addAndMakeVisible(PreDelayFeedbackLabel);
    
//Waveform Display
    addAndMakeVisible(waveformComponent);
    
//Combobox
    IRSelector.addItemList(Impulses, 1);
    IRSelector.setSelectedId(1);
    IR_attachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(vts, "selector", IRSelector));
    IRSelector.onChange = [this, &vts]()
    {
        updateThumbnail();
        waveformComponent.setVisible(false);
        waveformComponent.setVisible(true);
    };
    addAndMakeVisible(IRSelector);
    

}

ConvolutionReverbEffectComponent::~ConvolutionReverbEffectComponent()
{
}

void ConvolutionReverbEffectComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::mediumspringgreen);   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 5);   // draw an outline around the component

    
}

void ConvolutionReverbEffectComponent::resized()
{
    Convolution_Toggle.setBounds(getWidth()-60, 20, 50, 25);
    ToggleLabel.attachToComponent(&Convolution_Toggle, true);
    
    ConvolutionWDSlider.setBounds(getWidth()-275, getHeight()*2/4, 250, 250);
    ConvolutionWDLabel.attachToComponent(&ConvolutionWDSlider, false);
    ConvolutionWDLabel.setJustificationType(juce::Justification::centred);
    
    ConvolutionPreGainSlider.setBounds(getWidth() - 700, (getHeight()*2/4)+50, 200, 200);
    ConvolutionPreGainLabel.attachToComponent(&ConvolutionPreGainSlider, true);
    
    PreDelayTimeSlider.setBounds(getWidth() - 450, (getHeight()*2/4)+25, 100, 100);
    PreDelayTimeLabel.attachToComponent(&PreDelayTimeSlider, false);
    PreDelayTimeLabel.setJustificationType(juce::Justification::centred);
    
    PreDelayFeedbackSlider.setBounds(getWidth() - 450, (getHeight()*2/4)+150, 100, 100);
    PreDelayFeedbackLabel.attachToComponent(&PreDelayFeedbackSlider, false);
    PreDelayFeedbackLabel.setJustificationType(juce::Justification::centred);
    
    ConvolutionGainSlider.setBounds(getWidth() - 110, 60, 100, 100);
    ConvolutionGainLabel.attachToComponent(&ConvolutionGainSlider, true);
    
    waveformComponent.setBounds(20, 20, getWidth()*7/10, 225);
    
    IRSelector.setBounds(20, 260, 200, 30);
}

void ConvolutionReverbEffectComponent::updateThumbnail()
{
    auto IR = IRSelector.getSelectedId();
    
    if(IR != lastIR)
    {
        switch(IR)
        {
            case 1: //Church
                this->data = BinaryData::ChurchIR1_wav;
                this->Size = BinaryData::ChurchIR1_wavSize;
                break;
            case 2:
                this->data = BinaryData::DenContainer48k_wav;
                this->Size = BinaryData::DenContainer48k_wavSize;
                break;
            case 3:
                this->data = BinaryData::DenHall48k_wav;
                this->Size = BinaryData::DenHall48k_wavSize;
                break;
            case 4:
                this->data = BinaryData::Tent48k_wav;
                this->Size = BinaryData::Tent48k_wavSize;
                break;
            default:
                break;
        }
        
        waveformComponent.setFile(data, Size);
        
        lastIR = IR;
    }
}
