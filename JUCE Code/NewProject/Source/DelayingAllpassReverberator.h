/*
  ==============================================================================

    DelayingAllpassReverberator.h
    Created: 5 May 2024 7:54:49pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "CircularBuffer.h"

class DelayingAllpassReverberator{
public:
    DelayingAllpassReverberator(juce::AudioProcessorValueTreeState& vts): Reverb_Parameters(vts)
    {
        delTime = Reverb_Parameters.getRawParameterValue("reverb_dap_delay_time");
        G = Reverb_Parameters.getRawParameterValue("reverb_dap_g");
    };
    
    ~DelayingAllpassReverberator(){};
    
    //Pre-playback initialisation
    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        this->sampleRate = sampleRate;
        
        this->maxDelay = (sampleRate/1000) * 3000;
        
        delayLine.initBuffer(2, this->maxDelay, (int)sampleRate);
        
//        this->del = 70;
//        this->g = 0.6;

    };
    
    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        auto* inDataL = buffer.getReadPointer(0);
        auto* inDataR = buffer.getReadPointer(1);
        auto* outDataL = buffer.getWritePointer(0);
        auto* outDataR = buffer.getWritePointer(1);

        auto del = delTime->load();
        auto g = G->load();
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto delayedL = delayLine.getDelayedSample(0, del);
            auto delayedR = delayLine.getDelayedSample(1, del);
            
            delayLine.pushSampleToBuffer(0, (inDataL[sample] + ((*delayedL) * g)));
            delayLine.pushSampleToBuffer(1, (inDataR[sample] + ((*delayedR) * g)));
            
            outDataL[sample] += (-g * inDataL[sample]) + ((1 - std::pow(g,2)) * (*delayedL));
            outDataR[sample] += (-g * inDataR[sample]) + ((1 - std::pow(g,2)) * (*delayedR));
        }
    };
    
    juce::AudioProcessorValueTreeState& Reverb_Parameters;
private:
    float sampleRate;
    
    CircularBuffer delayLine;
    
    
    std::atomic<float>* delTime = nullptr;
    std::atomic<float>* G = nullptr;
//    float del = 70;
//    float g = 0.6;
    
    int maxDelay;
    float lastDelay = 0;
};

