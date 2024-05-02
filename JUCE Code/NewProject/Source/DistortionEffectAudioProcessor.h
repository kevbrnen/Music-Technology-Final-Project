/*
  ==============================================================================

    DistortionEffectAudioProcessor.h
    Created: 2 May 2024 3:16:10pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "VariableDistortion.h"


class DistortionEffectAudioProcessor  : public ProcessorBase
{
public:
    DistortionEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts): Distortion_Parameters(vts), distort(vts)
    {
        //Get variables from APVTS for Parameters
        Dist_on = Distortion_Parameters.getRawParameterValue("distortion_toggle");
        Dist_WD = Distortion_Parameters.getRawParameterValue("distortion_wetdry");
        Dist_Gain = Distortion_Parameters.getRawParameterValue("distortion_gain");
    };
    
    ~DistortionEffectAudioProcessor(){};
    
    const juce::String getName() const override{return "Distortion";};
    
    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        distort.prepareToPlay(sampleRate, samplesPerBlock);

    };
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        auto effectOn = Dist_on->load();
        
        if(effectOn != 0.0f)
        {
            juce::AudioBuffer<float> dry(buffer.getNumChannels(), buffer.getNumSamples());
            dry.copyFrom(0, 0, buffer, 0, 0, buffer.getNumSamples());
            dry.copyFrom(1, 0, buffer, 1, 0, buffer.getNumSamples());
            
            
//            if(Type->load() == 0)
//            {
//
//            }
            
            distort.processBlock(buffer);
            

            
            auto wetDry = Dist_WD->load();

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
            
            const auto NewGain = juce::Decibels::decibelsToGain(*Distortion_Parameters.getRawParameterValue("distortion_gain") + 0.0);
            
            if(NewGain != lastGain)
            {
                //Smooth gain to remove artefacts
                buffer.applyGainRamp(0, buffer.getNumSamples(), lastGain, NewGain);
                lastGain = NewGain;
            }
        }
    };
    
    juce::AudioProcessorValueTreeState& Distortion_Parameters;
    
private:
    
    std::atomic<float>* Dist_on = nullptr;
    std::atomic<float>* Dist_WD = nullptr;
    
    std::atomic<float>* Dist_Gain = nullptr;
    float lastGain;
    
    VariableDistortion distort;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistortionEffectAudioProcessor);
};
