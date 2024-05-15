/*
  ==============================================================================

    PingPongDelay.h
    Created: 18 Apr 2024 11:39:46pm
    Author:  Kevin Brennan

    A class to implement a Ping Pong delay effect
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "CircularBuffer.h"

class PingPongDelay{
public:
    PingPongDelay(juce::AudioProcessorValueTreeState& vts): Xpanse_Parameters(vts)
    {
        //Get parameters from value tree
        Ltime = Xpanse_Parameters.getRawParameterValue("pong_delay_L_time");
        Rtime = Xpanse_Parameters.getRawParameterValue("pong_delay_R_time");
        fdbk_amtL = Xpanse_Parameters.getRawParameterValue("pong_delay_L_fdbk");
        fdbk_amtR = Xpanse_Parameters.getRawParameterValue("pong_delay_R_fdbk");
    };
    
    ~PingPongDelay(){};
    
    //Pre playback initialisation
    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        this->fs = sampleRate;
        
        Maxsamps = (int)((sampleRate/1000.f) * maxDel);
        Lsamps = (int)((sampleRate/1000.f) * (Ltime->load()));
        Rsamps = (int)((sampleRate/1000.f) * (Rtime->load()));
        
        LBuffer.initBuffer(1, Maxsamps, sampleRate);
        RBuffer.initBuffer(1, Maxsamps, sampleRate);
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        auto* inDataL = buffer.getReadPointer(0);
        auto* inDataR = buffer.getReadPointer(1);
        auto* outDataL = buffer.getWritePointer(0);
        auto* outDataR = buffer.getWritePointer(1);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            //Push sample to buffer with feedback
            //Feedback from the opposite channel gives the ping pong effect
            auto bufinputL = (inDataL[sample] + ((fdbk_amtL->load()) * delayedR));
            auto bufinputR = (inDataR[sample] + ((fdbk_amtR->load()) * delayedL));
            LBuffer.pushSampleToBuffer(0, bufinputL);
            RBuffer.pushSampleToBuffer(0, bufinputR);
            
            //Smoothing Delay time changes
            //Smoothing changes in delay
            auto newDelayTimeL = Ltime->load();
            auto newDelayTimeR = Rtime->load();
            
            //For Smoothing Delay time changes
            float ht_constant = .3f;
            smoothingFac = std::pow((0.5), (1.f/(this->fs * ht_constant)));
            
            float delayTimeL = smoothingFac*(lastDelayL) + (1-smoothingFac)*newDelayTimeL;
            lastDelayL = delayTimeL;
            float delayTimeR = smoothingFac*(lastDelayR) + (1-smoothingFac)*newDelayTimeR;
            lastDelayR = delayTimeR;
            
            
            //Get the delayed sample
            delayedL = *LBuffer.getDelayedSample(0, (delayTimeL));
            delayedR = *RBuffer.getDelayedSample(0, (delayTimeR));
            
            auto tempL = delayedL;
            auto tempR = delayedR;
            
            outDataL[sample] = tempL;
            outDataR[sample] = tempR;
         }
    };
    
    
    juce::AudioProcessorValueTreeState& Xpanse_Parameters;
private:
    CircularBuffer LBuffer;
    CircularBuffer RBuffer;
    float delayedL, delayedR = 0.f;
    
    double fs = 0.f;
    
    float smoothingFac = 0.99f;
    float lastDelayL, lastDelayR = 0.f;
    
    float maxDel = 2000;
    std::atomic<float>* Ltime = nullptr; //float Ltime = 200;
    std::atomic<float>* Rtime = nullptr; //float Rtime = 160;
    float Maxsamps, Lsamps, Rsamps;
    //float wet_amt = 0.5;
    std::atomic<float>* fdbk_amtL = nullptr; //float fdbk_amtL = 0.2;
    std::atomic<float>* fdbk_amtR = nullptr;//float fdbk_amtR = 0.2;
    
};
