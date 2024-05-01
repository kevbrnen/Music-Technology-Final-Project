/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

    This class controls all audio processing in the plugin
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FilterEffectAudioProcessor.h"
#include "DelayEffectAudioProcessor.h"
#include "ConvolutionReverbEffectAudioProcessor.h"
#include "DelayXpanseEffectAudioProcessor.h"
#include "DegradeEffectAudioProcessor.h"
#include "PhaserEffectAudioProcessor.h"
#include "ReverbEffectAudioProcessor.h"


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
    
    //All Audio processor value tree state objects for the plugin.
    //These are used to control parameters, and act as a link between the UI
    //and the audio processor.
    juce::AudioProcessorValueTreeState Processing_Chain;
    juce::AudioProcessorValueTreeState Global_Parameters;
    juce::AudioProcessorValueTreeState Filter_Parameters;
    juce::AudioProcessorValueTreeState Delay_Parameters;
    juce::AudioProcessorValueTreeState Convolution_Parameters;
    juce::AudioProcessorValueTreeState DelayXpanse_Parameters;
    juce::AudioProcessorValueTreeState Degrade_Parameters;
    juce::AudioProcessorValueTreeState Phaser_Parameters;
    juce::AudioProcessorValueTreeState Reverb_Parameters;
private:
    //Individual effect Audio Processor objects
    FilterEffectAudioProcessor filterEffect;
    DelayEffectAudioProcessor delayEffect;
    ConvolutionReverbEffectAudioProcessor convolutionEffect;
    DelayXpanseEffectAudioProcessor xpanseEffect;
    DegradeEffectAudioProcessor degradeEffect;
    PhaserEffectAudioProcessor phaserEffect;
    ReverbEffectAudioProcessor reverbEffect;
    
    //For Processing chain
    void updateChain();
    int effects[7] = {0, 0, 0, 0, 0, 0, 0};
    int numSlots;
    
    
    //Global Parameters
    std::atomic<float>* globalGain = nullptr;
    float lastGain;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};
