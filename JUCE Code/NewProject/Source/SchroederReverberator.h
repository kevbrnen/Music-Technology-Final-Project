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
        APF_FDBK2 = Reverb_Parameters.getRawParameterValue("apf2_fdbk");
        combTime = Reverb_Parameters.getRawParameterValue("comb_delay_time");
        apfTime = Reverb_Parameters.getRawParameterValue("apf_delay_time");
    };
    
    ~SchroederReverberator(){};
    
    //Pre-playback initialisation
    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        this->sampleRate = sampleRate;
        
        this->maxDelay = (sampleRate/1000) * 2000;

        for(int i = 0; i < N; ++i)
        {
            parallelDelayLines[i].initBuffer(2, this->maxDelay, (int)sampleRate);
        }
        
        APFLine1.initBuffer(2, this->maxDelay, (int)sampleRate);
        APFLine2.initBuffer(2, this->maxDelay, (int)sampleRate);
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        auto* inDataL = buffer.getReadPointer(0);
        auto* inDataR = buffer.getReadPointer(1);
        auto* outDataL = buffer.getWritePointer(0);
        auto* outDataR = buffer.getWritePointer(1);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto fdbk1 = APF_FDBK1->load();
            auto fdbk2 = APF_FDBK2->load();
            auto combtime = combTime->load();
            auto apTime = apfTime->load();
            
            float outCombL = 0.f;
            float outCombR = 0.f;
            
            float outAPF1L = 0.f;
            float outAPF1R = 0.f;
            
            float outAPF2L = 0.f;
            float outAPF2R = 0.f;
            for(int i = 0; i < N; ++i)
            {
                auto delayedL = parallelDelayLines[i].getDelayedSample(0, delays[i] + combtime);
                auto delayedR = parallelDelayLines[i].getDelayedSample(1, delays[i] + combtime);
                
                outCombL += *delayedL;
                outCombR += *delayedR;
                
                *delayedL *= gains[i];
                *delayedR *= gains[i];
                
                
                parallelDelayLines[i].pushSampleToBuffer(0, (*delayedL + inDataL[sample]));
                parallelDelayLines[i].pushSampleToBuffer(1, (*delayedR + inDataR[sample]));
            }
            
            auto delayedAPF1L = APFLine1.getDelayedSample(0, 34.2+apTime);
            auto delayedAPF1R = APFLine1.getDelayedSample(1, 49+apTime);
            
            APFLine1.pushSampleToBuffer(0, (outCombL + (*delayedAPF1L * fdbk1)));
            APFLine1.pushSampleToBuffer(1, (outCombR + (*delayedAPF1R * fdbk1)));
            
            outAPF1L = (-fdbk1 * outCombL) + ((1 - std::pow(fdbk1,2)) * *delayedAPF1L);
            outAPF1R = (-fdbk1 * outCombR) + ((1 - std::pow(fdbk1,2)) * *delayedAPF1R);
            
            auto delayedAPF2L = APFLine2.getDelayedSample(0, 42+apTime);
            auto delayedAPF2R = APFLine2.getDelayedSample(1, 38+apTime);
            
            APFLine2.pushSampleToBuffer(0, (outAPF1L + (*delayedAPF2L * fdbk2)));
            APFLine2.pushSampleToBuffer(1, (outAPF1R + (*delayedAPF2R * fdbk2)));
            
            outAPF2L = (-fdbk2 * outAPF1L) + ((1 - std::pow(fdbk2,2)) * *delayedAPF2L);
            outAPF2R = (-fdbk2 * outAPF1R) + ((1 - std::pow(fdbk2,2)) * *delayedAPF2R);
            
            outDataL[sample] = outAPF2L;
            outDataR[sample] = outAPF2R;
        }
    };

    juce::AudioProcessorValueTreeState& Reverb_Parameters;
private:
    static const int N = 4;
    
    float sampleRate;
    
    CircularBuffer parallelDelayLines[N];
    CircularBuffer APFLine1, APFLine2;
    float gains[4] = {0.6, 0.4, 0.4, 0.63};
    double delays[4] = {21.5, 36.3, 53.7, 79.12}; //(100ms/3^i)
    
    float g1 = 0.6;
    float g2 = 0.6;
    
    int maxDelay;

    std::atomic<float>* APF_FDBK1 = nullptr;
    std::atomic<float>* APF_FDBK2 = nullptr;
    std::atomic<float>* combTime = nullptr;
    std::atomic<float>* apfTime = nullptr;
};

