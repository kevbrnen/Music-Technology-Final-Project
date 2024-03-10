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
Global_Parameters(*this, nullptr, juce::Identifier("Global_Params"), {std::make_unique<juce::AudioParameterFloat>("global_gain", "Global_Gain", juce::NormalisableRange{0.0f, 1.0f, 0.001f, 0.2f, false}, 0.5)}),
    Filter_Parameters(*this,nullptr, juce::Identifier("Filter_Params"),
                {std::make_unique<juce::AudioParameterFloat>("lowpass_cutoff_frequency","Lowpass_Cutoff_Frequency",
                    juce::NormalisableRange{20.f, 20000.f, 0.1f, 0.2f, false}, 500.f)})
{
    //Setting up audio processor value tree state objects and parameters above
    
    //Retrieving Parameter values
  cutoffFrequency =
    Filter_Parameters.getRawParameterValue("lowpass_cutoff_frequency");
    
    globalGain = Global_Parameters.getRawParameterValue("global_gain");
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
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //Create Spec to set up effects
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;
    
    //Set up LowPass filter object
    LPF_Test.setSpec(spec);
    LPF_Test.setCutoff(750);
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

    
    //Get and set new cutoff frequency
    const auto NewcutoffFrequency = cutoffFrequency->load();
    LPF_Test.setCutoff(NewcutoffFrequency);
    LPF_Test.process(buffer);
    
    //get and set new gain and update previous(for ramp)
    const auto NewGain = globalGain->load();
    buffer.applyGainRamp(0, buffer.getNumSamples(), lastGain, NewGain);
    lastGain = NewGain;
    
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
    /*
    // Create a parent XML element to hold both trees
    juce::XmlElement parentXml("PluginState");
    
    //Create state variables for each tree
    auto GlobalState = Global_Parameters.copyState();
    auto FilterState = Filter_Parameters.copyState();
    
    //Create XML from  state
    std::unique_ptr<juce::XmlElement> globalXml (GlobalState.createXml());
    std::unique_ptr<juce::XmlElement> filterXml (FilterState.createXml());
    
    //Add XML as children of parent XML
    parentXml.addChildElement(globalXml.get());
    parentXml.addChildElement(filterXml.get());
    
    //Save XML
    copyXmlToBinary(parentXml, destData);
     */
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    /*
    //Get parent XML for whole plugin
    std::unique_ptr<juce::XmlElement> parentXml (getXmlFromBinary(data, sizeInBytes));
    
    
    if(parentXml != nullptr && parentXml->hasTagName("PluginState"))
    {
        //Get child XML's from parent, one for each tree
        juce::XmlElement* globalXml = parentXml->getFirstChildElement();
        juce::XmlElement* filterXml = parentXml->getNextElement();
        
        //Set global parameter state from global XML
        if(globalXml != nullptr && globalXml->hasTagName(Filter_Parameters.state.getType()))
        {
            Global_Parameters.state = juce::ValueTree::fromXml(*globalXml);
        }
        
        //Set filter parameter state from filter XML
        if(filterXml != nullptr && filterXml->hasTagName(Filter_Parameters.state.getType()))
        {
            Filter_Parameters.state = juce::ValueTree::fromXml(*filterXml);
        }
    }
     */
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
