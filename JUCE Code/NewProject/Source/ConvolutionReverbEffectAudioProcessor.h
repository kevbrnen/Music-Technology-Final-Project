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
        //Get Convolution variables from APVTS for Conv Parameters
        Conv_on = ConvolutionReverb_Parameters.getRawParameterValue("convolution_toggle");
        Conv_WD = ConvolutionReverb_Parameters.getRawParameterValue("convolution_wetdry");
        Conv_Gain = ConvolutionReverb_Parameters.getRawParameterValue("conv_gain");
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
        
        updateFile(0);
        
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        auto effectOn = Conv_on->load();
        
        if(effectOn != 0.0f)
        {
            
            auto IR_index = ConvolutionReverb_Parameters.getRawParameterValue("selector");
            
            if((int)*IR_index != lastIR)
            {
                lastIR = *IR_index;
                updateFile(lastIR);
            }
            
            
            juce::AudioBuffer<float> dry(buffer.getNumChannels(), buffer.getNumSamples());
            dry.copyFrom(0, 0, buffer, 0, 0, buffer.getNumSamples());
            dry.copyFrom(1, 0, buffer, 1, 0, buffer.getNumSamples());
            
            auto preGain = juce::Decibels::decibelsToGain(*ConvolutionReverb_Parameters.getRawParameterValue("conv_pre_gain") + 0.0);
            buffer.applyGain(preGain);
            
            
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
            
            
            const auto NewGain = juce::Decibels::decibelsToGain(*ConvolutionReverb_Parameters.getRawParameterValue("conv_gain") + 0.0);
            
            if(NewGain != lastGain)
            {
                //Smooth gain to remove artefacts
                buffer.applyGainRamp(0, buffer.getNumSamples(), lastGain, NewGain);
                lastGain = NewGain;
            }
            
            
        }
    };
    
    void updateFile(int IR)
    {
            switch(IR)
            {
                case 0:
                    ConvEngine.loadImpulseResponse(BinaryData::ChurchIR1_wav, BinaryData::ChurchIR1_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
                                                   juce::dsp::Convolution::Normalise::yes);
                    break;
                case 1:
                    ConvEngine.loadImpulseResponse(BinaryData::DenContainer48k_wav, BinaryData::DenContainer48k_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
                                                   juce::dsp::Convolution::Normalise::yes);
                    break;
                case 2:
                    ConvEngine.loadImpulseResponse(BinaryData::DenHall48k_wav, BinaryData::DenHall48k_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
                                                   juce::dsp::Convolution::Normalise::yes);
                    break;
                case 3:
                    ConvEngine.loadImpulseResponse(BinaryData::Tent48k_wav, BinaryData::Tent48k_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
                                                   juce::dsp::Convolution::Normalise::yes);
                    break;
                default:
                    break;
            }
    }
    
    juce::AudioProcessorValueTreeState& ConvolutionReverb_Parameters;
    
private:
    juce::dsp::ProcessSpec pluginSpec;
    juce::AudioFormatManager formatManager;
    
    
    juce::dsp::Convolution ConvEngine;
    std::atomic<float>* Conv_on = nullptr;
    std::atomic<float>* Conv_WD = nullptr;
    
    std::atomic<float>* Conv_Gain = nullptr;
    float lastGain;

    float lastIR = 0;
    
    const char* IRs[4] = {BinaryData::ChurchIR1_wav, BinaryData::DenContainer48k_wav, BinaryData::DenHall48k_wav, BinaryData::Tent48k_wav};
    int Size[4] = {BinaryData::ChurchIR1_wavSize, BinaryData::DenContainer48k_wavSize, BinaryData::DenHall48k_wavSize, BinaryData::Tent48k_wavSize};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConvolutionReverbEffectAudioProcessor);
};
