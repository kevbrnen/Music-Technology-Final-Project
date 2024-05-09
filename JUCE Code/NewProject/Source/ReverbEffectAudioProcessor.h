/*
  ==============================================================================

    ReverbEffectAudioProcessor.h
    Created: 1 May 2024 2:23:37pm
    Author:  Kevin Brennan

    A Class for processing audio for the Reverb effect
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SchroederReverberator.h"
#include "DelayingAllpassReverberator.h"
#include "FDNReverberator.h"
#include "CircularBuffer.h"
#include "VariableFilter.h"


class ReverbEffectAudioProcessor  : public ProcessorBase
{
public:
    ReverbEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts): Reverb_Parameters(vts), schroederProcessor(vts), delApProcessor(vts), fdnProcessor(vts)
    {
        //Get variables from APVTS for Parameters
        Reverb_on = Reverb_Parameters.getRawParameterValue("reverb_toggle");
        Reverb_WD = Reverb_Parameters.getRawParameterValue("reverb_wetdry");
        Reverb_Gain = Reverb_Parameters.getRawParameterValue("reverb_gain");
        Type = Reverb_Parameters.getRawParameterValue("reverb_type");
        PreDel = Reverb_Parameters.getRawParameterValue("reverb_pre_delay");
        
        preCutoff = Reverb_Parameters.getRawParameterValue("reverb_pre_cutoff");
        preRes = Reverb_Parameters.getRawParameterValue("reverb_pre_resonance");
    };
    
    ~ReverbEffectAudioProcessor(){};
    
    const juce::String getName() const override{return "Reverb";};
    
    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        schroederProcessor.prepareToPlay(sampleRate, samplesPerBlock);
        delApProcessor.prepareToPlay(sampleRate, samplesPerBlock);
        fdnProcessor.prepareToPlay(sampleRate, samplesPerBlock);
        
        preDelay.initBuffer(2, (sampleRate/1000)*3000, (int)sampleRate);
        lastDelay = PreDel->load();
        delayTimeSmoothing.reset(sampleRate, 0.05);
        
        //Setup variable filter
        preFilt.prepareToPlay(sampleRate, samplesPerBlock);
        preFilt.setType(prefilterType);
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        auto effectOn = Reverb_on->load();
        
        if(effectOn != 0.0f)
        {
            juce::AudioBuffer<float> dry(buffer.getNumChannels(), buffer.getNumSamples());
            dry.copyFrom(0, 0, buffer, 0, 0, buffer.getNumSamples());
            dry.copyFrom(1, 0, buffer, 1, 0, buffer.getNumSamples());
            
            
//Pre Gain
            //Apply pre-gain to the buffer, before the processing
            auto preGain = juce::Decibels::decibelsToGain(*Reverb_Parameters.getRawParameterValue("reverb_pre_gain") + 0.0);
            buffer.applyGain(preGain);
            
            
// Pre Filter
            //Check if the type of filter has changed, if so update the type
            auto pre_type_index = Reverb_Parameters.getRawParameterValue("reverb_pre_filter_type");
            if((int)*pre_type_index != prefilterType)
            {
                this->prefilterType = *pre_type_index;
                preFilt.setType(this->prefilterType);
            }
            //Get and set resonance
            auto preResval = preRes->load();
            preFilt.setResonance(preResval);
            //Get and set cutoff frequency
            const auto PreCutoffFrequency = preCutoff->load();
            preFilt.setCutoffFrequency(PreCutoffFrequency);
            //Process the block
            preFilt.processBlock(buffer);
            

//Pre Delay
            auto C = 0.6f;
            for(int i = 0; i < buffer.getNumSamples(); ++i)
            {
                auto newDelayTime = PreDel->load();
                
                //Set the next target to smooth to as the new delay time
                delayTimeSmoothing.setTargetValue(newDelayTime);
                
                //loop through channels and process
                for(int channel = 0; channel < 2 ; ++channel)
                {
                    auto* inData = buffer.getReadPointer(channel);
                    auto* outData = buffer.getWritePointer(channel);
                    
                    //Push sample to buffer with feedback, determined by feedback amount
                    preDelay.pushSampleToBuffer(channel, inData[i]);
                        
                    //Get the smoothed delay time value
                    auto time = delayTimeSmoothing.getNextValue();
                    
                    //Get the delayed sample at the last delay time and the delayed sample at the new delay time
                    auto delayedSample_oldVal = preDelay.getDelayedSample(channel, lastDelay);
                    auto delayedSample_newVal = preDelay.getDelayedSample(channel, time);
                        
                    //Interpolate between old and new delayed samples, compute wet/dry combination and add to output
                    outData[i] = (((*delayedSample_newVal * (1-C)) + (*delayedSample_oldVal * C)));
                        
                    lastDelay = time;
                    feedback[channel] = outData[i];
                }
            }
            
            //Processing depending on selected type
            if(Type->load() == 0)
            {
                schroederProcessor.processBlock(buffer);
            }
            else if(Type->load() == 1)
            {
                delApProcessor.processBlock(buffer);
            }
            else if(Type->load() == 2)
            {
                fdnProcessor.processBlock(buffer);
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
    std::atomic<float>* PreDel = nullptr;
    std::atomic<float>* Type = nullptr;
    float lastGain = 0;
    float lastDelay = 0;
    juce::LinearSmoothedValue<float> delayTimeSmoothing {500.0f};
    float feedback[2] = {0.0, 0.0};
    
    VariableFilter preFilt;
    std::atomic<float>* preCutoff = nullptr;
    std::atomic<float>* preRes = nullptr;
    int prefilterType = 0;
    
    SchroederReverberator schroederProcessor;
    DelayingAllpassReverberator delApProcessor;
    FDNReverberator fdnProcessor;
    
    CircularBuffer preDelay;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbEffectAudioProcessor);
};
