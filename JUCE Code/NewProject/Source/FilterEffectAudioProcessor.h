/*
  ==============================================================================

    FilterEffectAudioProcessor.h
    Created: 1 Apr 2024 11:55:01pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ProcessorBase.h"
#include "LowpassFilter.h"

class FilterEffectAudioProcessor  : public ProcessorBase
{
public:
    FilterEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts) : Filter_Parameters(vts)
    {
        FilterCutoffFrequency = Filter_Parameters.getRawParameterValue("cutoff_frequency");
        Filt_on = Filter_Parameters.getRawParameterValue("filter_toggle");
        FilterGain = Filter_Parameters.getRawParameterValue("filter_gain");
        
    };
    
    ~FilterEffectAudioProcessor(){};
    
    
    const juce::String getName() const override {return "Filter";};
    
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        pluginSpec.sampleRate = sampleRate;
        pluginSpec.maximumBlockSize = samplesPerBlock;
        pluginSpec.numChannels = 2;
        
        LPF.setSpec(pluginSpec);
        LPF.setCutoff(750);
        
        //Gain
        lastGain = juce::Decibels::decibelsToGain(*Filter_Parameters.getRawParameterValue("filter_gain") + 0.0);
    };
    

    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        auto effectOn = Filt_on->load();
        if(effectOn != 0.0f)
        {
            //Get and set new cutoff frequency
            const auto NewCutoffFrequency = FilterCutoffFrequency->load();
            LPF.setCutoff(NewCutoffFrequency);
            LPF.process(buffer);
            
            
            //Filter Effect Gain
            const auto NewGain = juce::Decibels::decibelsToGain(*Filter_Parameters.getRawParameterValue("filter_gain") + 0.0);
            
            if(NewGain != lastGain)
            {
                buffer.applyGainRamp(0, buffer.getNumSamples(), lastGain, NewGain);
                lastGain = NewGain;
            }
            
        }
    };

    
    void initSpec(juce::dsp::ProcessSpec spec)
    {
        pluginSpec = spec;
    };
    
    juce::AudioProcessorValueTreeState& Filter_Parameters;
    
private:
    
    juce::dsp::ProcessSpec pluginSpec;
    LowpassFilter LPF;
    std::atomic<float>* FilterCutoffFrequency = nullptr;
    std::atomic<float>* Filt_on = nullptr;
    std::atomic<float>* FilterGain = nullptr;
    float lastGain;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterEffectAudioProcessor);
};
