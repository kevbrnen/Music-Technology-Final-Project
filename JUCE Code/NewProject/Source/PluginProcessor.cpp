/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PluginParameterLayout.h"


//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
              ),
#else
    :
#endif
Processing_Chain(*this, nullptr, juce::Identifier("Processing_Chain"), createProcessingParameterLayout()),
Global_Parameters(*this, nullptr,juce::Identifier("Global_Parameters"), createGlobalParameterLayout()),
Filter_Parameters(*this, nullptr, juce::Identifier("Filter_Parameters"), createFilterParameterLayout()),
Delay_Parameters(*this, nullptr, juce::Identifier("Delay_Parameters"), createDelayParameterLayout()),
Convolution_Parameters(*this, nullptr, juce::Identifier("Convolution_Parameters"), createConvolutionParameterLayout()),
DelayXpanse_Parameters(*this, nullptr, juce::Identifier("DelayXpanse_Parameters"), createXpanseParameterLayout()),
Degrade_Parameters(*this, nullptr, juce::Identifier("Degrade_Parameters"), createDegradeParameterLayout()),
Phaser_Parameters(*this, nullptr, juce::Identifier("Phaser_Parameters"), createPhaserParameterLayout()),
Reverb_Parameters(*this, nullptr, juce::Identifier("Reverb_Parameters"), createReverbParameterLayout()),
Distortion_Parameters(*this, nullptr, juce::Identifier("Distortion_Parameters"), createDistortionParameterLayout()),
filterEffect(Filter_Parameters), delayEffect(Delay_Parameters), convolutionEffect(Convolution_Parameters), xpanseEffect(DelayXpanse_Parameters), degradeEffect(Degrade_Parameters), phaserEffect(Phaser_Parameters), reverbEffect(Reverb_Parameters), distortionEffect(Distortion_Parameters)
{
    //Setting up audio processor value tree state objects and parameters above
    
    //Retrieving Parameter values
    //Global Parameters
    globalGain = Global_Parameters.getRawParameterValue("global_gain");
    
    //How many slots are in the processing chain (Fixed value)
    numSlots = sizeof(effects) / sizeof(effects[0]);
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

//==============================================================================

const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
//The prepare to play function below is used to set up any pre-playback variables etc
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //Calling prepare to play for each effect
    filterEffect.prepareToPlay(sampleRate, samplesPerBlock);
    delayEffect.prepareToPlay(sampleRate, samplesPerBlock);
    convolutionEffect.prepareToPlay(sampleRate, samplesPerBlock);
    xpanseEffect.prepareToPlay(sampleRate, samplesPerBlock);
    degradeEffect.prepareToPlay(sampleRate, samplesPerBlock);
    phaserEffect.prepareToPlay(sampleRate, samplesPerBlock);
    reverbEffect.prepareToPlay(sampleRate, samplesPerBlock);
    distortionEffect.prepareToPlay(sampleRate, samplesPerBlock);
    
    
    //Global Gain
    lastGain = juce::Decibels::decibelsToGain(*Global_Parameters.getRawParameterValue("global_gain") + 0.0);
    
}

void NewProjectAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NewProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //Block Processing Setup
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
//Audio processor chain setup and processing
    updateChain(); //Check for any changes to the chain
    //Filter = 1
    //Delay = 2
    //Convolution Reverb = 3
    //Delay-Xpanse = 4
    //Degrade = 5
    //Phaser = 6
    //Reverb = 7
    //Distortion = 8
    
    //Loops through the 'effects' array, whose values are determined by each slot in the processing chain
    //For each effect it calls the processBlock function in the effect, which adds the effect to the current block
    for(int i = 0; i<numSlots; ++i)
    {
        switch(effects[i])
        {
            case 1:
                filterEffect.processBlock(buffer, midiMessages);
                break;
            case 2:
                delayEffect.processBlock(buffer, midiMessages);
                break;
            case 3:
                convolutionEffect.processBlock(buffer, midiMessages);
                break;
            case 4:
                xpanseEffect.processBlock(buffer, midiMessages);
                break;
            case 5:
                degradeEffect.processBlock(buffer, midiMessages);
                break;
            case 6:
                phaserEffect.processBlock(buffer, midiMessages);
                break;
            case 7:
                reverbEffect.processBlock(buffer, midiMessages);
                break;
            case 8:
                distortionEffect.processBlock(buffer, midiMessages);
                break;
            default:
                break;
        }
    }
    
