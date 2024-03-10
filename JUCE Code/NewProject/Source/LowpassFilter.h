/*
  ==============================================================================

    LowpassFilter.h
    Created: 5 Mar 2024 3:14:36pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once

#include <vector>
#include "JuceHeader.h"

class LowpassFilter
{
public:
    LowpassFilter();
    LowpassFilter(const LowpassFilter& other);
    ~LowpassFilter();
    
    void process(juce::AudioBuffer<float>& buffer);
    
    void setCutoff(double newFreq);
    
    void setSpec(const juce::dsp::ProcessSpec& spec);
    
    double getCutoffFrequency();

private:
    void updateFilter();
    
    juce::dsp::StateVariableTPTFilter<float> LPF;
    
    juce::dsp::ProcessSpec Spec;
    
    double cutoffFrequency;
    
    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LowpassFilter)
};
