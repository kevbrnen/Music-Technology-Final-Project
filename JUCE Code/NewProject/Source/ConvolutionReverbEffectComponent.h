/*
  ==============================================================================

    ConvolutionReverbEffectComponent.h
    Created: 15 Apr 2024 10:31:41am
    Author:  Kevin Brennan

    Component showing any UI for the Convolution Reverb Effect
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WaveformThumbnailComponent.h"
#include "BinaryData.h"

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
    
    juce::Slider ConvolutionPreGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>preGainAttachment;
    juce::Label ConvolutionPreGainLabel;
    
    juce::Slider ConvolutionGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>gainAttachment;
    juce::Label ConvolutionGainLabel;
    
    juce::Slider PreDelayTimeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>PreDelayTimeAttachment;
    juce::Label PreDelayTimeLabel;
    
    juce::Slider PreDelayFeedbackSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>PreDelayFeedbackAttachment;
    juce::Label PreDelayFeedbackLabel;
    
    WaveformThumbnailComponent waveformComponent;
    
    juce::ComboBox IRSelector;
    juce::StringArray Impulses{"Ballinafad Castle", "Ballymote Castle", "Bedroom", "Church 1", "Church 2", "Shipping Container", "Wooden Hall", "Farmhouse", "Forest", "Friary", "Tent", "Tunnel"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>IR_attachment;
    int lastIR;
    const char* data = BinaryData::Ballinafad_Castle48k_wav;
    int Size = BinaryData::Ballinafad_Castle48k_wavSize;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConvolutionReverbEffectComponent)
};
