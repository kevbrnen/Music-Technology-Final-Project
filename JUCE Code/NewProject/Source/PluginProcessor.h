/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FilterEffectAudioProcessor.h"

#include "LowpassFilter.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createGlobalParameterLayout();
    juce::AudioProcessorValueTreeState::ParameterLayout createFilterParameterLayout();
    
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    
    juce::AudioProcessorValueTreeState Global_Parameters;
    juce::AudioProcessorValueTreeState Filter_Parameters;
private:
    
    //Global Parameters
    std::atomic<float>* globalGain = nullptr;
    float lastGain;
    
    //Filter Parameters
    FilterEffectAudioProcessor filterAudioProcessor;
    
    std::atomic<bool>* Filt_OnOff = nullptr;
    std::atomic<float>* FilterCutoffFrequency = nullptr;
    LowpassFilter LPF_Test;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};
