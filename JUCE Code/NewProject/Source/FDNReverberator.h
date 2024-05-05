/*
  ==============================================================================

    FDNReverberator.h
    Created: 5 May 2024 8:10:01pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "CircularBuffer.h"

class FDNReverberator{
public:
    FDNReverberator(juce::AudioProcessorValueTreeState& vts): Reverb_Parameters(vts)
    {

    };
    
    ~FDNReverberator(){};
    
    //Pre-playback initialisation
    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        this->sampleRate = sampleRate;
        
        this->maxDelay = (sampleRate/1000) * 3000;
        

    };
    
    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        auto* inDataL = buffer.getReadPointer(0);
        auto* inDataR = buffer.getReadPointer(1);
        auto* outDataL = buffer.getWritePointer(0);
        auto* outDataR = buffer.getWritePointer(1);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {

        }
    };
    
    juce::AudioProcessorValueTreeState& Reverb_Parameters;
private:
    float sampleRate;
    
    
    int maxDelay;
    float lastDelay = 0;
};

