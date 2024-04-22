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
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayXpanseEffectComponent)
};
