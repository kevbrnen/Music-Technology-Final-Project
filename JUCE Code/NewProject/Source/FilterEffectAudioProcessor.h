/*
  ==============================================================================

    FilterEffectAudioProcessor.h
    Created: 1 Apr 2024 11:55:01pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ProcessorBase.h"
#include "LowpassFilter.h"

class FilterEffectAudioProcessor  : public ProcessorBase
{
public:
    FilterEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts) : Filter_Parameters(vts)
    {
        FilterCutoffFrequency = Filter_Parameters.getRawParameterValue("cutoff_frequency");
        Filt_on = Filter_Parameters.getRawParameterValue("filter_toggle");
    };
    
    ~FilterEffectAudioProcessor(){};
    
    
    const juce::String getName() const override {return "Filter";};
    
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        LPF.setSpec(pluginSpec);
        LPF.setCutoff(750);
    };
    

    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        auto effectOn = Filt_on->load();
        if(effectOn != 0.0f)
        {
            //Get and set new cutoff frequency
            const auto NewCutoffFrequency = FilterCutoffFrequency->load();
            LPF.setCutoff(NewCutoffFrequency);
            LPF.process(buffer);
        }
    };

    
    void initSpec(juce::dsp::ProcessSpec spec)
    {
        pluginSpec = spec;
    };
    
//    void getStateInformation(juce::MemoryBlock& destData)
//    {
//        std::unique_ptr<juce::XmlElement> GlobalXml (Filter_Parameters.state.createXml());
//        
//        copyXmlToBinary(*GlobalXml, destData);
//    };
//    
//    void setStateInformation(const void* data, int sizeInBytes)
//    {
//        std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
//        
//        if (xmlState.get() != nullptr)
//        {
//            Filter_Parameters.state = juce::ValueTree::fromXml (*xmlState);
//        }
//    };
    
    juce::AudioProcessorValueTreeState& Filter_Parameters;
    
private:
    
    juce::dsp::ProcessSpec pluginSpec;
    LowpassFilter LPF;
    std::atomic<float>* FilterCutoffFrequency = nullptr;
    std::atomic<float>* Filt_on = nullptr;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterEffectAudioProcessor);
};
