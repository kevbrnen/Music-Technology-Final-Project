/*
  ==============================================================================

    LowpassFilter.cpp
    Created: 5 Mar 2024 3:14:36pm
    Author:  Kevin Brennan
    A Class for impementing a JUCE lowpass filter (StateVariableTPTFilter)
  ==============================================================================
*/

#include "LowpassFilter.h"

/* DSP Code */
LowpassFilter::LowpassFilter(){}
LowpassFilter::~LowpassFilter(){}

LowpassFilter::LowpassFilter(const LowpassFilter& other) : LPF(other.LPF), Spec(other.Spec), cutoffFrequency(other.cutoffFrequency)
{
    updateFilter();
}

//Have to pass spec to filter class for setting up filter.
//Contains info about samplerate, blocksize, numchannels
void LowpassFilter::process(juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    
    LPF.process(context);
}

void LowpassFilter::setCutoff(double newFreq)
{
    this->cutoffFrequency = newFreq;
    LPF.setCutoffFrequency(this->cutoffFrequency);
    DBG("Setting Freq in filter");
    //updateFilter();
}

//Sets up the filter itself, makes it lowpass and sets the initial cutoff frequency
void LowpassFilter::updateFilter()
{
    DBG("Update Filter");
    LPF.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    LPF.setCutoffFrequency(cutoffFrequency);
    LPF.prepare(Spec);
}

void LowpassFilter::setSpec(const juce::dsp::ProcessSpec &spec)
{
    this->Spec = spec;
    updateFilter();
}

double LowpassFilter::getCutoffFrequency()
{
    DBG("getCutoffFreq: " << this->cutoffFrequency);
    return this->cutoffFrequency;
}

