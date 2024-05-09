/*
  ==============================================================================

    FilterEffectAudioProcessor.h
    Created: 1 Apr 2024 11:55:01pm
    Author:  Kevin Brennan

    Class to process audio for the Filter Effect
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ProcessorBase.h"
#include "VariableFilter.h"
#include "LFOProcessor.h"

class FilterEffectAudioProcessor  : public ProcessorBase
{
public:
    FilterEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts) : Filter_Parameters(vts)
    {
        //Variables from APVTS for Filter parameters
        FilterCutoffFrequency = Filter_Parameters.getRawParameterValue("cutoff_frequency");
        FilterResonance = Filter_Parameters.getRawParameterValue("resonance");
        Filt_on = Filter_Parameters.getRawParameterValue("filter_toggle");
        FilterGain = Filter_Parameters.getRawParameterValue("filter_gain");
        
        
        FilterCutoffFrequency2 = Filter_Parameters.getRawParameterValue("cutoff_frequency2");
        FilterResonance2 = Filter_Parameters.getRawParameterValue("resonance2");
        Filt_on2 = Filter_Parameters.getRawParameterValue("filter_toggle2");
        
        
        Filter_LFO_on = Filter_Parameters.getRawParameterValue("filter_LFO_toggle");
        Filter_LFO_Type = Filter_Parameters.getRawParameterValue("filter_lfo_type");
        Filter_LFO_Speed = Filter_Parameters.getRawParameterValue("filter_lfo_speed");
        Filter_LFO_ModWidth = Filter_Parameters.getRawParameterValue("filter_lfo_modwidth");
        
        Filter1_Cutoff_LFO = Filter_Parameters.getRawParameterValue("filter1_cutoff_lfo");
        Filter1_Res_LFO = Filter_Parameters.getRawParameterValue("filter1_res_lfo");
        Filter2_Cutoff_LFO = Filter_Parameters.getRawParameterValue("filter2_cutoff_lfo");
        Filter2_Res_LFO = Filter_Parameters.getRawParameterValue("filter2_res_lfo");
    };
    
    ~FilterEffectAudioProcessor(){};
    
    
    const juce::String getName() const override {return "Filter";};
    
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        //Setup Spec for variable filter implementation
        pluginSpec.sampleRate = sampleRate;
        pluginSpec.maximumBlockSize = samplesPerBlock;
        pluginSpec.numChannels = 2;
        
        //Setup variable filter
        varFilt.prepareToPlay(sampleRate, samplesPerBlock);
        varFilt.setType(filterType);

        varFilt2.prepareToPlay(sampleRate, samplesPerBlock);
        varFilt2.setType(filterType2);
        
        //Gain
        lastGain = juce::Decibels::decibelsToGain(*Filter_Parameters.getRawParameterValue("filter_gain") + 0.0);
        
        
        LFO.prepareToPlay(sampleRate, samplesPerBlock);
        
        LastLFOSpeed = Filter_LFO_Speed->load() * samplesPerBlock;
        LastLFOMW = Filter_LFO_ModWidth->load();
        
        LFO.setSpeed(LastLFOSpeed);
        LFO.setMW(LastLFOMW);
    };
    

    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        auto effectOn = Filt_on->load(); //Process if effect on
        if(effectOn != 0.0f)
        {
            
            //Check if the type of filter has changed, if so update the type
            auto type_index = Filter_Parameters.getRawParameterValue("filter_types");
            if((int)*type_index != filterType)
            {
                this->filterType = *type_index;
                varFilt.setType(this->filterType);
            }
            
            auto type_index2 = Filter_Parameters.getRawParameterValue("filter_types2");
            if((int)*type_index2 != filterType2)
            {
                this->filterType2 = *type_index2;
                varFilt2.setType(this->filterType2);
            }
            
//LFO / NON LFO processing
            auto LFOOn = Filter_LFO_on->load();
            auto filt2On = Filt_on2->load();
            auto res = FilterResonance->load();
            auto NewCutoffFrequency = FilterCutoffFrequency->load();
            auto res2 = FilterResonance2->load();
            auto NewCutoffFrequency2 = FilterCutoffFrequency2->load();
            
            if(LFOOn != 0.f)
            {
        //Filter 1 LFO processing
                //Check if LFO speed was updated
                auto newSpeed = Filter_LFO_Speed->load() * pluginSpec.maximumBlockSize;
                if(newSpeed != LastLFOSpeed)
                {
                    LFO.setSpeed(newSpeed);
                    LastLFOSpeed = newSpeed;
                }
                
                //Check if LFO mod width was updated
                auto newMW = Filter_LFO_ModWidth->load();
                if(newMW != LastLFOMW)
                {
                    LFO.setMW(newMW);
                    LastLFOMW = newMW;
                }
                auto lfoVal = LFO.getNextLFOVal(Filter_LFO_Type->load());
                
                //Filter 1 Cutoff
                auto cutAmt1 = Filter1_Cutoff_LFO->load();
                
                if(cutAmt1 > 0.0f)
                {
                    auto modRange = (20000 * lfoVal * cutAmt1)+1.f;
                    NewCutoffFrequency += modRange;
                    NewCutoffFrequency = std::min(NewCutoffFrequency, 20000.f);
                    NewCutoffFrequency = std::max(NewCutoffFrequency, 1.f);
                }
                
                //Filter 1 Resonance
                auto resAmt1 = Filter1_Res_LFO->load();
                
                if(resAmt1 > 0.0f)
                {
                    res *= (lfoVal * resAmt1);
                    
                    res = std::max(0.001f, std::min(1.0f, res));
                }
            
                varFilt.setCutoffFrequency(NewCutoffFrequency);
                varFilt.setResonance(res);
                varFilt.processBlock(buffer);
                
        //Filter 2 LFO Processing
                if(filt2On != 0.0f)
                {
                    auto cutAmt2 = Filter2_Cutoff_LFO->load();
                    
                    if(cutAmt2 > 0.0f)
                    {
                        auto modRange = (20000 * lfoVal * cutAmt2) + 1.f;
                        NewCutoffFrequency2 += modRange;
                        NewCutoffFrequency2 = std::min(NewCutoffFrequency2, 20000.f);
                        NewCutoffFrequency2 = std::max(NewCutoffFrequency2, 1.f);
                    }
                    
                    //Filter 2 Resonance
                    auto resAmt2 = Filter2_Res_LFO->load();
                    
                    if(resAmt2 > 0.0f)
                    {
                        res2 *= (lfoVal * resAmt2);
                        
                        res2 = std::max(0.001f, std::min(1.0f, res2));
                    }
                
                    
                    //Set resonance
                    varFilt2.setResonance(res2);
                    //Set cutoff frequency
                    varFilt2.setCutoffFrequency(NewCutoffFrequency2);
                    //Process the block
                    varFilt2.processBlock(buffer);
                }
                
            }
            else //Non LFO Processing
            {
                //Get and set resonance and cutoff freqs
                varFilt.setResonance(res);
                //Get and set cutoff frequency
                varFilt.setCutoffFrequency(NewCutoffFrequency);
                
                varFilt.processBlock(buffer);
                
                if(filt2On != 0.0f)
                {
                    //Set resonance
                    varFilt2.setResonance(res2);
                    //Set cutoff frequency
                    varFilt2.setCutoffFrequency(NewCutoffFrequency2);
                    //Process the block
                    varFilt2.processBlock(buffer);
                }
            }
        
            
            //Filter Effect Gain
            const auto NewGain = juce::Decibels::decibelsToGain(*Filter_Parameters.getRawParameterValue("filter_gain") + 0.0);
            
            if(NewGain != lastGain)
            {
                //Smooth gain to remove artefacts
                buffer.applyGainRamp(0, buffer.getNumSamples(), lastGain, NewGain);
                lastGain = NewGain;
            }
            
        }
    };


    
    juce::AudioProcessorValueTreeState& Filter_Parameters;
    
private:
    //Filter variables
    juce::dsp::ProcessSpec pluginSpec;
    VariableFilter varFilt, varFilt2;
    
    std::atomic<float>* FilterCutoffFrequency = nullptr;
    std::atomic<float>* FilterResonance = nullptr;
    std::atomic<float>* FilterCutoffFrequency2 = nullptr;
    std::atomic<float>* FilterResonance2 = nullptr;
    std::atomic<float>* Filt_on = nullptr;
    std::atomic<float>* Filt_on2 = nullptr;
    std::atomic<float>* FilterGain = nullptr;
    float lastGain;
    
    std::atomic<float>* Filter_LFO_on = nullptr;
    std::atomic<float>* Filter_LFO_Type = nullptr;
    std::atomic<float>* Filter_LFO_Speed = nullptr;
    std::atomic<float>* Filter_LFO_ModWidth = nullptr;
    
    std::atomic<float>* Filter1_Cutoff_LFO = nullptr;
    std::atomic<float>* Filter1_Res_LFO = nullptr;
    std::atomic<float>* Filter2_Cutoff_LFO = nullptr;
    std::atomic<float>* Filter2_Res_LFO = nullptr;
    float LastLFOSpeed = 0;
    float LastLFOMW = 0;
    
    int filterType = 0;
    int filterType2 = 0;
    
    LFOProcessor LFO;
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterEffectAudioProcessor);
};
