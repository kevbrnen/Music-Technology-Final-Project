/*
  ==============================================================================

    DegradeEffectAudioProcessor.h
    Created: 20 Apr 2024 5:10:39pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ProcessorBase.h"
#include "LowpassFilter.h"


class DegradeEffectAudioProcessor  : public ProcessorBase
{
public:
    DegradeEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts): Degrade_Parameters(vts)
    {
        //Get Convolution variables from APVTS for Conv Parameters
        Degrade_on = Degrade_Parameters.getRawParameterValue("degrade_toggle");
        Degrade_WD = Degrade_Parameters.getRawParameterValue("degrade_wetdry");
        Degrade_Gain = Degrade_Parameters.getRawParameterValue("degrade_gain");
        Degrade_PRE_cutoff = Degrade_Parameters.getRawParameterValue("pre_cutoff_frequency");
        Degrade_POST_cutoff = Degrade_Parameters.getRawParameterValue("post_cutoff_frequency");
        Degrade_Frequency = Degrade_Parameters.getRawParameterValue("degrade_frequency");
    };
    
    ~DegradeEffectAudioProcessor(){};
    
    const juce::String getName() const override{return "Degrade";};
    
    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        auto fs_new = Degrade_Frequency->load();
        
        //Setup Spec for LPF implementation
        pluginSpec.sampleRate = sampleRate;
        pluginSpec.maximumBlockSize = samplesPerBlock;
        pluginSpec.numChannels = 2;
        
        //Setup LPF
        LPF_Pre.setSpec(pluginSpec);
        LPF_Pre.setCutoff(0.5 * fs_new);
        LPF_Post.setSpec(pluginSpec);
        LPF_Post.setCutoff(0.5 * fs_new);
        
        N = (int)(sampleRate/fs_new);
        
        this->sampleRate = sampleRate;
        
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        auto effectOn = Degrade_on->load();
        
        if(effectOn != 0.0f)
        {
            juce::AudioBuffer<float> dry(buffer.getNumChannels(), buffer.getNumSamples());
            dry.copyFrom(0, 0, buffer, 0, 0, buffer.getNumSamples());
            dry.copyFrom(1, 0, buffer, 1, 0, buffer.getNumSamples());
            
            auto NewCutoffFrequency_Pre = Degrade_PRE_cutoff->load();
            auto NewCutoffFrequency_Post = Degrade_POST_cutoff->load();
            
            auto fs_new = Degrade_Frequency->load();
            N = (int)(sampleRate/fs_new);
            
            
            LPF_Pre.setCutoff(NewCutoffFrequency_Pre);
            LPF_Pre.process(buffer);
            
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                for(int channel = 0; channel < buffer.getNumChannels(); ++channel)
                {
                    auto* inData = buffer.getReadPointer(channel);
                    auto* outData = buffer.getWritePointer(channel);
                    
                    if(sample % N == 0)
                    {
                        outData[sample] = inData[sample];
                    }
                    else
                    {
                        outData[sample] = 0.0f;
                    }
                    
                }
             }
            
            LPF_Post.setCutoff(NewCutoffFrequency_Post);
            LPF_Post.process(buffer);
            
            
            auto wetDry = Degrade_WD->load();

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
            
            
            const auto NewGain = juce::Decibels::decibelsToGain(*Degrade_Parameters.getRawParameterValue("degrade_gain") + 0.0);
            
            if(NewGain != lastGain)
            {
                //Smooth gain to remove artefacts
                buffer.applyGainRamp(0, buffer.getNumSamples(), lastGain, NewGain);
                lastGain = NewGain;
            }
        }
    };
    
    juce::AudioProcessorValueTreeState& Degrade_Parameters;
    
private:
    
    std::atomic<float>* Degrade_on = nullptr;
    std::atomic<float>* Degrade_WD = nullptr;
    
    std::atomic<float>* Degrade_Gain = nullptr;
    float lastGain;
    
    std::atomic<float>* Degrade_PRE_cutoff = nullptr;
    std::atomic<float>* Degrade_POST_cutoff = nullptr;
    
    std::atomic<float>* Degrade_Frequency = nullptr;
    int N = 0;
    float sampleRate;
    
    juce::dsp::ProcessSpec pluginSpec;
    LowpassFilter LPF_Pre, LPF_Post;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DegradeEffectAudioProcessor);
};

