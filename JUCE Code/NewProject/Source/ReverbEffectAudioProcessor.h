/*
  ==============================================================================

    ReverbEffectAudioProcessor.h
    Created: 1 May 2024 2:23:37pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class ReverbEffectAudioProcessor  : public ProcessorBase
{
public:
    ReverbEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts): Reverb_Parameters(vts)
    {
        //Get variables from APVTS for Parameters
        Reverb_on = Reverb_Parameters.getRawParameterValue("reverb_toggle");
        Reverb_WD = Reverb_Parameters.getRawParameterValue("reverb_wetdry");
        Reverb_Gain = Reverb_Parameters.getRawParameterValue("reverb_gain");
        Type = Reverb_Parameters.getRawParameterValue("reverb_type");
    };
    
    ~ReverbEffectAudioProcessor(){};
    
    const juce::String getName() const override{return "Reverb";};
    
    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {

    };
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        auto effectOn = Reverb_on->load();
        
        if(effectOn != 0.0f)
        {
            juce::AudioBuffer<float> dry(buffer.getNumChannels(), buffer.getNumSamples());
            dry.copyFrom(0, 0, buffer, 0, 0, buffer.getNumSamples());
            dry.copyFrom(1, 0, buffer, 1, 0, buffer.getNumSamples());
            
            
            if(Type->load() == 0)
            {
             
            }
            else if(Type->load() == 1)
            {
           
            }
            
            auto wetDry = Reverb_WD->load();

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
            
            const auto NewGain = juce::Decibels::decibelsToGain(*Reverb_Parameters.getRawParameterValue("reverb_gain") + 0.0);
            
            if(NewGain != lastGain)
            {
                //Smooth gain to remove artefacts
                buffer.applyGainRamp(0, buffer.getNumSamples(), lastGain, NewGain);
                lastGain = NewGain;
            }
        }
    };
    
    juce::AudioProcessorValueTreeState& Reverb_Parameters;
    
private:
    
    std::atomic<float>* Reverb_on = nullptr;
    std::atomic<float>* Reverb_WD = nullptr;
    
    std::atomic<float>* Reverb_Gain = nullptr;
    std::atomic<float>* Type = nullptr;
    float lastGain;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbEffectAudioProcessor);
};
