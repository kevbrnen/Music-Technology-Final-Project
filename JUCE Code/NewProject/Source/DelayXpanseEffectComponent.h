/*
  ==============================================================================

    DelayXpanseEffectComponent.h
    Created: 18 Apr 2024 11:38:33pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DelayXpanseEffectComponent  : public juce::Component
{
public:
    DelayXpanseEffectComponent(juce::AudioProcessorValueTreeState& vts);
    ~DelayXpanseEffectComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void setPingPongComponents(bool show);
    
    void setSpectralComponents(bool show);

private:
    juce::TextButton Xpanse_Toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>Xpanse_OnOff_Attachment;
    juce::Label ToggleLabel;
    
    juce::Slider XpanseWDSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>XpanseWDAttachment;
    juce::Label XpanseWDLabel;
    
    juce::Slider XpanseGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>gainAttachment;
    juce::Label XpanseGainLabel;
    
    juce::ComboBox TypeSelector;
    juce::StringArray Delay_Choices{"Ping-Pong Delay", "Spectral Delay"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>Type_attachment;
    
    
//Ping Pong Delay
    juce::Slider pingPongDelaySlider_L;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>Delay_Attachment_L;
    juce::Label DelayLabel_L;
    
    juce::Slider pingPongDelaySlider_R;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>Delay_Attachment_R;
    juce::Label DelayLabel_R;
    
    juce::Slider pingPongFDBKSlider_L;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>FDBK_Attachment_L;
    juce::Label FDBKLabel_L;
    
    juce::Slider pingPongFDBKSlider_R;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>FDBK_Attachment_R;
    juce::Label FDBKLabel_R;
    
//Spectral Delay
    juce::Slider Band1Gain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>band1Gain_attachment;
    juce::Label Band1GainLabel;
    
    juce::Slider Band2Gain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>band2Gain_attachment;
    juce::Label Band2GainLabel;
    
    juce::Slider Band3Gain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>band3Gain_attachment;
    juce::Label Band3GainLabel;
    
    juce::Slider Band4Gain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>band4Gain_attachment;
    juce::Label Band4GainLabel;
    
    
    juce::Slider Band1Time;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>band1Time_attachment;
    juce::Label Band1TimeLabel;
    
    juce::Slider Band2Time;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>band2Time_attachment;
    juce::Label Band2TimeLabel;
    
    juce::Slider Band3Time;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>band3Time_attachment;
    juce::Label Band3TimeLabel;
    
    juce::Slider Band4Time;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>band4Time_attachment;
    juce::Label Band4TimeLabel;
    
    
    juce::Slider Band1FDBK;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>band1FDBK_attachment;
    juce::Label Band1FDBKLabel;
    
    juce::Slider Band2FDBK;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>band2FDBK_attachment;
    juce::Label Band2FDBKLabel;
    
    juce::Slider Band3FDBK;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>band3FDBK_attachment;
    juce::Label Band3FDBKLabel;
    
    juce::Slider Band4FDBK;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>band4FDBK_attachment;
    juce::Label Band4FDBKLabel;
    
    
    juce::Slider specCutoff1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>specCutoff1_attachment;
    juce::Label specCutoff1Label;
    
    juce::Slider specCutoff2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>specCutoff2_attachment;
    juce::Label specCutoff2Label;
    
    juce::Slider specCutoff3;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>specCutoff3_attachment;
    juce::Label specCutoff3Label;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayXpanseEffectComponent)
};
