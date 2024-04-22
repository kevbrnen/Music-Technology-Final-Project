/*
  ==============================================================================

    DelayXpanseEffectAudioProcessor.h
    Created: 18 Apr 2024 11:38:08pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PingPongDelay.h"


class DelayXpanseEffectAudioProcessor  : public ProcessorBase
{
public:
    DelayXpanseEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts): Xpanse_Parameters(vts), pingPongDelay(vts)
    {
        //Get Convolution variables from APVTS for Conv Parameters
        Xpanse_on = Xpanse_Parameters.getRawParameterValue("xpanse_toggle");
        Xpanse_WD = Xpanse_Parameters.getRawParameterValue("xpanse_wetdry");
        Xpanse_Gain = Xpanse_Parameters.getRawParameterValue("xpanse_gain");
    };
    
    ~DelayXpanseEffectAudioProcessor(){};
    
    const juce::String getName() const override{return "DelayXpanse";};
    
    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        pingPongDelay.prepareToPlay(sampleRate, samplesPerBlock);
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        auto effectOn = Xpanse_on->load();
        
        if(effectOn != 0.0f)
        {
            pingPongDelay.processBlock(buffer);
            
            
            const auto NewGain = juce::Decibels::decibelsToGain(*Xpanse_Parameters.getRawParameterValue("xpanse_gain") + 0.0);
            
            if(NewGain != lastGain)
            {
                //Smooth gain to remove artefacts
                buffer.applyGainRamp(0, buffer.getNumSamples(), lastGain, NewGain);
                lastGain = NewGain;
            }
        }
    };
    
    juce::AudioProcessorValueTreeState& Xpanse_Parameters;
    
private:
    
    PingPongDelay pingPongDelay;
    
    std::atomic<float>* Xpanse_on = nullptr;
    std::atomic<float>* Xpanse_WD = nullptr;
    
    std::atomic<float>* Xpanse_Gain = nullptr;
    float lastGain;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayXpanseEffectAudioProcessor);
};
