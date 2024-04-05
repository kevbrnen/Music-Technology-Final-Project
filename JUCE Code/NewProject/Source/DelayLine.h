/*
  ==============================================================================

    DelayLine.h
    Created: 4 Apr 2024 10:30:21pm
    Author:  Kevin Brennan

    Delay Line Class
  ==============================================================================
*/

#pragma once

#include <vector>
#include "JuceHeader.h"

class DelayLine
{
public:
    DelayLine(){ updateDelayLine(); };
    ~DelayLine(){};
    DelayLine(const DelayLine& other) : delayLine(other.delayLine), Spec(other.Spec), delayMs(other.delayMs)
    {
        initDelayLine();
    };
    
    //==============================================================================
    void initDelayLine()
    {
        delayLine.prepare(Spec);
        delayLine.setMaximumDelayInSamples(((48000.0f/1000.0f)*3000.0f));
        setDelayTime(500);
        delayLine.setDelay(this->delaySamples);
        resetDelayLine();
    };
    
    void processSample(int channel, float sample)
    {
        delayLine.pushSample(channel, sample);
    };
    
    float getDelayedSample(int channel)
    {
        return delayLine.popSample(channel, this->delaySamples, true);
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        
        delayLine.process(context);
    };
    
    void setDelayTime(double delayInMs)
    {
        this->delayMs = delayInMs;
        auto delayS = (Spec.sampleRate/1000) * this->delayMs;
        this->delaySamples = delayS;
    };
    
    void setSpec(const juce::dsp::ProcessSpec& spec)
    {
        this->Spec = spec;
        updateDelayLine();
    };
    
    float getDelaySamples(){ return this->delaySamples; };
    
    void resetDelayLine(){ delayLine.reset(); };
    
private:
    void updateDelayLine()
    {
        delayLine.prepare(Spec);
    };
    
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Thiran> delayLine;
    
    juce::dsp::ProcessSpec Spec;
    
    float delaySamples;
    double delayMs;
    
};
