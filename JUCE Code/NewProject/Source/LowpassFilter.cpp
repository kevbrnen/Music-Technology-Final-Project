/*
  ==============================================================================

    LowpassFilter.cpp
    Created: 5 Mar 2024 3:14:36pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#include "LowpassFilter.h"

//Have to pass spec to filter class for setting up filter.
//Contains info about samplerate, blocksize, numchannels
LowpassFilter::LowpassFilter(){}

LowpassFilter::~LowpassFilter(){}

void LowpassFilter::process(juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    
    LPF.process(context);
}

void LowpassFilter::setCutoff(double newFreq, const juce::dsp::ProcessSpec& spec)
{
    cutoffFrequency = newFreq;
    updateFilter(spec);
    
}

//Sets up the filter itself, makes it lowpass and sets the initial cutoff frequency
void LowpassFilter::updateFilter(const juce::dsp::ProcessSpec& spec)
{
    LPF.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    LPF.setCutoffFrequency(cutoffFrequency);
    LPF.prepare(spec);
}

