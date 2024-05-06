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
    
    
    juce::ComboBox LFOSelector;
    juce::StringArray LFO_Choices{"Sin", "Square", "Saw", "Birds"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>LFO_attachment;
    
    juce::Slider LFOSpeedSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>LFOSpeedAttachment;
    juce::Label LFOSpeedLabel;
    
    juce::Slider LFOModWidthSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>LFOModWidthAttachment;
    juce::Label LFOModWidthLabel;
    
    
    juce::Slider TimeLFOSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>timeLFOAttachment;
    juce::Label TimeLFOLabel;
    
    juce::Slider FDBKLFOSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>fdbkLFOAttachment;
    juce::Label FDBKLFOLabel;
    
    juce::Slider WDLFOSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>wdLFOAttachment;
    juce::Label WDLFOLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayEffectComponent)
};
