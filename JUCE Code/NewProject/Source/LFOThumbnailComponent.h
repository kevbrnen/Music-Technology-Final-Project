/*
  ==============================================================================

    LFOThumbnailComponent.h
    Created: 12 Apr 2024 3:16:07pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LFOThumbnailComponent  : public juce::Component
{
public:
    LFOThumbnailComponent(): thumbnailCache(1), thumbnail(512, formatManager, thumbnailCache)
    {
        formatManager.registerBasicFormats();
        
        getAudioBufferFromFile();
    }

    ~LFOThumbnailComponent() override{}

    void paint (juce::Graphics& g) override
    {
         
    }

    void resized() override {}
    
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
        }
        else
        {
            LFO.setSize(1, 256);
            LFO.clear();
        }
        
        thumbnail.setSource(&LFO, 48000, 0);
    }

private:
    juce::AudioFormatManager formatManager;   
    juce::AudioThumbnailCache thumbnailCache;
    juce::AudioThumbnail thumbnail;
    juce::AudioBuffer<float> LFO;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOThumbnailComponent)
};
