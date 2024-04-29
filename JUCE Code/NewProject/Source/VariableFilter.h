/*
  ==============================================================================

    VariableFilter.h
    Created: 28 Apr 2024 11:38:30pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "AllpassFilter.h"

class VariableFilter
{
public:
    VariableFilter(){};
    ~VariableFilter(){};
    
    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        Spec.sampleRate = sampleRate;
        Spec.maximumBlockSize = samplesPerBlock;
        Spec.numChannels = 2;
        
        filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        filter.setCutoffFrequency(cutoffFrequency);
        filter.setResonance(resonance);
        filter.prepare(Spec);
        
        allpassFilter.prepareToPlay(sampleRate, samplesPerBlock);
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        if(currentType == 0)
        {
            allpassFilter.processBlock(buffer);
        }
        else
        {
            juce::dsp::AudioBlock<float> block(buffer);
            juce::dsp::ProcessContextReplacing<float> context(block);
            filter.process(context);
        }
    };
    
    void setType(int type)
    {
        //0 = allpass
        //1 = lowpass
        //2 = bandpass
        //3 = highpass
        switch(type)
        {
            case 0:
                this->currentType = type;
                break;
            case 1:
                filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
                this->currentType = type;
                break;
            case 2:
                filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
                this->currentType = type;
                break;
            case 3:
                filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
                this->currentType = type;
                break;
            default:
                break;
        }
    };
    
    void setCutoffFrequency(float newCutoff)
    {
        this->cutoffFrequency = newCutoff;
        if(currentType == 0)
        {
            allpassFilter.setCutoffFrequency(this->cutoffFrequency);
        }
        else
        {
            filter.setCutoffFrequency(this->cutoffFrequency);
        }
    };

    void setResonance(float newRes)
    {
        this->resonance = newRes;
        
        filter.setResonance(this->resonance);
    };
    
private:
    juce::dsp::ProcessSpec Spec;
    
    juce::dsp::StateVariableTPTFilter<float> filter;
    AllpassFilter allpassFilter;
    
    int currentType;
    
    float cutoffFrequency = 3000;
    float resonance = 0.1;
};
