/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FilterEffectAudioProcessor.h"
#include "DelayEffectAudioProcessor.h"
#include "ConvolutionReverbEffectAudioProcessor.h"
#include "DelayXpanseEffectAudioProcessor.h"
#include "DegradeEffectAudioProcessor.h"


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
    
    juce::StringArray processorChoices{"Empty", "Filter", "Delay", "Convolution", "Delay-Xpanse", "Degrade"};  //Effects that can be added to the chain
    
    juce::StringArray IR_Choices{"Church-1", "Shipping Container", "Hall-1", "Tent"};

    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    //==============================================================================
    //Parameter layouts for different tree types
    juce::AudioProcessorValueTreeState::ParameterLayout createProcessingParameterLayout();
    juce::AudioProcessorValueTreeState::ParameterLayout createGlobalParameterLayout();
    juce::AudioProcessorValueTreeState::ParameterLayout createFilterParameterLayout();
    juce::AudioProcessorValueTreeState::ParameterLayout createDelayParameterLayout();
    juce::AudioProcessorValueTreeState::ParameterLayout createConvolutionParameterLayout();
    juce::AudioProcessorValueTreeState::ParameterLayout createXpanseParameterLayout();
    juce::AudioProcessorValueTreeState::ParameterLayout createDegradeParameterLayout();
    
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
    juce::AudioProcessorValueTreeState Convolution_Parameters;
    juce::AudioProcessorValueTreeState DelayXpanse_Parameters;
    juce::AudioProcessorValueTreeState Degrade_Parameters;
private:
    FilterEffectAudioProcessor filterEffect;
    DelayEffectAudioProcessor delayEffect;
    ConvolutionReverbEffectAudioProcessor convolutionEffect;
    DelayXpanseEffectAudioProcessor xpanseEffect;
    DegradeEffectAudioProcessor degradeEffect;
    
    //For AudioProcessorGraph (Chain of effects)
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
    Node::Ptr slot3Node;
    //Node::Ptr slot4Node;
    
    //Global Parameters
    std::atomic<float>* globalGain = nullptr;
    float lastGain;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};
