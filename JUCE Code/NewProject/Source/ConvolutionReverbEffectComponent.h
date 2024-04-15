/*
  ==============================================================================

    ConvolutionReverbEffectComponent.h
    Created: 15 Apr 2024 10:31:41am
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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

private:
    juce::TextButton Convolution_Toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>Convolution_OnOff_Attachment;
    juce::Label ToggleLabel;
    
    juce::Slider ConvolutionWDSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>ConvolutionWDAttachment;
    juce::Label ConvolutionWDLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConvolutionReverbEffectComponent)
};
