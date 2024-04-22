/*
  ==============================================================================

    WaveformThumbnailComponent.h
    Created: 20 Apr 2024 10:41:57pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformThumbnailComponent  : public juce::Component
{
public:
    WaveformThumbnailComponent(): thumbnailCache(1), thumbnail(512, formatManager, thumbnailCache)
    {
        formatManager.registerBasicFormats();
        
        getAudioBufferFromFile(BinaryData::ChurchIR1_wav, BinaryData::ChurchIR1_wavSize);
    }

    ~WaveformThumbnailComponent() override{}

    void paint (juce::Graphics& g) override
    {
        juce::Rectangle<int> thumbnailBounds (getLocalBounds());
        
        g.setColour(juce::Colours::black);
        g.fillRect(thumbnailBounds);
        g.setColour(juce::Colours::white);
        
        thumbnail.drawChannels(g, thumbnailBounds, 0.0, thumbnail.getTotalLength(), 1.0f);
    }

    void resized() override {}
    
    
    void setFile(const char* data, int Size)
    {
        getAudioBufferFromFile(data, Size);
    }
    
    
    void getAudioBufferFromFile(const char* data, int Size)
    {
        auto input = std::make_unique<juce::MemoryInputStream>(data, Size, false);
        
        auto* reader = formatManager.createReaderFor(std::move(input));
        
        if(reader)
        {
            auto numSamples = static_cast<int>(reader->lengthInSamples);
            
            IR.setSize(1, numSamples);
            IR.clear();
            
            reader->read(&IR, 0, numSamples, 0, true, true);
        }
        else
        {
            IR.setSize(1, 256);
            IR.clear();
        }
        
        thumbnail.setSource(&IR, 48000, 0);
    }

private:
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;
    juce::AudioBuffer<float> IR;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformThumbnailComponent)
};
