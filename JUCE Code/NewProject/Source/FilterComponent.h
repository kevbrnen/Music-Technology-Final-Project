/*
  ==============================================================================

    FilterComponent.h
    Created: 9 Mar 2024 10:36:04pm
    Author:  Kevin Brennan

    Component to show UI for Filter Effect
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LFOThumbnailComponent.h"

//==============================================================================
/*
*/
class FilterComponent  : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& vts);
    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    juce::TextButton LPF_Toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>Filter_OnOff_Attachment;
    juce::Label ToggleLabel;
    
    juce::TextButton Filter_LFO_Toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>Filter_LFO_Toggle_Attachment;
    juce::Label LFOLabel;
    
    juce::ComboBox TypeSelector;
    juce::StringArray Filter_Choices{"Allpass", "Lowpass", "Bandpass", "Highpass"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>Type_attachment;
    
    juce::Slider cutoffFrequencySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>cutoffFrequencyAttachment;
    juce::Label cutoffFrequencyLabel;
    
    juce::Slider resonanceSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>resonanceAttachment;
    juce::Label resonanceLabel;
    
    
    juce::Slider filterGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>gainAttachment;
    juce::Label filterGainLabel;
    
    LFOThumbnailComponent LFOThumb;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
