/*
  ==============================================================================

    DelayEffectAudioProcessor.h
    Created: 4 Apr 2024 10:14:24pm
    Author:  Kevin Brennan

    Class to process audio for the Delay Effect
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <math.h>
#include "ProcessorBase.h"
#include "SmoothingFilter.h"
#include "CircularBuffer.h"
#include "LFOProcessor.h"

class DelayEffectAudioProcessor  : public ProcessorBase
{
public:
    DelayEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts) : Delay_Parameters(vts)
    {
        //Get Delay variables from APVTS for Delay Parameters
        Delay_on = Delay_Parameters.getRawParameterValue("delay_toggle");
        Delay_Gain = Delay_Parameters.getRawParameterValue("delay_gain");
        Delay_Time = Delay_Parameters.getRawParameterValue("delay_time");
        Delay_FDBK = Delay_Parameters.getRawParameterValue("delay_fdbk");
        Delay_WD = Delay_Parameters.getRawParameterValue("delay_wetdry");
        
        Delay_LFO_on = Delay_Parameters.getRawParameterValue("delay_LFO_toggle");
        Delay_LFO_type = Delay_Parameters.getRawParameterValue("delay_lfo_type");
        Delay_LFO_Speed = Delay_Parameters.getRawParameterValue("delay_lfo_speed");
        Delay_LFO_ModWidth = Delay_Parameters.getRawParameterValue("delay_lfo_modwidth");
        
        Delay_Time_LFO = Delay_Parameters.getRawParameterValue("delay_time_lfo");
        Delay_FDBK_LFO = Delay_Parameters.getRawParameterValue("delay_fdbk_lfo");
        Delay_WD_LFO = Delay_Parameters.getRawParameterValue("delay_wetdry_lfo");
    };
    
    ~DelayEffectAudioProcessor(){};
    
    const juce::String getName() const override{return "Delay";};
    
    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        pluginSpec.sampleRate = sampleRate;
        pluginSpec.maximumBlockSize = samplesPerBlock;
        pluginSpec.numChannels = 2;
        
        this->maxDelay = ((sampleRate/1000.0f)*3000.0f);
        
        //Set up buffer with num channels, max possible delay in samples and the sample rate
        circularBuffer.initBuffer(2, (int)this->maxDelay, sampleRate);
        
        //Used for smoothing changes in delay time
        lastDelay = Delay_Time->load();
        
        LFO.prepareToPlay(sampleRate, samplesPerBlock);
        
        LastLFOSpeed = Delay_LFO_Speed->load();
        LastLFOMW = Delay_LFO_ModWidth->load();
        
        LFO.setSpeed(LastLFOSpeed);
        LFO.setMW(LastLFOMW);
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
        {
            auto WetAmount = 0.0f;
            auto effectOn = Delay_on->load();
            
            if(effectOn != 0.0f) //Process if the effect is on
            {
                auto C = 0.7; //interpolation factor
                
                for(int i = 0; i < buffer.getNumSamples(); ++i)
                {
                    //load parameter for this sample
                    auto LFOOn = Delay_LFO_on->load();
                    auto newDelayTime = Delay_Time->load();
                    auto Fdbk_amt = Delay_FDBK->load();
                    
                    WetAmount = Delay_WD->load();
                    
                    float ht_constant = .3f;
                    smoothingFac = std::pow((0.5), (1.f/(pluginSpec.sampleRate * ht_constant)));
                    float delayTime = smoothingFac*(lastDelay) + (1-smoothingFac)*newDelayTime;
                    lastDelay = delayTime;
                    
// LFO Processing
                    if(LFOOn != 0.f)
                    {
                        //Check if LFO speed was updated
                        auto newSpeed = Delay_LFO_Speed->load();
                        if(newSpeed != LastLFOSpeed)
                        {
                            LFO.setSpeed(newSpeed);
                            LastLFOSpeed = newSpeed;
                        }
                        
                        //Check if LFO mod width was updated
                        auto newMW = Delay_LFO_ModWidth->load();
                        if(newMW != LastLFOMW)
                        {
                            LFO.setMW(newMW);
                            LastLFOMW = newMW;
                        }
                        
                        //Get the next LFO val
                        auto lfoVal = LFO.getNextLFOVal(Delay_LFO_type->load());
                        
                        auto timeAmt = Delay_Time_LFO->load();
                        auto wetAmt = Delay_WD_LFO->load();
                        auto fdbkAmt = Delay_FDBK_LFO->load();
                        
                        //Apply LFO val and parameter amount vals to parameters if there is
                        //LFO to apply (Amt > 0)
                        if(wetAmt > 0.0f){WetAmount *= (lfoVal * wetAmt);}
                        
                        if(timeAmt > 0.0f){newDelayTime += std::min((delayTime * lfoVal * timeAmt),(this->maxDelay - 1));}
                        
                        if(fdbkAmt > 0.0f){Fdbk_amt *= (lfoVal * fdbkAmt);}
                            
                    }
                    
                    
                    //Set the next target to smooth to as the new delay time
                    //delayTimeSmoothing.setTargetValue(newDelayTime);
                    
                    //loop through channels and process
                    for(int channel = 0; channel < 2 ; ++channel)
                    {
                        auto* inData = buffer.getReadPointer(channel);
                        auto* outData = buffer.getWritePointer(channel);
                        
                        //Push sample to buffer with feedback, determined by feedback amount
                        circularBuffer.pushSampleToBuffer(channel, (inData[i] + (feedback[channel] * Fdbk_amt)));
                        
                        //Get the delayed sample at the last delay time and the delayed sample at the new delay time
                        auto delayedSample_newVal = circularBuffer.getDelayedSample(channel, delayTime);
                            
                        //Interpolate between old and new delayed samples, compute wet/dry combination and add to output
                        outData[i] = (((*delayedSample_newVal * (1-C)) + (delayedSample_oldVal * C)) * WetAmount) + (inData[i] * (1 - WetAmount));
                        delayedSample_oldVal = *delayedSample_newVal;
                            
                        //Feedback for each channel
                        feedback[channel] = outData[i];
                        
                    }
                }
                
                const auto NewGain = juce::Decibels::decibelsToGain(*Delay_Parameters.getRawParameterValue("delay_gain") + 0.0);
                
                if(NewGain != lastGain)
                {
                    //Smooth gain to remove artefacts
                    buffer.applyGainRamp(0, buffer.getNumSamples(), lastGain, NewGain);
                    lastGain = NewGain;
                }
                
                
            }
        };
    
    juce::AudioProcessorValueTreeState& Delay_Parameters;
    
private:
    
    juce::dsp::ProcessSpec pluginSpec;
    CircularBuffer circularBuffer;
    std::atomic<float>* Delay_on = nullptr;
    
    std::atomic<float>* Delay_Gain = nullptr;
    float lastGain = 0;
    
    std::atomic<float>* Delay_Time = nullptr;
    float lastDelay;
    float smoothingFac = 0.99f;
    float delayedSample_oldVal = 0.f;
    
    std::atomic<float>* Delay_FDBK = nullptr;
    float feedback[2];
    
    std::atomic<float>* Delay_WD = nullptr;
    
    std::atomic<float>* Delay_LFO_on = nullptr;
    std::atomic<float>* Delay_LFO_type = nullptr;
    std::atomic<float>* Delay_LFO_Speed = nullptr;
    std::atomic<float>* Delay_LFO_ModWidth = nullptr;
    std::atomic<float>* Delay_Time_LFO = nullptr;
    std::atomic<float>* Delay_FDBK_LFO = nullptr;
    std::atomic<float>* Delay_WD_LFO = nullptr;
    
    float LastLFOSpeed;
    float LastLFOMW;
    
    LFOProcessor LFO;
    
    float maxDelay = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayEffectAudioProcessor);
};
