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
        
        delayLine.prepare(pluginSpec);
        delayLine.setMaximumDelayInSamples((int)((48000.0f/1000.0f)*3000.0f));
        delayLine.reset();
        
        
        SmoothingLPF.setFc(100);
        
        lastDelay = Delay_Time->load();
        delaySmoothed.setTargetValue(lastDelay);
        delaySmoothed.reset(sampleRate, 1);
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
        {
            auto effectOn = Delay_on->load();
            if(effectOn != 0.0f)
            {
                auto newDelayTime = Delay_Time->load();
                
                newDelayTime = SmoothingLPF.process(newDelayTime);
                    
                    for(int i = 0; i < buffer.getNumSamples(); ++i)
                    {
                        for(int channel = 0; channel < 2 ; ++channel)
                        {
                            auto* inData = buffer.getReadPointer(channel);
                            auto* outData = buffer.getWritePointer(channel);
                            delayLine.pushSample(channel, inData[i]);
                            
                            auto time = delaySmoothed.getNextValue();
                            //delayLine.setDelayTime(time);
                            
                            auto delayedSample_oldVal = delayLine.popSample(channel, ((48000.0f/1000.0f)*time), true);
                            
                            if(newDelayTime != lastDelay)
                            {
                                
                                lastDelay = newDelayTime;
                                delaySmoothed.setTargetValue(lastDelay);
                                delaySmoothed.reset(pluginSpec.sampleRate);
                                delaySmoothed.setTargetValue(newDelayTime);
                                delaySmoothed.reset(pluginSpec.sampleRate);
                                
                                auto t_n = delaySmoothed.getNextValue();
                                
                                auto delayedSample_newVal = delayLine.popSample(channel, ((48000.0f/1000.0f)*t_n), true);
                                
                                outData[i] = ((delayedSample_oldVal + delayedSample_newVal)/2 * WetAmount) + (inData[i] * (1 - WetAmount));
                            }
                            else
                            {
                                outData[i] = ((delayedSample_oldVal) * WetAmount) + (inData[i] * (1 - WetAmount));
                            }
                    }
                }
            }
        };
    
    juce::AudioProcessorValueTreeState& Delay_Parameters;
    
private:
    
    juce::dsp::ProcessSpec pluginSpec;
    //DelayLine delayLine;
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Thiran> delayLine;
    std::atomic<float>* Delay_on = nullptr;
    
    std::atomic<float>* Delay_Time = nullptr;
    float lastDelay;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> delaySmoothed;
    SmoothingFilter SmoothingLPF;

    
    float WetAmount = 0.6;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayEffectAudioProcessor);
};
