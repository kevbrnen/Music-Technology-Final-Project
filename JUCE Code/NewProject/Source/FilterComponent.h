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
    
    juce::ComboBox LFOSelector;
    juce::StringArray LFO_Choices{"Sin", "Square", "Saw", "Birds"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>LFO_attachment;
    
    juce::Slider LFOSpeedSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>LFOSpeedAttachment;
    juce::Label LFOSpeedLabel;
    
    juce::Slider LFOModWidthSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>LFOModWidthAttachment;
    juce::Label LFOModWidthLabel;
    
    
    juce::Slider Cutoff1LFOSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>cutoff1LFOAttachment;
    juce::Label cutoff1LFOLabel;
    
    juce::Slider Res1LFOSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>res1LFOAttachment;
    juce::Label res1LFOLabel;
    
    juce::Slider Cutoff2LFOSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>cutoff2LFOAttachment;
    juce::Label cutoff2LFOLabel;
    
    juce::Slider Res2LFOSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>res2LFOAttachment;
    juce::Label res2LFOLabel;
    
    
    
    
    
    
    juce::ComboBox TypeSelector;
    juce::StringArray Filter_Choices{"Allpass", "Lowpass", "Bandpass", "Highpass"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>Type_attachment;
    juce::ComboBox TypeSelector2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>Type_attachment2;
    
    juce::Slider cutoffFrequencySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>cutoffFrequencyAttachment;
    juce::Label cutoffFrequencyLabel;
    
    juce::Slider resonanceSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>resonanceAttachment;
    juce::Label resonanceLabel;
    
    
    juce::TextButton filt2_Toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>Filt2_OnOff_Attachment;
    juce::Label filt2Label;
    
    juce::Slider cutoffFrequencySlider2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>cutoffFrequencyAttachment2;
    juce::Label cutoffFrequencyLabel2;
    
    juce::Slider resonanceSlider2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>resonanceAttachment2;
    juce::Label resonanceLabel2;
    
    juce::Slider filterGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>gainAttachment;
    juce::Label filterGainLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
