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
#include "ProcessorBase.h"
#include "DelayLine.h"
#include "SmoothingFilter.h"

class DelayEffectAudioProcessor  : public ProcessorBase
{
public:
    DelayEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts) : Delay_Parameters(vts)
    {
        //Get Delay variables from APVTS for Delay Parameters
        Delay_on = Delay_Parameters.getRawParameterValue("delay_toggle");
        Delay_Time = Delay_Parameters.getRawParameterValue("delay_time");
        
    };
    
    ~DelayEffectAudioProcessor(){};
    
    const juce::String getName() const override{return "Delay";};
    
    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        pluginSpec.sampleRate = sampleRate;
        pluginSpec.maximumBlockSize = samplesPerBlock;
        pluginSpec.numChannels = 2;
        
        delayLine.setSpec(pluginSpec);
        delayLine.initDelayLine();
        
        SmoothingLPF.setFc(100);
        
        
        lastDelay = Delay_Time->load();
        delaySmoothed.setTargetValue(lastDelay);
        delaySmoothed.reset(samplesPerBlock);
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        auto effectOn = Delay_on->load();
        if(effectOn != 0.0f)
        {
            auto newDelayTime = Delay_Time->load();
            
            if(newDelayTime != lastDelay)
            {
                delayLine.setDelayTime(newDelayTime);
                lastDelay = newDelayTime;
                delaySmoothed.setTargetValue(newDelayTime);
                delaySmoothed.reset(buffer.getNumSamples());
            }
            
            for(int channel = 0; channel < pluginSpec.numChannels; ++channel)
            {
                auto* inData = buffer.getReadPointer(channel);
                auto* outData = buffer.getWritePointer(channel);
                
                for(int i = 0; i < buffer.getNumSamples(); ++i)
                {
                    delayLine.processSample(channel, inData[i]);
                    
                    auto time = SmoothingLPF.process(delaySmoothed.getCurrentValue());
                    delayLine.setDelayTime(time);
                    
                    auto delayedSample = delayLine.getDelayedSample(channel);
                    
                    outData[i] = (delayedSample * WetAmount) + (inData[i] * (1 - WetAmount));
                }
                
            }
        }
    };
    
    juce::AudioProcessorValueTreeState& Delay_Parameters;
    
private:
    
    juce::dsp::ProcessSpec pluginSpec;
    DelayLine delayLine;
    std::atomic<float>* Delay_on = nullptr;
    
    std::atomic<float>* Delay_Time = nullptr;
    float lastDelay;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> delaySmoothed;
    SmoothingFilter SmoothingLPF;

    
    float WetAmount = 0.6;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayEffectAudioProcessor);
};
