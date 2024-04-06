/*
  ==============================================================================

    CircularBuffer.h
    Created: 6 Apr 2024 7:42:40pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once

#include <vector>
#include <math.h>

class CircularBuffer
{
public:
    void initBuffer(int channels, int maxBufferSize, int sampleRate)
    {
        //Set up buffer
        circularBuffer.setSize(channels, maxBufferSize, false, false, false);
        circularBuffer.clear();
        this->currentBufferIndex = 0;
        this->maxBufferSize = maxBufferSize;
        this->sampleRate = sampleRate;
    };
    
    //method to add sample to buffer
    void pushSampleToBuffer(int channel, float sample)
    {
        circularBuffer.setSample(channel, currentBufferIndex, sample);
        
        lastBufferIndex = currentBufferIndex;
        
        this->currentBufferIndex++;
        this->currentBufferIndex %= maxBufferSize;
    };
    
    //method to get sample from buffer at given delay time with fractional interpolation (by channel)
    float* getDelayedSample(int channel, float delayTimeMs)
    {
        float delayInSamples = (sampleRate/1000)*delayTimeMs;
        
        int delay_int = (int)delayInSamples;
        float delay_frac = delayInSamples - delay_int;
        
        //Getting delayed sample index
        int delayedSampleIndex = ((lastBufferIndex - delay_int) + maxBufferSize) % maxBufferSize;
        
        //Integer sample and sample to interpolate to
        float sample_int = circularBuffer.getSample(channel, delayedSampleIndex);
        float sample_nextIndex = circularBuffer.getSample(channel, (((delayedSampleIndex - 1) + maxBufferSize) % maxBufferSize));
        
        float delayedSample = ((1 - delay_frac) * sample_int) + (delay_frac * sample_nextIndex);
        
        return &delayedSample;
        //return &sample_int;
    };
    
private:
    int currentBufferIndex;
    int lastBufferIndex;
    int maxBufferSize;
    int sampleRate;
    juce::AudioBuffer<float> circularBuffer;
};
