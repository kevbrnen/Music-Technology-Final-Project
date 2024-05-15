/*
  ==============================================================================

    ConvolutionReverbEffectAudioProcessor.h
    Created: 15 Apr 2024 10:32:15am
    Author:  Kevin Brennan

    Class for processing audio for the Convolution Reverb Effect
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ProcessorBase.h"
#include "CircularBuffer.h"
#include "BinaryData.h"


class ConvolutionReverbEffectAudioProcessor : public ProcessorBase
{
public:
    ConvolutionReverbEffectAudioProcessor(juce::AudioProcessorValueTreeState& vts): ConvolutionReverb_Parameters(vts)
    {
        //Get Convolution variables from APVTS for Conv Parameters
        Conv_on = ConvolutionReverb_Parameters.getRawParameterValue("convolution_toggle");
        Conv_WD = ConvolutionReverb_Parameters.getRawParameterValue("convolution_wetdry");
        Conv_Gain = ConvolutionReverb_Parameters.getRawParameterValue("conv_gain");
        Delay_Time = ConvolutionReverb_Parameters.getRawParameterValue("pre_delay_time");
        Delay_FDBK = ConvolutionReverb_Parameters.getRawParameterValue("pre_delay_fdbk");
    };
    
    ~ConvolutionReverbEffectAudioProcessor(){};
    
    const juce::String getName() const override{return "Convolution Reverb";};
    
    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        pluginSpec.sampleRate = sampleRate;
        pluginSpec.maximumBlockSize = samplesPerBlock;
        pluginSpec.numChannels = 2;
        
        ConvEngine.prepare(pluginSpec);
        
        //initialise pre delay circular buffer
        preDelay.initBuffer(2, (int)((sampleRate/1000.0f)*250.0f), sampleRate);
        //Used for smoothing changes in delay time
        lastDelay = Delay_Time->load();
        
        updateFile(0);
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        auto effectOn = Conv_on->load();
        
        if(effectOn != 0.0f)
        {
            
            //Check if impulse response has been updated
            auto IR_index = ConvolutionReverb_Parameters.getRawParameterValue("selector");
            if((int)*IR_index != lastIR)
            {
                lastIR = *IR_index;
                updateFile(lastIR);
            }
            
//Pre Gain
            //Copy dry buffer so it can be mixed with the wet signal after
            juce::AudioBuffer<float> dry(buffer.getNumChannels(), buffer.getNumSamples());
            dry.copyFrom(0, 0, buffer, 0, 0, buffer.getNumSamples());
            dry.copyFrom(1, 0, buffer, 1, 0, buffer.getNumSamples());
            
            //Apply pre-gain to the buffer, before the convolution processing
            auto preGain = juce::Decibels::decibelsToGain(*ConvolutionReverb_Parameters.getRawParameterValue("conv_pre_gain") + 0.0);
            buffer.applyGain(preGain);

            
//Pre Delay
            auto C = 0.6f;
            for(int i = 0; i < buffer.getNumSamples(); ++i)
            {
                auto newDelayTime = Delay_Time->load();
                auto Fdbk_amt = Delay_FDBK->load();
                
                //Smoothing delay time changes
                float ht_constant = .3f;
                smoothingFac = std::pow((0.5), (1.f/(pluginSpec.sampleRate * ht_constant)));
                float delayTime = smoothingFac*(lastDelay) + (1-smoothingFac)*newDelayTime;
                
                //loop through channels and process
                for(int channel = 0; channel < 2 ; ++channel)
                {
                    auto* inData = buffer.getReadPointer(channel);
                    auto* outData = buffer.getWritePointer(channel);
                    
                    //Push sample to buffer with feedback, determined by feedback amount
                    preDelay.pushSampleToBuffer(channel, (inData[i] + (feedback[channel] * Fdbk_amt)));
                        
                    
                    //Get the delayed sample at the last delay time and the delayed sample at the new delay time
                    auto delayedSample_oldVal = preDelay.getDelayedSample(channel, lastDelay);
                    auto delayedSample_newVal = preDelay.getDelayedSample(channel, delayTime);
                    
                    //Interpolate between old and new delayed samples, compute wet/dry combination and add to output
                    outData[i] = (((*delayedSample_newVal * (1-C)) + (*delayedSample_oldVal * C)));
                        
                    //Feedback for each channel
                    feedback[channel] = outData[i];
                }
                lastDelay = delayTime;
            }
            
            
//Convolution
            //Process the block through the convolution engine
            juce::dsp::AudioBlock<float> block (buffer);
            juce::dsp::ProcessContextReplacing<float> context (block);
            ConvEngine.process(context);
            
            
//Wet Dry
            auto wetDry = Conv_WD->load();
            
            buffer.applyGain(wetDry);
            dry.applyGain(1-wetDry);
            
            // Combine wet and dry signals
            for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
            {
                auto* channelData = buffer.getWritePointer(channel);
                auto* dryData = dry.getReadPointer(channel);
                
                for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
                {
                    // Mix wet and dry signals
                    channelData[sample] += dryData[sample];
                }
            }
            
            
//Effect Gain
            const auto NewGain = juce::Decibels::decibelsToGain(*ConvolutionReverb_Parameters.getRawParameterValue("conv_gain") + 0.0);
            
            if(NewGain != lastGain)
            {
                //Smooth gain to remove artefacts
                buffer.applyGainRamp(0, buffer.getNumSamples(), lastGain, NewGain);
                lastGain = NewGain;
            }
        }
    };
    
    //Change selected impulse response in the convolution engine
    void updateFile(int IR)
    {
            switch(IR)
            {
                case 0:
                    ConvEngine.loadImpulseResponse(BinaryData::Ballinafad_Castle48k_wav, BinaryData::Ballinafad_Castle48k_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
                                                   juce::dsp::Convolution::Normalise::yes);
                    break;
                case 1:
                    ConvEngine.loadImpulseResponse(BinaryData::Ballymote_Castle48k_wav, BinaryData::Ballymote_Castle48k_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
                                                   juce::dsp::Convolution::Normalise::yes);
                    break;
                case 2:
                    ConvEngine.loadImpulseResponse(BinaryData::Bedroom48k_wav, BinaryData::Bedroom48k_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
                                                   juce::dsp::Convolution::Normalise::yes);
                    break;
                case 3:
                    ConvEngine.loadImpulseResponse(BinaryData::ChurchIR1_wav, BinaryData::ChurchIR1_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
                                                   juce::dsp::Convolution::Normalise::yes);
                    break;
                case 4:
                    ConvEngine.loadImpulseResponse(BinaryData::church_248k_wav, BinaryData::church_248k_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
                                                   juce::dsp::Convolution::Normalise::yes);
                    break;
                case 5:
                    ConvEngine.loadImpulseResponse(BinaryData::DenContainer48k_wav, BinaryData::DenContainer48k_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
                                                   juce::dsp::Convolution::Normalise::yes);
                    break;
                case 6:
                    ConvEngine.loadImpulseResponse(BinaryData::DenHall48k_wav, BinaryData::DenHall48k_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
                                                   juce::dsp::Convolution::Normalise::yes);
                    break;
                case 7:
                    ConvEngine.loadImpulseResponse(BinaryData::Farmhouse48k_wav, BinaryData::Farmhouse48k_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
                                                   juce::dsp::Convolution::Normalise::yes);
                    break;
                case 8:
                    ConvEngine.loadImpulseResponse(BinaryData::Forest48k_wav, BinaryData::Forest48k_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
                                                   juce::dsp::Convolution::Normalise::yes);
                    break;
                case 9:
                    ConvEngine.loadImpulseResponse(BinaryData::Friary48k_wav, BinaryData::Friary48k_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
                                                   juce::dsp::Convolution::Normalise::yes);
                    break;
                case 10:
                    ConvEngine.loadImpulseResponse(BinaryData::Tent48k_wav, BinaryData::Tent48k_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
                                                   juce::dsp::Convolution::Normalise::yes);
                    break;
                case 11:
                    ConvEngine.loadImpulseResponse(BinaryData::Tunnel48k_wav, BinaryData::Tunnel48k_wavSize, juce::dsp::Convolution::Stereo::no, juce::dsp::Convolution::Trim::yes, 0,
                                                   juce::dsp::Convolution::Normalise::yes);
                    break;
                default:
                    break;
            }
    }
    
    juce::AudioProcessorValueTreeState& ConvolutionReverb_Parameters;
    
private:
    juce::dsp::ProcessSpec pluginSpec;
    juce::AudioFormatManager formatManager;
    
    CircularBuffer preDelay;
    
    juce::dsp::Convolution ConvEngine;
    std::atomic<float>* Conv_on = nullptr;
    std::atomic<float>* Conv_WD = nullptr;
    
    std::atomic<float>* Conv_Gain = nullptr;
    float lastGain;
    float smoothingFac = 0.99f;

    float lastIR = 0.f;
    
    std::atomic<float>* Delay_Time = nullptr;
    float lastDelay;
    std::atomic<float>* Delay_FDBK = nullptr;
    float feedback[2];
    
    const char* IRs[4] = {BinaryData::ChurchIR1_wav, BinaryData::DenContainer48k_wav, BinaryData::DenHall48k_wav, BinaryData::Tent48k_wav};
    int Size[4] = {BinaryData::ChurchIR1_wavSize, BinaryData::DenContainer48k_wavSize, BinaryData::DenHall48k_wavSize, BinaryData::Tent48k_wavSize};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConvolutionReverbEffectAudioProcessor);
};
