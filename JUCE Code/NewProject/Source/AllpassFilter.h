/*
  ==============================================================================

    AllpassFilter.h
    Created: 28 Apr 2024 11:38:47pm
    Author:  Kevin Brennan

    A class to implement a Linkwitz Riley allpass filter using juce::dsp
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class AllpassFilter{
public:
    
    AllpassFilter(){};
    
    ~AllpassFilter(){};
    
    //Pre playback initialisation
    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        Spec.sampleRate = sampleRate;
        Spec.maximumBlockSize = samplesPerBlock;
        Spec.numChannels = 2;
        
        LRF_APF.setType(juce::dsp::LinkwitzRileyFilterType::allpass);
        LRF_APF.setCutoffFrequency(cutoffFrequency);
        LRF_APF.prepare(Spec);
    }
    
    //Process the buffer through the filter
    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        
        LRF_APF.process(context);
    };
    
    //Set the cutoff frequency of the filter
    void setCutoffFrequency(float newFreq)
    {
        this->cutoffFrequency = newFreq;
        LRF_APF.setCutoffFrequency(this->cutoffFrequency);
    };
    
private:
    
    juce::dsp::ProcessSpec Spec;
    
    juce::dsp::LinkwitzRileyFilter<float> LRF_APF;
    
    float cutoffFrequency = 3000;
};
