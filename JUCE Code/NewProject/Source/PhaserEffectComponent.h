/*
  ==============================================================================

    PhaserEffectComponent.h
    Created: 30 Apr 2024 11:45:29am
    Author:  Kevin Brennan
 
    Component showing any UI for the Phaser Effect
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PhaserEffectComponent  : public juce::Component
{
public:
    PhaserEffectComponent(juce::AudioProcessorValueTreeState& vts);
    ~PhaserEffectComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextButton Phaser_Toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>Phaser_OnOff_Attachment;
    juce::Label ToggleLabel;
    
    juce::Slider phaserGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>gainAttachment;
    juce::Label phaserGainLabel;
    
    juce::Slider PhaserWDSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>PhaserWDAttachment;
    juce::Label PhaserWDLabel;
    
    juce::Slider PhaserSpeedSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>PhaserSpeedAttachment;
    juce::Label PhaserSpeedLabel;
    
    juce::Slider PhaserIntensitySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>PhaserIntensityAttachment;
    juce::Label PhaserIntensityLabel;
    
    juce::Slider PhaserQSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>PhaserQAttachment;
    juce::Label PhaserQLabel;
    
    juce::Slider PhaserModWidthSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>PhaserModWidthAttachment;
    juce::Label PhaserModWidthLabel;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaserEffectComponent)
};
