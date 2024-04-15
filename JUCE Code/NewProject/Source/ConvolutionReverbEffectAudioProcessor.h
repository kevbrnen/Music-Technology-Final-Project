/*
  ==============================================================================

    ConvolutionReverbEffectAudioProcessor.h
    Created: 15 Apr 2024 10:32:15am
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ProcessorBase.h"

class ConvolutionReverbEffectAudioProcessor : public ProcessorBase
{
public:
    ConvolutionReverbEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts): ConvolutionReverb_Parameters(vts)
    {
        //Get Convolution variables from APVTS for Delay Parameters
        Conv_on = ConvolutionReverb_Parameters.getRawParameterValue("convolution_toggle");
        Conv_WD = ConvolutionReverb_Parameters.getRawParameterValue("convolution_wetdry");
    };
    
    ~ConvolutionReverbEffectAudioProcessor(){};
    
    const juce::String getName() const override{return "Convolution Reverb";};
    
    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        pluginSpec.sampleRate = sampleRate;
        pluginSpec.maximumBlockSize = samplesPerBlock;
        pluginSpec.numChannels = 2;
        
        ConvEngine.prepare(pluginSpec);
        
        ConvEngine.loadImpulseResponse(BinaryData::ChurchIR1_wav, BinaryData::ChurchIR1_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
            juce::dsp::Convolution::Normalise::yes);
        
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        auto effectOn = Conv_on->load();
        
        if(effectOn != 0.0f)
        {
            juce::AudioBuffer<float> dry(buffer.getNumChannels(), buffer.getNumSamples());
            dry.copyFrom(0, 0, buffer, 0, 0, buffer.getNumSamples());
            dry.copyFrom(1, 0, buffer, 1, 0, buffer.getNumSamples());
            
            juce::dsp::AudioBlock<float> block (buffer);
            juce::dsp::ProcessContextReplacing<float> context (block);
            
            ConvEngine.process(context);
            
            auto wetDry = Conv_WD->load();
            
            buffer.applyGain(wetDry);
            dry.applyGain(1-wetDry);
            
            // Combine wet and dry signals
            for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
            {
                auto* channelData = buffer.getWritePointer(channel);
                auto* dryData = dry.getReadPointer(channel);
                
                for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
                {
                    // Mix wet and dry signals
                    channelData[sample] += dryData[sample];
                }
            }
            
            
            
        }
    };
    
    juce::AudioProcessorValueTreeState& ConvolutionReverb_Parameters;
    
private:
    juce::dsp::ProcessSpec pluginSpec;
    juce::AudioFormatManager formatManager;
    
    
    juce::dsp::Convolution ConvEngine;
    std::atomic<float>* Conv_on = nullptr;
    std::atomic<float>* Conv_WD = nullptr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConvolutionReverbEffectAudioProcessor);
};
