/*
  ==============================================================================

    PhaserEffectAudioProcessor.h
    Created: 30 Apr 2024 11:45:50am
    Author:  Kevin Brennan
 
    A class for processing audio for the Phaser effect
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <cmath>
#include "ProcessorBase.h"
#include "MXR100_Phaser.h"


class PhaserEffectAudioProcessor  : public ProcessorBase
{
public:
    PhaserEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts) : Phaser_Parameters(vts), phaser(vts)
    {
        Phaser_on = Phaser_Parameters.getRawParameterValue("phaser_toggle");
    };
    
    ~PhaserEffectAudioProcessor(){};
    
    const juce::String getName() const override{return "Phaser";};
    
    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        pluginSpec.sampleRate = sampleRate;
        pluginSpec.maximumBlockSize = samplesPerBlock;
        pluginSpec.numChannels = 2;
        
        phaser.prepareToPlay(sampleRate, samplesPerBlock);

    };
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
        {
            auto effectOn = Phaser_on->load();
            
            if(effectOn != 0.0f)
            {
                phaser.processBlock(buffer);
                
                const auto NewGain = juce::Decibels::decibelsToGain(*Phaser_Parameters.getRawParameterValue("phaser_gain") + 0.0);
                
                if(NewGain != lastGain)
                {
                    //Smooth gain to remove artefacts
                    buffer.applyGainRamp(0, buffer.getNumSamples(), lastGain, NewGain);
                    lastGain = NewGain;
                }
            }
        };
    
    juce::AudioProcessorValueTreeState& Phaser_Parameters;
    
private:
    
    juce::dsp::ProcessSpec pluginSpec;
    
    MXR100_Phaser phaser;
   
    std::atomic<float>* Phaser_on = nullptr;
    
    float lastGain;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PhaserEffectAudioProcessor);
};
