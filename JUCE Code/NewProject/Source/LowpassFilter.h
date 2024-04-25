/*
  ==============================================================================

    LowpassFilter.h
    Created: 5 Mar 2024 3:14:36pm
    Author:  Kevin Brennan
 
    A Class for impementing a JUCE lowpass filter (StateVariableTPTFilter)
  ==============================================================================
*/

#pragma once

#include <vector>
#include "JuceHeader.h"

class LowpassFilter
{
public:
    LowpassFilter(){};
    LowpassFilter(const LowpassFilter& other) : LPF(other.LPF), Spec(other.Spec), cutoffFrequency(other.cutoffFrequency), resonance(other.resonance)
    {
        updateFilter();
    };
    
    ~LowpassFilter(){};
    
    void process(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        
        LPF.process(context);
    };
    
    void setCutoff(double newFreq)
    {
        this->cutoffFrequency = newFreq;
        LPF.setCutoffFrequency(this->cutoffFrequency);
    };
    
    void setResonance(float res)
    {
        this->resonance = res;
        LPF.setResonance(this->resonance);
    };
    
    void setSpec(const juce::dsp::ProcessSpec &spec)
    {
        this->Spec = spec;
        updateFilter();
    };
    
    double getCutoffFrequency()
    {
        return this->cutoffFrequency;
    };
    
    double getResonance()
    {
        return this->resonance;
    };

private:
    void updateFilter()
    {
        LPF.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        LPF.setCutoffFrequency(cutoffFrequency);
        LPF.setResonance(resonance);
        LPF.prepare(Spec);
    };
    
    juce::dsp::StateVariableTPTFilter<float> LPF;
    
    juce::dsp::ProcessSpec Spec;
    
    double cutoffFrequency;
    float resonance;
    
    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LowpassFilter)
};