//Global parameters
    //get and set new gain and update previous(for ramp)
    const auto NewGain = juce::Decibels::decibelsToGain(*Global_Parameters.getRawParameterValue("global_gain") + 0.0);
    if(NewGain != lastGain)
    {
        //For Smoothing between gain values
        buffer.applyGainRamp(0, buffer.getNumSamples(), lastGain, NewGain);
        lastGain = NewGain;
    }
    
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor(*this, Global_Parameters, Filter_Parameters, Processing_Chain, Delay_Parameters, Convolution_Parameters, DelayXpanse_Parameters, Degrade_Parameters, Phaser_Parameters, Reverb_Parameters, Distortion_Parameters);
}

//==============================================================================
//Copies all parameters and their values into an XML file to save
void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    //Tree to hold all "child" trees (any trees from any other audio processors/effects)
    juce::ValueTree combinedValueTree(juce::Identifier("All_Params"));

    //Global Parameters
    juce::ValueTree globalChild("Global_Parameters");//Temporary **NAMED** tree (Must be named so it can be loaded in setStateInformation)
    globalChild.copyPropertiesAndChildrenFrom(Global_Parameters.state, nullptr);//Copy APVTS state to temp tree
    combinedValueTree.addChild(globalChild, -1, nullptr);//Add temp tree as child to combined tree
    
    
    //Chain Parameters
    juce::ValueTree chainChild("Processing_Chain");
    chainChild.copyPropertiesAndChildrenFrom(Processing_Chain.state, nullptr);
    combinedValueTree.addChild(chainChild, -1, nullptr);
    
    
    //Filter Effect Parameters
    juce::ValueTree filterChild("Filter_Parameters");
    filterChild.copyPropertiesAndChildrenFrom(Filter_Parameters.state, nullptr);
    combinedValueTree.addChild(filterChild, -1, nullptr);
    
    //Delay Effect Parameters
    juce::ValueTree delayChild("Delay_Parameters");
    delayChild.copyPropertiesAndChildrenFrom(Delay_Parameters.state, nullptr);
    combinedValueTree.addChild(delayChild, -1, nullptr);
    
    //Convolution Effect Parameters
    juce::ValueTree convChild("Convolution_Parameters");
    convChild.copyPropertiesAndChildrenFrom(Convolution_Parameters.state, nullptr);
    combinedValueTree.addChild(convChild, -1, nullptr);

    //Delay-Xpanse Effect Parameters
    juce::ValueTree xpanseChild("Xpanse_Parameters");
    xpanseChild.copyPropertiesAndChildrenFrom(DelayXpanse_Parameters.state, nullptr);
    combinedValueTree.addChild(xpanseChild, -1, nullptr);
    
    //Degrade Effect Parameters
    juce::ValueTree degradeChild("Degrade_Parameters");
    degradeChild.copyPropertiesAndChildrenFrom(Degrade_Parameters.state, nullptr);
    combinedValueTree.addChild(degradeChild, -1, nullptr);
    
    //Phaser Effect Parameters
    juce::ValueTree phaserChild("Phaser_Parameters");
    phaserChild.copyPropertiesAndChildrenFrom(Phaser_Parameters.state, nullptr);
    combinedValueTree.addChild(phaserChild, -1, nullptr);
    
    //Reverb Effect Parameters
    juce::ValueTree reverbChild("Reverb_Parameters");
    reverbChild.copyPropertiesAndChildrenFrom(Reverb_Parameters.state, nullptr);
    combinedValueTree.addChild(reverbChild, -1, nullptr);
    
    //Distortion Effect Parameters
    juce::ValueTree distortionChild("Distortion_Parameters");
    distortionChild.copyPropertiesAndChildrenFrom(Distortion_Parameters.state, nullptr);
    combinedValueTree.addChild(distortionChild, -1, nullptr);
    
    //Create XML file from combined tree
    std::unique_ptr<juce::XmlElement> CombinedXml (combinedValueTree.createXml());
    copyXmlToBinary(*CombinedXml, destData);
}

