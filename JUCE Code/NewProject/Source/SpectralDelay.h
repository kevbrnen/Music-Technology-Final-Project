/*
  ==============================================================================

    SpectralDelay.h
    Created: 28 Apr 2024 11:38:12pm
    Author:  Kevin Brennan

    A class to implement a Spectral Delay effect
 
    Linkwitz Riley filters are used to split the input signal into 4 bands.
    These 4 bands can be delayed by differing amounts with individual gain and feedback
    controls for each band.
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "CircularBuffer.h"

class SpectralDelay{
public:
    SpectralDelay(juce::AudioProcessorValueTreeState& vts): Xpanse_Parameters(vts)
    {
        //Get parameters from value tree
        band1Time = Xpanse_Parameters.getRawParameterValue("spec_band1_time");
        band2Time = Xpanse_Parameters.getRawParameterValue("spec_band2_time");
        band3Time = Xpanse_Parameters.getRawParameterValue("spec_band3_time");
        band4Time = Xpanse_Parameters.getRawParameterValue("spec_band4_time");
        
        fdbk_amt1 = Xpanse_Parameters.getRawParameterValue("spec_band1_fdbk");
        fdbk_amt2 = Xpanse_Parameters.getRawParameterValue("spec_band2_fdbk");
        fdbk_amt3 = Xpanse_Parameters.getRawParameterValue("spec_band3_fdbk");
        fdbk_amt4 = Xpanse_Parameters.getRawParameterValue("spec_band4_fdbk");
        
        cutoff1 = Xpanse_Parameters.getRawParameterValue("spec_cutoff1");
        cutoff2 = Xpanse_Parameters.getRawParameterValue("spec_cutoff2");
        cutoff3 = Xpanse_Parameters.getRawParameterValue("spec_cutoff3");
        
    };
    
    ~SpectralDelay(){};
    
    //Pre-playback initialisation
    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        Spec.sampleRate = sampleRate;
        Spec.maximumBlockSize = samplesPerBlock;
        Spec.numChannels = 2;
        
        //Filter 0 (Lowpass and highpass)
        LRF_LP0.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
        LRF_LP0.setCutoffFrequency(cutoff1->load());
        LRF_LP0.prepare(Spec);
        LRF_HP0.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
        LRF_HP0.setCutoffFrequency(cutoff1->load());
        LRF_HP0.prepare(Spec);
        
        //Filter 1 (Lowpass and highpass)
        LRF_LP1.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
        LRF_LP1.setCutoffFrequency(cutoff2->load());
        LRF_LP1.prepare(Spec);
        LRF_HP1.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
        LRF_HP1.setCutoffFrequency(cutoff2->load());
        LRF_HP1.prepare(Spec);
        
        //Filter 2 (Lowpass and highpass)
        LRF_LP2.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
        LRF_LP2.setCutoffFrequency(cutoff3->load());
        LRF_LP2.prepare(Spec);
        LRF_HP2.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
        LRF_HP2.setCutoffFrequency(cutoff3->load());
        LRF_HP2.prepare(Spec);
        
        //Delay lines for each band
        loDelay1.initBuffer(2, (int)((sampleRate/1000.0f)*maxDel), sampleRate);
        hiDelay1.initBuffer(2, (int)((sampleRate/1000.0f)*maxDel), sampleRate);
        loDelay2.initBuffer(2, (int)((sampleRate/1000.0f)*maxDel), sampleRate);
        hiDelay2.initBuffer(2, (int)((sampleRate/1000.0f)*maxDel), sampleRate);
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        //Get cutoff frequencies
        LRF_LP0.setCutoffFrequency(cutoff1->load());
        LRF_HP0.setCutoffFrequency(cutoff1->load());
        LRF_LP1.setCutoffFrequency(cutoff2->load());
        LRF_HP1.setCutoffFrequency(cutoff2->load());
        LRF_LP2.setCutoffFrequency(cutoff3->load());
        LRF_HP2.setCutoffFrequency(cutoff3->load());
        
            
        for(int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            //Smoothing changes in delay
            auto newDelayTime1 = band1Time->load();
            auto newDelayTime2 = band2Time->load();
            auto newDelayTime3 = band3Time->load();
            auto newDelayTime4 = band4Time->load();
            
            float ht_constant = .3f;
            smoothingFac = std::pow((0.5), (1.f/(Spec.sampleRate * ht_constant)));
            
            float delayTime1 = smoothingFac*(lastDelay1) + (1-smoothingFac)*newDelayTime1;
            lastDelay1 = delayTime1;
            float delayTime2 = smoothingFac*(lastDelay2) + (1-smoothingFac)*newDelayTime2;
            lastDelay2 = delayTime2;
            float delayTime3 = smoothingFac*(lastDelay3) + (1-smoothingFac)*newDelayTime3;
            lastDelay3 = delayTime3;
            float delayTime4 = smoothingFac*(lastDelay4) + (1-smoothingFac)*newDelayTime4;
            lastDelay4 = delayTime4;
            
            for(int channel = 0; channel < 2 ; ++channel)
            {
                auto* inData = buffer.getReadPointer(channel);
                
                auto* outData = buffer.getWritePointer(channel);
        
                //Use lowpass and highpass filters with cutoff1 to separate band 1 from the rest
                auto band1Samp = LRF_LP0.processSample(channel, inData[sample]);
                auto tempBand2 = LRF_HP0.processSample(channel, inData[sample]);
                
                //Use lowpass and highpass filters with cutoff2 to separate band 2 from the rest
                auto band2Samp = LRF_LP1.processSample(channel, tempBand2);
                auto tempBand3 = LRF_HP1.processSample(channel, tempBand2);
                
                //Use lowpass and highpass filters with cutoff3 to separate band 3 and band 4
                auto band3Samp = LRF_LP2.processSample(channel, tempBand3);
                auto band4Samp = LRF_HP2.processSample(channel, tempBand3);
                    

                //Apply gains to the individual bands
                auto band1 = (band1Samp * juce::Decibels::decibelsToGain(*Xpanse_Parameters.getRawParameterValue("spec_band1_gain") + 0.0));
                auto band2 = (band2Samp * juce::Decibels::decibelsToGain(*Xpanse_Parameters.getRawParameterValue("spec_band2_gain") + 0.0));
                auto band3 = (band3Samp * juce::Decibels::decibelsToGain(*Xpanse_Parameters.getRawParameterValue("spec_band3_gain") + 0.0));
                auto band4 = (band4Samp * juce::Decibels::decibelsToGain(*Xpanse_Parameters.getRawParameterValue("spec_band4_gain") + 0.0));

                    
            //Process Delays
                loDelay1.pushSampleToBuffer(channel, (band1 + (fdbk_amt1->load() * feedback[channel][0])));
                hiDelay1.pushSampleToBuffer(channel, (band2 + (fdbk_amt2->load() * feedback[channel][1])));
                loDelay2.pushSampleToBuffer(channel, (band3 + (fdbk_amt3->load() * feedback[channel][2])));
                hiDelay2.pushSampleToBuffer(channel, (band4 + (fdbk_amt4->load() * feedback[channel][3])));
                

                //Get delayed samples and add them to output
                auto delayed_b1 = loDelay1.getDelayedSample(channel, delayTime1);
                outData[sample] = *delayed_b1;
                
                auto delayed_b2 = hiDelay1.getDelayedSample(channel, delayTime2);
                outData[sample] += *delayed_b2;
                    
                auto delayed_b3 = loDelay2.getDelayedSample(channel, delayTime3);
                outData[sample] += *delayed_b3;
                    
                auto delayed_b4 = hiDelay2.getDelayedSample(channel, delayTime4);
                outData[sample] += *delayed_b4;

                //Store feedbacks for next iteration
                feedback[channel][0] = *delayed_b1;
                feedback[channel][1] = *delayed_b2;
                feedback[channel][2] = *delayed_b3;
                feedback[channel][3] = *delayed_b4;
     
            }
        }
    };

    juce::AudioProcessorValueTreeState& Xpanse_Parameters;
private:
    juce::dsp::ProcessSpec Spec;

    CircularBuffer loDelay1;
    CircularBuffer hiDelay1;
    CircularBuffer loDelay2;
    CircularBuffer hiDelay2;
    
    
    juce::dsp::LinkwitzRileyFilter<float> LRF_LP0;
    juce::dsp::LinkwitzRileyFilter<float> LRF_HP0;
    juce::dsp::LinkwitzRileyFilter<float> LRF_LP1;
    juce::dsp::LinkwitzRileyFilter<float> LRF_HP1;
    juce::dsp::LinkwitzRileyFilter<float> LRF_LP2;
    juce::dsp::LinkwitzRileyFilter<float> LRF_HP2;
    
    
    float maxDel = 1500;
    std::atomic<float>* band1Time = nullptr;
    std::atomic<float>* band2Time = nullptr;
    std::atomic<float>* band3Time = nullptr;
    std::atomic<float>* band4Time = nullptr;
    
    std::atomic<float>* fdbk_amt1 = nullptr;
    std::atomic<float>* fdbk_amt2 = nullptr;
    std::atomic<float>* fdbk_amt3 = nullptr;
    std::atomic<float>* fdbk_amt4 = nullptr;
    float feedback[2][4];
    
    std::atomic<float>* cutoff1 = nullptr;
    std::atomic<float>* cutoff2 = nullptr;
    std::atomic<float>* cutoff3 = nullptr;
    
    float smoothingFac = 0.99f;
    float lastDelay1 = 0.f;
    float lastDelay2 = 0.f;
    float lastDelay3 = 0.f;
    float lastDelay4 = 0.f;
};
