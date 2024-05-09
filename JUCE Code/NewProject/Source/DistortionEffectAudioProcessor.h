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
#include "VariableFilter.h"


class DistortionEffectAudioProcessor  : public ProcessorBase
{
public:
    DistortionEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts): Distortion_Parameters(vts), distort(vts)
    {
        //Get variables from APVTS for Parameters
        Dist_on = Distortion_Parameters.getRawParameterValue("distortion_toggle");
        Dist_WD = Distortion_Parameters.getRawParameterValue("distortion_wetdry");
        Dist_Gain = Distortion_Parameters.getRawParameterValue("distortion_gain");
        
        Dist_preCutoff = Distortion_Parameters.getRawParameterValue("distortion_pre_cutoff");
        Dist_preRes = Distortion_Parameters.getRawParameterValue("distortion_pre_resonance");
        Dist_postCutoff = Distortion_Parameters.getRawParameterValue("distortion_post_cutoff");
        Dist_postRes = Distortion_Parameters.getRawParameterValue("distortion_post_resonance");
    };
    
    ~DistortionEffectAudioProcessor(){};
    
    const juce::String getName() const override{return "Distortion";};
    
    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        //Setup Spec for variable filter implementations
        pluginSpec.sampleRate = sampleRate;
        pluginSpec.maximumBlockSize = samplesPerBlock;
        pluginSpec.numChannels = 2;
        
        //Setup variable filters
        preFilt.prepareToPlay(sampleRate, samplesPerBlock);
        preFilt.setType(prefilterType);
        
        postFilt.prepareToPlay(sampleRate, samplesPerBlock);
        postFilt.setType(postfilterType);
        
        
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
            
// Pre Filter
            //Check if the type of filter has changed, if so update the type
            auto pre_type_index = Distortion_Parameters.getRawParameterValue("pre_filter_type");
            if((int)*pre_type_index != prefilterType)
            {
                this->prefilterType = *pre_type_index;
                preFilt.setType(this->prefilterType);
            }
            //Get and set resonance
            auto preRes = Dist_preRes->load();
            preFilt.setResonance(preRes);
            //Get and set cutoff frequency
            const auto PreCutoffFrequency = Dist_preCutoff->load();
            preFilt.setCutoffFrequency(PreCutoffFrequency);
            //Process the block
            preFilt.processBlock(buffer);
            
            
            
// Distortion Processing using variable distortion and currently set type
            distort.processBlock(buffer);
            
            
            
// Post Filter
            //Check if the type of filter has changed, if so update the type
            auto post_type_index = Distortion_Parameters.getRawParameterValue("post_filter_type");
            if((int)*post_type_index != postfilterType)
            {
                this->postfilterType = *post_type_index;
                postFilt.setType(this->postfilterType);
            }
            //Get and set resonance
            auto postRes = Dist_postRes->load();
            postFilt.setResonance(postRes);
            //Get and set cutoff frequency
            const auto PostCutoffFrequency = Dist_postCutoff->load();
            postFilt.setCutoffFrequency(PostCutoffFrequency);
            //Process the block
            postFilt.processBlock(buffer);
            
        
            
// Wet Dry
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
            
// Gain
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
    //Filter variables
    juce::dsp::ProcessSpec pluginSpec;
    VariableFilter preFilt, postFilt;
    
    std::atomic<float>* Dist_on = nullptr;
    std::atomic<float>* Dist_WD = nullptr;
    
    std::atomic<float>* Dist_Gain = nullptr;
    float lastGain;
    
    std::atomic<float>* Dist_preCutoff = nullptr;
    std::atomic<float>* Dist_preRes = nullptr;
    std::atomic<float>* Dist_postCutoff = nullptr;
    std::atomic<float>* Dist_postRes = nullptr;
    
    VariableDistortion distort;
    
    int prefilterType = 0;
    int postfilterType = 0;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistortionEffectAudioProcessor);
};
