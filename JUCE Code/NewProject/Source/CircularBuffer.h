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
#include <cmath>

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
        float p0 = circularBuffer.getSample(channel, (((delayedSampleIndex + 1) + maxBufferSize) % maxBufferSize));
        float p1 = circularBuffer.getSample(channel, delayedSampleIndex);
        float x = delay_frac;
        float p2 = circularBuffer.getSample(channel, (((delayedSampleIndex - 1) + maxBufferSize) % maxBufferSize));
        float p3 = circularBuffer.getSample(channel, (((delayedSampleIndex - 2) + maxBufferSize) % maxBufferSize));
        
        float delayedSample = ((-0.5*p0 + 1.5*p1 - 1.5*p2 + 0.5*p3) * pow(x, 3.0)) +
                                ((p0 - 2.5*p1 + 2*p2 - 0.5*p3) * pow(x, 2.0)) +
                                    ((-0.5*p0 + 0.5*p2) * x) + p1;
        
        
        
        return &delayedSample;
        //return &sample_int;
    };
    
    //Method to get buffer of N samples from the circular buffer
    void getDelayedBufferWithRamp(int channel, float delayTimeMs, int size, bool fade_in_out, int old_new)
    {
        delayedBuff.setSize(2, size, false, false, false);
        delayedBuff.clear();
        
        int delayTimeSamples = (sampleRate/1000)*delayTimeMs;
        circBuffIndex = (((delayTimeSamples - 1 - size) + maxBufferSize) % maxBufferSize);
        delayedBuff.addFrom(old_new, 0, circularBuffer, channel, circBuffIndex, size);
        delayedBuff.reverse(channel, 0, size);
        
        if(fade_in_out) // Fade in
        {
            delayedBuff.applyGainRamp(0, size, 0, 1);
        }
        else // Fade out
        {
            delayedBuff.applyGainRamp(0, size, 1, 0);
        }
    };
    
    void combineAndAddBuffers(int channel)
    {
        auto size = delayedBuff.getNumSamples();
        
        for(int i = 0; i < size; ++i)
        {
            circularBuffer.setSample(channel, (circBuffIndex + i) % maxBufferSize, (delayedBuff.getSample(0, i) + delayedBuff.getSample(1, i)));
        }
        
    };
    
private:
    int currentBufferIndex;
    int lastBufferIndex;
    int maxBufferSize;
    int sampleRate;
    int circBuffIndex;
    juce::AudioBuffer<float> circularBuffer;
    juce::AudioBuffer<float> delayedBuff;
};
