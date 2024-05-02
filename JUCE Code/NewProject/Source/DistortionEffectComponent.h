/*
  ==============================================================================

    DistortionEffectComponent.h
    Created: 2 May 2024 3:15:55pm
    Author:  Kevin Brennan

    Component Showing any UI for the Distortion Effect
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DistortionEffectComponent  : public juce::Component
{
public:
    DistortionEffectComponent(juce::AudioProcessorValueTreeState& vts);
    ~DistortionEffectComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    juce::TextButton Distortion_Toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>Distortion_OnOff_Attachment;
    juce::Label ToggleLabel;
    
    juce::Slider DistortionWDSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>DistortionWDAttachment;
    juce::Label DistortionWDLabel;
    
    juce::Slider DistortionGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>gainAttachment;
    juce::Label DistortionGainLabel;
    
    juce::ComboBox TypeSelector;
    juce::StringArray Distortion_Choices{"Tanh", "Cubic Soft", "Square", "Clipping", "Soft Saturation"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>Type_attachment;
    
    juce::Slider PreGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>preGainAttachment;
    juce::Label PreGainLabel;
    
    juce::Slider ThreshSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> threshAttachment;
    juce::Label ThreshLabel;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionEffectComponent)
};
