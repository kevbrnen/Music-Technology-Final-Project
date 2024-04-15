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
        
        
    };
    
    ~DelayEffectAudioProcessor(){};
    
    const juce::String getName() const override{return "Delay";};
    
    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        pluginSpec.sampleRate = sampleRate;
        pluginSpec.maximumBlockSize = samplesPerBlock;
        pluginSpec.numChannels = 2;
        
        circularBuffer.initBuffer(2, (int)((48000.0f/1000.0f)*3000.0f), sampleRate);
        
        lastDelay = Delay_Time->load();
        
        delayTimeSmoothing.reset(sampleRate, 0.005);
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
        {
            auto WetAmount = 0.0f;
            auto effectOn = Delay_on->load();
            
            if(effectOn != 0.0f)
            {
                auto C = 0.6;
                
                for(int i = 0; i < buffer.getNumSamples(); ++i)
                {
                    auto LFOOn = Delay_LFO_on->load();
                    if(LFOOn != 0.0f)
                    {
                        WetAmount = LFO.getNextLFOVal();
                    }
                    else
                    {
                        WetAmount = Delay_WD->load();
                    }
                    
                    auto newDelayTime = Delay_Time->load();
                    auto Fdbk_amt = Delay_FDBK->load();
                    
                    delayTimeSmoothing.setTargetValue(newDelayTime);
                    
                    for(int channel = 0; channel < 2 ; ++channel)
                    {
                        auto* inData = buffer.getReadPointer(channel);
                        auto* outData = buffer.getWritePointer(channel);
                        
                        circularBuffer.pushSampleToBuffer(channel, inData[i] + (feedback[channel] * Fdbk_amt));
                            
                        auto time = delayTimeSmoothing.getNextValue();
                        
                        auto delayedSample_oldVal = circularBuffer.getDelayedSample(channel, lastDelay);
                        auto delayedSample_newVal = circularBuffer.getDelayedSample(channel, time);
                            
                        outData[i] = (((*delayedSample_newVal * (1-C)) + (*delayedSample_oldVal * C)) * WetAmount) + (inData[i] * (1 - WetAmount));
                            
                            
                        //Feedback for each channel
                        feedback[channel] = outData[i];
                        lastDelay = time;
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
    float lastGain;
    
    std::atomic<float>* Delay_Time = nullptr;
    float lastDelay;
    juce::LinearSmoothedValue<float> delayTimeSmoothing {500.0f};
    SmoothingFilter SmoothingLPF;
    
    std::atomic<float>* Delay_FDBK = nullptr;
    float feedback[2];
    
    std::atomic<float>* Delay_WD = nullptr;
    
    std::atomic<float>* Delay_LFO_on = nullptr;
    
    LFOProcessor LFO;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayEffectAudioProcessor);
};
