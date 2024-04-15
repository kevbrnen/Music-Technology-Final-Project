/*
  ==============================================================================

    LFOProcessor.h
    Created: 11 Apr 2024 10:26:28pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "LFOThumbnailComponent.h"

class LFOProcessor
{
public:
    LFOProcessor() : currentIndex(0), sampleRate(48000)
    {
        formatManager.registerBasicFormats();
        
        getAudioBufferFromFile();
    
    };
    ~LFOProcessor(){};
    
    float getNextLFOVal()
    {
        if(LFO.getNumSamples() == 0)
        {
            return 0;
        }
        else
        {
            auto val = LFO.getSample(0,currentIndex);
            
            currentIndex++;
            currentIndex = currentIndex % LFO.getNumSamples();
            
            return val;
        }
    };
    
    void getAudioBufferFromFile()
    {
        auto input = std::make_unique<juce::MemoryInputStream>(BinaryData::Birds_LFO_wav, BinaryData::Birds_LFO_wavSize, false);
        
        auto* reader = formatManager.createReaderFor(std::move(input));
        
        if(reader)
        {
            auto numSamples = static_cast<int>(reader->lengthInSamples);
            
            LFO.setSize(1, numSamples);
            LFO.clear();
            
            reader->read(&LFO, 0, numSamples, 0, true, true);
            
            normaliseBuffer(LFO);
        }
        else
        {
            LFO.setSize(1, 256);
            LFO.clear();
        }
    }
   
    juce::AudioFormatManager formatManager;
private:
    
    void normaliseBuffer(juce::AudioBuffer<float>& buffer)
    {
        // Find the maximum absolute value within the buffer
        float maxAbsValue = 0.0f;
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            const float* channelData = buffer.getReadPointer(channel);
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                float absValue = std::abs(channelData[sample]);
                if (absValue > maxAbsValue)
                    maxAbsValue = absValue;
            }
        }

        // Scale all samples proportionally so that the maximum absolute value becomes 1
        if (maxAbsValue > 0.0f)
        {
            float scaleFactor = 1.0f / maxAbsValue;
            buffer.applyGain(scaleFactor * -1.0f);
        }
    }

    juce::AudioBuffer<float> LFO;
    int currentIndex;
    int sampleRate;
};