//Opens the XML file created above and resets all parameters to their saved values
void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Load XML element
    std::unique_ptr<juce::XmlElement> combinedXml(getXmlFromBinary(data, sizeInBytes));

    if (combinedXml != nullptr)
    {
        // Get the combined ValueTree from the XML element
        juce::ValueTree combinedValueTree = juce::ValueTree::fromXml(*combinedXml);

        // Get individual value trees
        juce::ValueTree globalValueTree = combinedValueTree.getChildWithName("Global_Parameters");
        
        juce::ValueTree chainValueTree = combinedValueTree.getChildWithName("Processing_Chain");
        
        juce::ValueTree filterValueTree = combinedValueTree.getChildWithName("Filter_Parameters");
        
        juce::ValueTree delayValueTree = combinedValueTree.getChildWithName("Delay_Parameters");
        
        juce::ValueTree convValueTree = combinedValueTree.getChildWithName("Convolution_Parameters");
        
        juce::ValueTree xpanseValueTree = combinedValueTree.getChildWithName("Xpanse_Parameters");
        
        juce::ValueTree degradeValueTree = combinedValueTree.getChildWithName("Degrade_Parameters");
        
        juce::ValueTree phaserValueTree = combinedValueTree.getChildWithName("Phaser_Parameters");
        
        juce::ValueTree reverbValueTree = combinedValueTree.getChildWithName("Reverb_Parameters");
        
        juce::ValueTree distortionValueTree = combinedValueTree.getChildWithName("Distortion_Parameters");

        // Set the state of Global_Parameters and Filter_Parameters using their respective value trees
        Global_Parameters.replaceState(globalValueTree);
        Processing_Chain.replaceState(chainValueTree);
        Filter_Parameters.replaceState(filterValueTree);
        Delay_Parameters.replaceState(delayValueTree);
        Convolution_Parameters.replaceState(convValueTree);
        DelayXpanse_Parameters.replaceState(xpanseValueTree);
        Degrade_Parameters.replaceState(degradeValueTree);
        Phaser_Parameters.replaceState(phaserValueTree);
        Reverb_Parameters.replaceState(reverbValueTree);
        Distortion_Parameters.replaceState(distortionValueTree);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}


//================================ Audio Processing Chain Functions ==============================================

//Update the graph
void NewProjectAudioProcessor::updateChain()
{
    
    juce::Array<juce::AudioParameterChoice*> choices{dynamic_cast<juce::AudioParameterChoice*>(Processing_Chain.getParameter("slot1")),
                                                     dynamic_cast<juce::AudioParameterChoice*>(Processing_Chain.getParameter("slot2")),
                                                     dynamic_cast<juce::AudioParameterChoice*>(Processing_Chain.getParameter("slot3")),
                                                     dynamic_cast<juce::AudioParameterChoice*>(Processing_Chain.getParameter("slot4")),
                                                     dynamic_cast<juce::AudioParameterChoice*>(Processing_Chain.getParameter("slot5")),
                                                     dynamic_cast<juce::AudioParameterChoice*>(Processing_Chain.getParameter("slot6")),
                                                     dynamic_cast<juce::AudioParameterChoice*>(Processing_Chain.getParameter("slot7")),
                                                     dynamic_cast<juce::AudioParameterChoice*>(Processing_Chain.getParameter("slot8"))};
    
    //Loop through all choice parameters (the dropdown boxes in the chain menu) and check if any have changed, and if so what they have changed to
    //Null for empty, Filter for filter etc.
    for(int i = 0; i<choices.size(); i++)
    {
        auto& choice = choices.getReference(i);
        
        switch(choice->getIndex())
        {
            case 0:
                effects[i] = 0;
                break;
            case 1:
                effects[i] = 1;
                break;
            case 2:
                effects[i] = 2;
                break;
            case 3:
                effects[i] = 3;
                break;
            case 4:
                effects[i] = 4;
                break;
            case 5:
                effects[i] = 5;
                break;
            case 6:
                effects[i] = 6;
                break;
            case 7:
                effects[i] = 7;
                break;
            case 8:
                effects[i] = 8;
                break;
            default:
                break;
        }
    }
}

