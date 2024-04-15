/*
  ==============================================================================

    FilterEffectAudioProcessor.h
    Created: 1 Apr 2024 11:55:01pm
    Author:  Kevin Brennan

    Class to process audio for the Filter Effect
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ProcessorBase.h"
#include "LowpassFilter.h"
#include "LFOProcessor.h"

class FilterEffectAudioProcessor  : public ProcessorBase
{
public:
    FilterEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts) : Filter_Parameters(vts)
    {
        //Variables from APVTS for Filter parameters
        FilterCutoffFrequency = Filter_Parameters.getRawParameterValue("cutoff_frequency");
        Filt_on = Filter_Parameters.getRawParameterValue("filter_toggle");
        FilterGain = Filter_Parameters.getRawParameterValue("filter_gain");
        Filter_LFO_on = Filter_Parameters.getRawParameterValue("filter_LFO_toggle");
    };
    
    ~FilterEffectAudioProcessor(){};
    
    
    const juce::String getName() const override {return "Filter";};
    
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        //Setup Spec for LPF implementation
        pluginSpec.sampleRate = sampleRate;
        pluginSpec.maximumBlockSize = samplesPerBlock;
        pluginSpec.numChannels = 2;
        
        //Setup LPF
        LPF.setSpec(pluginSpec);
        LPF.setCutoff(750);
        
        //Gain
        lastGain = juce::Decibels::decibelsToGain(*Filter_Parameters.getRawParameterValue("filter_gain") + 0.0);
    };
    

    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        auto effectOn = Filt_on->load(); //Process if effect on
        if(effectOn != 0.0f)
        {
            //Get and set new cutoff frequency
            auto LFOOn = Filter_LFO_on->load();
            if(LFOOn != 0.0f)
            {
                const auto NewCutoffFrequency = (300 + (LFO.getNextLFOVal() * 1000.f));
                
                LPF.setCutoff(NewCutoffFrequency);
            }
            else
            {
                const auto NewCutoffFrequency = FilterCutoffFrequency->load();
                
                LPF.setCutoff(NewCutoffFrequency);
            }
            
            LPF.process(buffer);
            
            //Filter Effect Gain
            const auto NewGain = juce::Decibels::decibelsToGain(*Filter_Parameters.getRawParameterValue("filter_gain") + 0.0);
            
            if(NewGain != lastGain)
            {
                //Smooth gain to remove artefacts
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
    //Filter variables
    juce::dsp::ProcessSpec pluginSpec;
    LowpassFilter LPF;
    std::atomic<float>* FilterCutoffFrequency = nullptr;
    std::atomic<float>* Filt_on = nullptr;
    std::atomic<float>* FilterGain = nullptr;
    float lastGain;
    
    std::atomic<float>* Filter_LFO_on = nullptr;
    
    LFOProcessor LFO;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterEffectAudioProcessor);
};
