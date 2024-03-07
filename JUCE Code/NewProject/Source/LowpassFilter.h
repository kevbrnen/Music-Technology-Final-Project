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
    ~LowpassFilter();
    
    void setSpec(const juce::dsp::ProcessSpec& spec);
    
    void process(juce::AudioBuffer<float>& buffer);
    
    void setCutoff(double newFreq, const juce::dsp::ProcessSpec& spec);

private:
    void updateFilter(const juce::dsp::ProcessSpec& spec);
    
    juce::dsp::StateVariableTPTFilter<float> LPF;
    
    double cutoffFrequency;
};
