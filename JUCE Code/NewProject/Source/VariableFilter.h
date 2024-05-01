/*
  ==============================================================================

    VariableFilter.h
    Created: 28 Apr 2024 11:38:30pm
    Author:  Kevin Brennan

    A class for a variable filter
    One class containing four different filter types
    Cutoff frequency and resonance can be controlled as well as what type of filter is being used
     //0 = allpass (Linkwitz Riley)
     //1 = lowpass (TPT)
     //2 = bandpass (TPT)
     //3 = highpass (TPT)
 
    Had to add a separate Allpass filter implementation as the juce State Variable TPT filter has no allpass type
    Allpass filter is defined in "AllpassFilter.h"
  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "AllpassFilter.h"

class VariableFilter
{
public:
    VariableFilter(){};
    ~VariableFilter(){};
    
    //Prepare to play to set up any pre-playback variables
    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        Spec.sampleRate = sampleRate;
        Spec.maximumBlockSize = samplesPerBlock;
        Spec.numChannels = 2;
        
        //Initialising TPT filter
        filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        filter.setCutoffFrequency(cutoffFrequency);
        filter.setResonance(resonance);
        filter.prepare(Spec);
        
        //Initialising Allpass filter
        allpassFilter.prepareToPlay(sampleRate, samplesPerBlock);
    };
    
    //Process the buffer through the current filter
    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        if(currentType == 0)
        {
            allpassFilter.processBlock(buffer);
        }
        else
        {
            juce::dsp::AudioBlock<float> block(buffer);
            juce::dsp::ProcessContextReplacing<float> context(block);
            filter.process(context);
        }
    };
    
    void setType(int type)
    {
        //Set the type of filter to use
        //0 = allpass
        //1 = lowpass
        //2 = bandpass
        //3 = highpass
        switch(type)
        {
            case 0:
                this->currentType = type;
                break;
            case 1:
                filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
                this->currentType = type;
                break;
            case 2:
                filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
                this->currentType = type;
                break;
            case 3:
                filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
                this->currentType = type;
                break;
            default:
                break;
        }
    };
    
    //Set the cutoff of the filter
    void setCutoffFrequency(float newCutoff)
    {
        this->cutoffFrequency = newCutoff;
        if(currentType == 0)
        {
            allpassFilter.setCutoffFrequency(this->cutoffFrequency);
        }
        else
        {
            filter.setCutoffFrequency(this->cutoffFrequency);
        }
    };

    //Set the resonance of the filter
    void setResonance(float newRes)
    {
        this->resonance = newRes;
        
        filter.setResonance(this->resonance);
    };
    
private:
    juce::dsp::ProcessSpec Spec;
    
    juce::dsp::StateVariableTPTFilter<float> filter;
    AllpassFilter allpassFilter;
    
    int currentType;
    
    float cutoffFrequency = 3000;
    float resonance = 0.1;
};
