/*
  ==============================================================================

    DelayXpanseEffectAudioProcessor.h
    Created: 18 Apr 2024 11:38:08pm
    Author:  Kevin Brennan

    A class for processing audio for the delay xpanse effect
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PingPongDelay.h"
#include "SpectralDelay.h"


class DelayXpanseEffectAudioProcessor  : public ProcessorBase
{
public:
    DelayXpanseEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts): Xpanse_Parameters(vts), pingPongDelay(vts), spectralDelay(vts)
    {
        //Get parameters from VTS
        Xpanse_on = Xpanse_Parameters.getRawParameterValue("xpanse_toggle");
        Xpanse_WD = Xpanse_Parameters.getRawParameterValue("xpanse_wetdry");
        Xpanse_Gain = Xpanse_Parameters.getRawParameterValue("xpanse_gain");
        Type = Xpanse_Parameters.getRawParameterValue("xpanse_type");
    };
    
    ~DelayXpanseEffectAudioProcessor(){};
    
    const juce::String getName() const override{return "DelayXpanse";};
    
    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        pingPongDelay.prepareToPlay(sampleRate, samplesPerBlock);
        spectralDelay.prepareToPlay(sampleRate, samplesPerBlock);
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        auto effectOn = Xpanse_on->load();
        
        if(effectOn != 0.0f)
        {
            //Dry copy of buffer
            juce::AudioBuffer<float> dry(buffer.getNumChannels(), buffer.getNumSamples());
            dry.copyFrom(0, 0, buffer, 0, 0, buffer.getNumSamples());
            dry.copyFrom(1, 0, buffer, 1, 0, buffer.getNumSamples());
            
            
            //Process depending on selected type
            if(Type->load() == 0)
            {
                pingPongDelay.processBlock(buffer);
            }
            else if(Type->load() == 1)
            {
                spectralDelay.processBlock(buffer);
            }
            
            
            //Wet Dry
            auto wetDry = Xpanse_WD->load();
            buffer.applyGain(wetDry);
            dry.applyGain(1-wetDry);
            
            // Combine wet and dry signals
            for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
            {
                auto* channelData = buffer.getWritePointer(channel);
                auto* dryData = dry.getReadPointer(channel);
                
                for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
                {
                    // Mix wet and dry signals
                    channelData[sample] += dryData[sample];
                }
            }
            
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
    SpectralDelay spectralDelay;
    
    std::atomic<float>* Xpanse_on = nullptr;
    std::atomic<float>* Xpanse_WD = nullptr;
    
    std::atomic<float>* Xpanse_Gain = nullptr;
    std::atomic<float>* Type = nullptr;
    float lastGain;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayXpanseEffectAudioProcessor);
};
