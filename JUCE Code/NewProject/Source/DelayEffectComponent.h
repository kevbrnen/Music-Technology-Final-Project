/*
  ==============================================================================

    DelayEffectComponent.h
    Created: 5 Apr 2024 10:54:36am
    Author:  Kevin Brennan
 
    Component showing any UI for the Delay Effect
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LFOThumbnailComponent.h"

//==============================================================================
/*
*/
class DelayEffectComponent  : public juce::Component
{
public:
    DelayEffectComponent(juce::AudioProcessorValueTreeState& vts);
    ~DelayEffectComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextButton Delay_Toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>Delay_OnOff_Attachment;
    juce::Label ToggleLabel;
    
    juce::TextButton Delay_LFO_Toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>Delay_LFO_Toggle_Attachment;
    juce::Label LFOLabel;
    
    juce::Slider DelayTimeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>DelayTimeAttachment;
    juce::Label DelayTimeLabel;
    
    juce::Slider DelayFeedbackSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>DelayFeedbackAttachment;
    juce::Label DelayFeedbackLabel;
    
    juce::Slider DelayWDSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>DelayWDAttachment;
    juce::Label DelayWDLabel;
    
    juce::Slider delayGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>gainAttachment;
    juce::Label delayGainLabel;
    
    LFOThumbnailComponent LFOThumb;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayEffectComponent)
};
