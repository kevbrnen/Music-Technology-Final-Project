/*
  ==============================================================================

    ReverbEffectComponent.h
    Created: 1 May 2024 2:23:18pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ReverbEffectComponent  : public juce::Component
{
public:
    ReverbEffectComponent(juce::AudioProcessorValueTreeState& vts);
    ~ReverbEffectComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void setSchroederComponents(bool show);
    void setCombComponents(bool show);

private:
    juce::TextButton Reverb_Toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>Reverb_OnOff_Attachment;
    juce::Label ToggleLabel;
    
    juce::Slider ReverbWDSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>ReverbWDAttachment;
    juce::Label ReverbWDLabel;
    
    juce::Slider ReverbGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>gainAttachment;
    juce::Label ReverbGainLabel;
    
    juce::ComboBox TypeSelector;
    juce::StringArray Reverb_Choices{"Schroeder Reverb", "Comb Filter Reverb"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>Type_attachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbEffectComponent)
};
