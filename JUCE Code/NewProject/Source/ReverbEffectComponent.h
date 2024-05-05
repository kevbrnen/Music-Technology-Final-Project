/*
  ==============================================================================

    ReverbEffectComponent.h
    Created: 1 May 2024 2:23:18pm
    Author:  Kevin Brennan
 
    Component showing any UI for the Reverb Effect
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
    void setDAPComponents(bool show);
    void setFDNComponents(bool show);

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
    
    
    
    juce::Slider ReverbPreGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>preGainAttachment;
    juce::Label ReverbPreGainLabel;
    
    juce::StringArray Filter_Choices{"Allpass", "Lowpass", "Bandpass", "Highpass"};
    juce::ComboBox PreSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>preType_attachment;
    juce::ComboBox PostSelector;
    
    juce::Slider PreCutoffSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>preCutoffAttachment;
    juce::Label PreCutoffLabel;
    
    juce::Slider PreResSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> preResAttachment;
    juce::Label PreResLabel;
    
    
    
    juce::Slider preTimeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>preTimeAttachment;
    juce::Label preTimeLabel;
    
    
//Schroeder Parameters
    juce::Slider APF1FDBKSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>APF1FDBKAttachment;
    juce::Label APF1FDBKLabel;
    
    juce::Slider CombTimeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>CombTimeAttachment;
    juce::Label CombTimeLabel;
    
//Delaying Allpass Parameters
    juce::Slider DapTimeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>DapTimeAttachment;
    juce::Label DapTimeLabel;
    
    juce::Slider DapGSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>DapGAttachment;
    juce::Label DapGLabel;
    
    
    
    juce::ComboBox TypeSelector;
    juce::StringArray Reverb_Choices{"Schroeder Reverb", "Delaying Allpass Reverb", "FDN Reverb"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>Type_attachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbEffectComponent)
};
