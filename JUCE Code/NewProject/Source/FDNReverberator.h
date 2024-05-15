/*
  ==============================================================================

    FDNReverberator.h
    Created: 5 May 2024 8:10:01pm
    Author:  Kevin Brennan

    A simple 3x3 Feedback Delay Network Reverb implementation
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "CircularBuffer.h"

class FDNReverberator{
public:
    FDNReverberator(juce::AudioProcessorValueTreeState& vts): Reverb_Parameters(vts)
    {
        Time = Reverb_Parameters.getRawParameterValue("fdn_time");
        FDBK = Reverb_Parameters.getRawParameterValue("fdn_fdbk");
    };
    
    ~FDNReverberator(){};
    
    //Pre-playback initialisation
    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        this->sampleRate = sampleRate;
        
        this->maxDelay = (sampleRate/1000) * 3000;
        
        
        for(int i = 0; i < N; ++i)
        {
            delayLines[i].initBuffer(2, this->maxDelay, (int)sampleRate);
        }
        
        lastTime = Time->load();
        recalculateDelays(lastTime);
        
        lastFdbk = FDBK->load();
        recalculateMatrix(lastFdbk);
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        auto* inDataL = buffer.getReadPointer(0);
        auto* inDataR = buffer.getReadPointer(1);
        auto* outDataL = buffer.getWritePointer(0);
        auto* outDataR = buffer.getWritePointer(1);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            //For smoothing Delay time Changes
            auto time = Time->load();
            float ht_constant = .3f;
            smoothingFac = std::pow((0.5), (1.f/(this->sampleRate * ht_constant)));
            float delayTime = smoothingFac*(lastTime) + (1-smoothingFac)*time;
            
            if(delayTime != lastTime)
            {
                recalculateDelays(delayTime);
                this->lastTime = delayTime;
            }
            
            auto fdbk = FDBK->load();
            if(fdbk != lastFdbk)
            {
                recalculateMatrix(fdbk);
                this->lastFdbk = fdbk;
            }
            
            for(int i = 0; i < N; ++i)
            {
                //Delayed samples
                auto temp1L = delayLines[i].getDelayedSample(0, delays[i]);
                auto temp1R = delayLines[i].getDelayedSample(1, delays[i]);
                
                float delayedL = *temp1L;
                float delayedR = *temp1R;
                
                //K gain
                delayedL *= k[i];
                delayedR *= k[i];
                
                float fdbkL[N][N];
                float fdbkR[N][N];
                //Multiply value by feedback matrix
                for (int q = 0; q < N; ++q)
                {
                    for (int r = 0; r < N; ++r)
                    {
                        fdbkL[q][r] = fdn_Matrix[q][r] * delayedL;
                        fdbkR[q][r] = fdn_Matrix[q][r] * delayedR;
                    }
                }
                
                float temp2L = 0;
                float temp2R = 0;
                for(int l = 0; l < N; ++l)
                {
                    temp2L += fdbkL[i][l];
                    temp2R += fdbkR[i][l];
                }
                
                
                delayLines[i].pushSampleToBuffer(0, (temp2L + (b[i] * inDataL[sample])));
                delayLines[i].pushSampleToBuffer(1, (temp2R + (b[i] * inDataR[sample])));
                
                
                outDataL[sample] += c[i] * delayedL;
                outDataR[sample] += c[i] * delayedR;
                
            }
        }
    };
    
    void recalculateDelays(float time)
    {
        for(int i = 0; i < N; ++i)
        {
            delays[i] = (time)/(std::pow(3, i));
        }
    }
    
    void recalculateMatrix(float fdbk)
    {
        
        for(int i = 0; i < N; ++i)
        {
            for(int j = 0; j < N; ++j)
            {
                fdn_Matrix[i][j] *= fdbk;
            }
        }
        
    }
    
    juce::AudioProcessorValueTreeState& Reverb_Parameters;
private:
    float sampleRate;
    
    int N = 3;
    
    float delays[3] = {20.f, 30.f, 25.f};
    
    float b[3] = {0.8f, 0.9f, 1.f};
    float c[3] = {0.7f, 0.8f, 0.9f};
    float k[3] = {0.6f, 0.7f, 0.8f};
    
    float fdn_Matrix[3][3] = {{-0.3f, -0.5f, 0.7f},
                              {0.4f, -0.1f, -0.6f},
                              {-0.2f, 0.3f, -0.5f}};
    
    CircularBuffer delayLines[3];
    
    
    int maxDelay;
    float lastTime = 0;
    float lastFdbk = 0;
    float smoothingFac = 0.99f;
    
    std::atomic<float>* Time = nullptr;
    std::atomic<float>* FDBK = nullptr;

};

