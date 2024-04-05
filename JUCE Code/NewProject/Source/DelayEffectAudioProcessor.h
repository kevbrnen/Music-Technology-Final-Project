/*
  ==============================================================================

    DelayEffectAudioProcessor.h
    Created: 4 Apr 2024 10:14:24pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ProcessorBase.h"
#include "DelayLine.h"

class DelayEffectAudioProcessor  : public ProcessorBase
{
public:
    DelayEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts) : Delay_Parameters(vts)
    {
        
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
        
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        //delayLine.processBlock(buffer);
        for(int channel = 0; channel < pluginSpec.numChannels; ++channel)
        {
            auto* inData = buffer.getReadPointer(channel);
            auto* outData = buffer.getWritePointer(channel);

            for(int i = 0; i < buffer.getNumSamples(); ++i)
            {
                delayLine.processSample(channel, inData[i]);
                outData[i] = (delayLine.getDelayedSample(channel) * WetAmount) + (inData[i] * (1 - WetAmount));
            }

        }
    };
    
    juce::AudioProcessorValueTreeState& Delay_Parameters;
    
private:
    
    juce::dsp::ProcessSpec pluginSpec;
    
    DelayLine delayLine;
    
    float WetAmount = 0.6;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayEffectAudioProcessor);
};
