/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FilterEffectAudioProcessor.h"
#include "DelayEffectAudioProcessor.h"


//==============================================================================
/**
*/
class NewProjectAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    //Audio Processor Graph Public
    using AudioGraphIOProcessor = juce::AudioProcessorGraph::AudioGraphIOProcessor;
    using Node = juce::AudioProcessorGraph::Node;
    juce::StringArray processorChoices{"Empty", "Filter"};

    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createProcessingParameterLayout();
    juce::AudioProcessorValueTreeState::ParameterLayout createGlobalParameterLayout();
    juce::AudioProcessorValueTreeState::ParameterLayout createFilterParameterLayout();
    juce::AudioProcessorValueTreeState::ParameterLayout createDelayParameterLayout();
    
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
    
    juce::AudioProcessorValueTreeState Processing_Chain;
    juce::AudioProcessorValueTreeState Global_Parameters;
    juce::AudioProcessorValueTreeState Filter_Parameters;
    juce::AudioProcessorValueTreeState Delay_Parameters;
private:
    //For AudioProcessorGraph
    void initialiseGraph();
    void connectAudioNodes();
    void connectMidiNodes();
    void updateGraph();
    
    std::unique_ptr<juce::AudioProcessorGraph> mainProcessor;
    
    Node::Ptr audioInputNode;
    Node::Ptr audioOutputNode;
    Node::Ptr midiInputNode;
    Node::Ptr midiOutputNode;
    
    Node::Ptr slot1Node;
    Node::Ptr slot2Node;

    
    //Global Parameters
    std::atomic<float>* globalGain = nullptr;
    float lastGain;
    
    //Filter
    FilterEffectAudioProcessor filterAudioProcessor;
    
    //Delay
    DelayEffectAudioProcessor delayAudioProcessor;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};
