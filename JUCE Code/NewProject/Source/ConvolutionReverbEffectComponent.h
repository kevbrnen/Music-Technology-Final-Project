/*
  ==============================================================================

    ConvolutionReverbEffectComponent.h
    Created: 15 Apr 2024 10:31:41am
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WaveformThumbnailComponent.h"

//==============================================================================
/*
*/
class ConvolutionReverbEffectComponent  : public juce::Component
{
public:
    ConvolutionReverbEffectComponent(juce::AudioProcessorValueTreeState& vts);
    ~ConvolutionReverbEffectComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void updateThumbnail();

private:
    juce::TextButton Convolution_Toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>Convolution_OnOff_Attachment;
    juce::Label ToggleLabel;
    
    juce::Slider ConvolutionWDSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>ConvolutionWDAttachment;
    juce::Label ConvolutionWDLabel;
    
    juce::Slider ConvolutionGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>gainAttachment;
    juce::Label ConvolutionGainLabel;
    
    WaveformThumbnailComponent waveformComponent;
    
    juce::ComboBox IRSelector;
    juce::StringArray Impulses{"Church-1", "Shipping Container", "Hall-1", "Tent"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>IR_attachment;
    int lastIR;
    const char* data = BinaryData::ChurchIR1_wav;
    int Size = BinaryData::ChurchIR1_wavSize;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConvolutionReverbEffectComponent)
};
