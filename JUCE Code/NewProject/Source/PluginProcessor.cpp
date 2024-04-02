/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


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
Global_Parameters(*this, //Audio Processor to connect to
                  nullptr, //Undo Manager
                  juce::Identifier("Global_Parameters"), createGlobalParameterLayout()),
Filter_Parameters(*this, nullptr, juce::Identifier("Filter_Parameters"), createFilterParameterLayout()), filterAudioProcessor(Filter_Parameters)
{
    
    //Setting up audio processor value tree state objects and parameters above
    
    //Retrieving Parameter values
    //Global Parameters
    globalGain = Global_Parameters.getRawParameterValue("global_gain");
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

//==============================================================================
//Global Parameters
juce::AudioProcessorValueTreeState::ParameterLayout NewProjectAudioProcessor::createGlobalParameterLayout()
{
    //Container for all parameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    //gain parameter itself
    auto gainParameter = std::make_unique<juce::AudioParameterFloat>("global_gain", //Parameter ID
                                                                     "Global_Gain", //Parameter Name
                                                                     juce::NormalisableRange<float>(-48.0f, 0.0f), //Normalisable Range (Range of values for slider)
                                                                     -6.0f, //Default Value
                                                                     juce::String(), //PArameter Label (optional)
                                                                     juce::AudioProcessorParameter::genericParameter, //Parameter Category (optional)
                                                                     [](float value, int){return juce::String(value, 2);});
    
    //Efficiently add parameter to list
    params.push_back(std::move(gainParameter));
    
    return {params.begin(), params.end()}; //Returning vector
}

//Filter Parameters
juce::AudioProcessorValueTreeState::ParameterLayout NewProjectAudioProcessor::createFilterParameterLayout()
{
    //Container for all parameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> paramsFilt;
    
    auto cutoffFrequencyParameter = std::make_unique<juce::AudioParameterFloat>("cutoff_frequency", "Cutoff_Frequency", juce::NormalisableRange{20.f, 20000.f, 0.1f, 0.2f, false}, 500.f);
    
    auto filterToggleParameter = std::make_unique<juce::AudioParameterBool>("filter_toggle", "Filter_Toggle", false);
    
    //Efficiently add parameter to list
    paramsFilt.push_back(std::move(cutoffFrequencyParameter));
    paramsFilt.push_back(std::move(filterToggleParameter));
    
    return {paramsFilt.begin(), paramsFilt.end()}; //Returning vector

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
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //Create Spec to set up effects
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;
    
    
    //Global Effects
    lastGain = juce::Decibels::decibelsToGain(*Global_Parameters.getRawParameterValue("global_gain") + 0.0);
    
    
    filterAudioProcessor.initSpec(spec);
    filterAudioProcessor.prepareToPlay(sampleRate, samplesPerBlock);
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
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
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
//Filter Effect
    filterAudioProcessor.processBlock(buffer, midiMessages);
   
//Global parameters
    //get and set new gain and update previous(for ramp)
    const auto NewGain = juce::Decibels::decibelsToGain(*Global_Parameters.getRawParameterValue("global_gain") + 0.0);
    if(NewGain != lastGain)
    {
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
    return new NewProjectAudioProcessorEditor(*this, Global_Parameters, Filter_Parameters);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    //Tree to hold all "child" trees (any trees from any other audio processors/effects)
    juce::ValueTree combinedValueTree(juce::Identifier("All_Params"));

    //Global Parameters
    juce::ValueTree globalChild("Global_Parameters");//Temporary **NAMED** tree (Must be named so it can be loaded in setStateInformation)
    globalChild.copyPropertiesAndChildrenFrom(Global_Parameters.state, nullptr);//Copy APVTS state to temp tree
    combinedValueTree.addChild(globalChild, -1, nullptr);//Add temp tree as child to combined tree
    
    //Filter Effect Parameters
    juce::ValueTree filterChild("Filter_Parameters");
    filterChild.copyPropertiesAndChildrenFrom(Filter_Parameters.state, nullptr);
    combinedValueTree.addChild(filterChild, -1, nullptr);
    
    //Create XML file from combined tree
    std::unique_ptr<juce::XmlElement> CombinedXml (combinedValueTree.createXml());
    copyXmlToBinary(*CombinedXml, destData);
}

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
        juce::ValueTree filterValueTree = combinedValueTree.getChildWithName("Filter_Parameters");

        // Set the state of Global_Parameters and Filter_Parameters using their respective value trees
        Global_Parameters.replaceState(globalValueTree);
        Filter_Parameters.replaceState(filterValueTree);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
