/*
  ==============================================================================

    DegradeEffectComponent.h
    Created: 20 Apr 2024 5:10:22pm
    Author:  Kevin Brennan

    Component showing any UI for the Degrade Effect
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DegradeEffectComponent  : public juce::Component
{
public:
    DegradeEffectComponent(juce::AudioProcessorValueTreeState& vts);
    ~DegradeEffectComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextButton Degrade_Toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>Degrade_OnOff_Attachment;
    juce::Label ToggleLabel;
    
    juce::Slider DegradeWDSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>DegradeWDAttachment;
    juce::Label DegradeWDLabel;
    
    juce::Slider DegradeGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>gainAttachment;
    juce::Label DegradeGainLabel;
    
    juce::Slider DegradePRECutoff;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>PRECutoffAttachment;
    juce::Label DegradePRECutoffLabel;
    
    juce::Slider DegradePOSTCutoff;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>POSTCutoffAttachment;
    juce::Label DegradePOSTCutoffLabel;
    
    juce::Slider DegradeFrequencySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>FrequencyAttachment;
    juce::Label DegradeFrequencyLabel;
    
    juce::TextButton Bitcrush_Toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>Bitcrush_OnOff_Attachment;
    juce::Label BitcrushToggleLabel;
    
    juce::Slider BitcrushRateSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>BitcrushRateAttachment;
    juce::Label BitcrushRateLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DegradeEffectComponent)
};
