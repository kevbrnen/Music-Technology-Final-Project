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
#include "VariableFilter.h"
#include "LFOProcessor.h"

class FilterEffectAudioProcessor  : public ProcessorBase
{
public:
    FilterEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts) : Filter_Parameters(vts)
    {
        //Variables from APVTS for Filter parameters
        FilterCutoffFrequency = Filter_Parameters.getRawParameterValue("cutoff_frequency");
        FilterResonance = Filter_Parameters.getRawParameterValue("resonance");
        Filt_on = Filter_Parameters.getRawParameterValue("filter_toggle");
        
        
        FilterCutoffFrequency2 = Filter_Parameters.getRawParameterValue("cutoff_frequency2");
        FilterResonance2 = Filter_Parameters.getRawParameterValue("resonance2");
        Filt_on2 = Filter_Parameters.getRawParameterValue("filter_toggle2");
        
        
        FilterGain = Filter_Parameters.getRawParameterValue("filter_gain");
        Filter_LFO_on = Filter_Parameters.getRawParameterValue("filter_LFO_toggle");
    };
    
    ~FilterEffectAudioProcessor(){};
    
    
    const juce::String getName() const override {return "Filter";};
    
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        //Setup Spec for variable filter implementation
        pluginSpec.sampleRate = sampleRate;
        pluginSpec.maximumBlockSize = samplesPerBlock;
        pluginSpec.numChannels = 2;
        
        //Setup variable filter
        varFilt.prepareToPlay(sampleRate, samplesPerBlock);
        varFilt.setType(filterType);

        varFilt2.prepareToPlay(sampleRate, samplesPerBlock);
        varFilt2.setType(filterType2);
        
        //Gain
        lastGain = juce::Decibels::decibelsToGain(*Filter_Parameters.getRawParameterValue("filter_gain") + 0.0);
    };
    

    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        auto effectOn = Filt_on->load(); //Process if effect on
        if(effectOn != 0.0f)
        {
            
            //Check if the type of filter has changed, if so update the type
            auto type_index = Filter_Parameters.getRawParameterValue("filter_types");
            if((int)*type_index != filterType)
            {
                this->filterType = *type_index;
                varFilt.setType(this->filterType);
            }
            //Get and set resonance
            auto res = FilterResonance->load();
            varFilt.setResonance(res);
            //Get and set cutoff frequency
            const auto NewCutoffFrequency = FilterCutoffFrequency->load();
            varFilt.setCutoffFrequency(NewCutoffFrequency);
            //Process the block
            varFilt.processBlock(buffer);
            
            
            auto filt2On = Filt_on2->load();
            if(filt2On != 0.0f)
            {
                //Check if the type of filter has changed, if so update the type
                auto type_index2 = Filter_Parameters.getRawParameterValue("filter_types2");
                if((int)*type_index2 != filterType2)
                {
                    this->filterType2 = *type_index2;
                    varFilt2.setType(this->filterType2);
                }
                //Get and set resonance
                auto res2 = FilterResonance2->load();
                varFilt2.setResonance(res2);
                //Get and set cutoff frequency
                const auto NewCutoffFrequency2 = FilterCutoffFrequency2->load();
                varFilt2.setCutoffFrequency(NewCutoffFrequency2);
                //Process the block
                varFilt2.processBlock(buffer);
            }
            
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


    
    juce::AudioProcessorValueTreeState& Filter_Parameters;
    
private:
    //Filter variables
    juce::dsp::ProcessSpec pluginSpec;
    VariableFilter varFilt, varFilt2;
    
    std::atomic<float>* FilterCutoffFrequency = nullptr;
    std::atomic<float>* FilterResonance = nullptr;
    std::atomic<float>* FilterCutoffFrequency2 = nullptr;
    std::atomic<float>* FilterResonance2 = nullptr;
    std::atomic<float>* Filt_on = nullptr;
    std::atomic<float>* Filt_on2 = nullptr;
    std::atomic<float>* FilterGain = nullptr;
    float lastGain;
    
    std::atomic<float>* Filter_LFO_on = nullptr;
    
    int filterType = 0;
    int filterType2 = 0;
    
    LFOProcessor LFO;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterEffectAudioProcessor);
};
