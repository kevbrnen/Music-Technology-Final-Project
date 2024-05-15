/*
  ==============================================================================

    SchroederReverberator.h
    Created: 1 May 2024 5:55:06pm
    Author:  Kevin Brennan

    A simple Schroeder Reverberator implementation with 16 delay lines in parallel and 2 allpass filters
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

        //Setting up array of delay lines makes processing easier and tidies up code
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
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto fdbk1 = APF_FDBK1->load();
            auto combtime = combTime->load();
            
            //For Smoothing Delay time changes
            float ht_constant = .3f;
            smoothingFac = std::pow((0.5), (1.f/(this->sampleRate * ht_constant)));
            float delayTime = smoothingFac*(lastDelay) + (1-smoothingFac)*combtime;
            
            
            if(delayTime != lastDelay)
            {
                recalculateDelays(delayTime);
                lastDelay = delayTime;
            }
            
            
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
                
                
                outCombL += (*delayedL);
                outCombR += (*delayedR);
                
                *delayedL *= gains[i];
                *delayedR *= gains[i];
            
                parallelDelayLines[i].pushSampleToBuffer(0, (*delayedL + inDataL[sample]));
                parallelDelayLines[i].pushSampleToBuffer(1, (*delayedR + inDataR[sample]));
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
        //Calculating each delay line length using - (Time_ms/3^i)
        for(int i = 0; i < N; ++i)
        {
            delays[i] = (time)/(std::pow(3, i));
        }
    }

    juce::AudioProcessorValueTreeState& Reverb_Parameters;
private:
    static const int N = 16;
    
    float sampleRate;
    
    CircularBuffer parallelDelayLines[N];
    CircularBuffer APFLine1, APFLine2;
    float gains[16] = {0.7, 0.5, 0.7, 0.63, 0.53, 0.49, 0.394, 0.433, 0.352, 0.392, 0.323, 0.269, 0.214, 0.324, 0.54, 0.38};
    double delays[16] = {29.7, 37.1, 53.7, 79.12, 42, 33, 14, 4, 6, 9, 42, 33, 14, 4, 6, 9};
    double apDelays[4] = {34.2, 49, 42, 38.3};
    
    int maxDelay;
    float lastDelay = 0;
    float smoothingFac = 0.f;

    std::atomic<float>* APF_FDBK1 = nullptr;
    std::atomic<float>* combTime = nullptr;
};

