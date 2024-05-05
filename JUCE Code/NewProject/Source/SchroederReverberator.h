/*
  ==============================================================================

    SchroederReverberator.h
    Created: 1 May 2024 5:55:06pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "CircularBuffer.h"

class SchroederReverberator{
public:
    SchroederReverberator(juce::AudioProcessorValueTreeState& vts): Reverb_Parameters(vts)
    {
        APF_FDBK1 = Reverb_Parameters.getRawParameterValue("apf1_fdbk");
        combTime = Reverb_Parameters.getRawParameterValue("comb_delay_time");
    };
    
    ~SchroederReverberator(){};
    
    //Pre-playback initialisation
    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        this->sampleRate = sampleRate;
        
        this->maxDelay = (sampleRate/1000) * 3000;

        for(int i = 0; i < N; ++i)
        {
            parallelDelayLines[i].initBuffer(2, this->maxDelay, (int)sampleRate);
        }
        
        APFLine1.initBuffer(2, this->maxDelay, (int)sampleRate);
        APFLine2.initBuffer(2, this->maxDelay, (int)sampleRate);
        
        lastDelay = combTime->load();
        
        recalculateDelays(lastDelay);
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        auto* inDataL = buffer.getReadPointer(0);
        auto* inDataR = buffer.getReadPointer(1);
        auto* outDataL = buffer.getWritePointer(0);
        auto* outDataR = buffer.getWritePointer(1);
        
        auto fdbk1 = APF_FDBK1->load();
        auto combtime = combTime->load();
        
        
        if(combtime != lastDelay)
        {
            recalculateDelays(combtime);
            lastDelay = combtime;
        }
        

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float outCombL = 0.f;
            float outCombR = 0.f;
            
            float outAPF1L = 0.f;
            float outAPF1R = 0.f;
            
            float outAPF2L = 0.f;
            float outAPF2R = 0.f;
            for(int i = 0; i < N; ++i)
            {
                auto delayedL = parallelDelayLines[i].getDelayedSample(0, delays[i]);
                auto delayedR = parallelDelayLines[i].getDelayedSample(1, delays[i]);
                
                outCombL += *delayedL;
                outCombR += *delayedR;
            
                parallelDelayLines[i].pushSampleToBuffer(0, ((*delayedL * gains[i]) + inDataL[sample]));
                parallelDelayLines[i].pushSampleToBuffer(1, ((*delayedR * gains[i]) + inDataR[sample]));
            }
            
            auto delayedAPF1L = APFLine1.getDelayedSample(0, apDelays[0]);
            auto delayedAPF1R = APFLine1.getDelayedSample(1, apDelays[1]);
            
            APFLine1.pushSampleToBuffer(0, (outCombL + (*delayedAPF1L * fdbk1)));
            APFLine1.pushSampleToBuffer(1, (outCombR + (*delayedAPF1R * fdbk1)));
            
            outAPF1L = (-fdbk1 * outCombL) + ((1 - std::pow(fdbk1,2)) * *delayedAPF1L);
            outAPF1R = (-fdbk1 * outCombR) + ((1 - std::pow(fdbk1,2)) * *delayedAPF1R);
            
            auto delayedAPF2L = APFLine2.getDelayedSample(0, apDelays[2]);
            auto delayedAPF2R = APFLine2.getDelayedSample(1, apDelays[3]);
            
            APFLine2.pushSampleToBuffer(0, (outAPF1L + (*delayedAPF2L * fdbk1)));
            APFLine2.pushSampleToBuffer(1, (outAPF1R + (*delayedAPF2R * fdbk1)));
            
            outAPF2L = (-fdbk1 * outAPF1L) + ((1 - std::pow(fdbk1,2)) * *delayedAPF2L);
            outAPF2R = (-fdbk1 * outAPF1R) + ((1 - std::pow(fdbk1,2)) * *delayedAPF2R);
            
            outDataL[sample] = outAPF2L;
            outDataR[sample] = outAPF2R;
        }
    };
    
    void recalculateDelays(float time)
    {
        for(int i = 0; i < N; ++i)
        {
            delays[i] = (time)/(std::pow(3, i));
        }
    }

    juce::AudioProcessorValueTreeState& Reverb_Parameters;
private:
    static const int N = 4;
    
    float sampleRate;
    
    CircularBuffer parallelDelayLines[N];
    CircularBuffer APFLine1, APFLine2;
    float gains[4] = {0.6, 0.4, 0.4, 0.63};
    double delays[4] = {29.7, 37.1, 53.7, 79.12}; //(100ms/3^i)
    double apDelays[4] = {34.2, 49, 42, 38.3};
    
    int maxDelay;
    float lastDelay = 0;

    std::atomic<float>* APF_FDBK1 = nullptr;
    std::atomic<float>* combTime = nullptr;
};

