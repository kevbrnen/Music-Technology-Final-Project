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
Processing_Chain(*this, nullptr, juce::Identifier("Processing_Chain"), createProcessingParameterLayout()),
Global_Parameters(*this, nullptr,juce::Identifier("Global_Parameters"), createGlobalParameterLayout()),
Filter_Parameters(*this, nullptr, juce::Identifier("Filter_Parameters"), createFilterParameterLayout()),
Delay_Parameters(*this, nullptr, juce::Identifier("Delay_Parameters"), createDelayParameterLayout()),
mainProcessor(new juce::AudioProcessorGraph()),
filterAudioProcessor(Filter_Parameters), delayAudioProcessor(Delay_Parameters)
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
//Processing Chain
juce::AudioProcessorValueTreeState::ParameterLayout NewProjectAudioProcessor::createProcessingParameterLayout()
{
    //Container for all parameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    auto slot1 = std::make_unique<juce::AudioParameterChoice>("slot1", "Slot1", processorChoices, 0);
    auto slot2 = std::make_unique<juce::AudioParameterChoice>("slot2", "Slot2", processorChoices, 1);
    
    //Efficiently add parameter to list
    params.push_back(std::move(slot1));
    params.push_back(std::move(slot2));
    
    return {params.begin(), params.end()};
    
}

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
    
    auto filterGainParameter = std::make_unique<juce::AudioParameterFloat>("filter_gain", "Filter_Gain", juce::NormalisableRange<float>(-48.0f, 0.0f), -6.0f, juce::String(),
                                                                           juce::AudioProcessorParameter::genericParameter, [](float value, int){return juce::String(value, 2);});
    
    //Efficiently add parameter to list
    paramsFilt.push_back(std::move(cutoffFrequencyParameter));
    paramsFilt.push_back(std::move(filterToggleParameter));
    paramsFilt.push_back(std::move(filterGainParameter));
    
    return {paramsFilt.begin(), paramsFilt.end()}; //Returning vector

}

//Delay Parameters
juce::AudioProcessorValueTreeState::ParameterLayout NewProjectAudioProcessor::createDelayParameterLayout()
{
    //Container for all parameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> paramsDelay;
    
    auto delayToggleParameter = std::make_unique<juce::AudioParameterBool>("delay_toggle", "Delay_Toggle", false);
    
    
    return {paramsDelay.begin(), paramsDelay.end()}; //Returning vector
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
    //Audio Proessor Graph Setup
    mainProcessor->setPlayConfigDetails(getMainBusNumInputChannels(),
                                        getMainBusNumOutputChannels(),
                                        sampleRate, samplesPerBlock);
    mainProcessor->prepareToPlay(sampleRate, samplesPerBlock);
    initialiseGraph();
    
    
    //Create Spec to set up effects
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;
    
    //Global Effects
    lastGain = juce::Decibels::decibelsToGain(*Global_Parameters.getRawParameterValue("global_gain") + 0.0);
    
    delayAudioProcessor.prepareToPlay(sampleRate, samplesPerBlock);
    
}

void NewProjectAudioProcessor::releaseResources()
{
    mainProcessor->releaseResources();
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
    
    
//Audio processor graph setup and processing
    updateGraph();
    mainProcessor->processBlock(buffer, midiMessages);

//Delay Effect **Test**
    delayAudioProcessor.processBlock(buffer, midiMessages);

   
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
    return new NewProjectAudioProcessorEditor(*this, Global_Parameters, Filter_Parameters, Processing_Chain);
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
    
    
    //Chain Parameters
    juce::ValueTree chainChild("Processing_Chain");
    chainChild.copyPropertiesAndChildrenFrom(Processing_Chain.state, nullptr);
    combinedValueTree.addChild(chainChild, -1, nullptr);
    
    
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
        
        juce::ValueTree chainValueTree = combinedValueTree.getChildWithName("Processing_Chain");
        
        juce::ValueTree filterValueTree = combinedValueTree.getChildWithName("Filter_Parameters");

        // Set the state of Global_Parameters and Filter_Parameters using their respective value trees
        Global_Parameters.replaceState(globalValueTree);
        Processing_Chain.replaceState(chainValueTree);
        Filter_Parameters.replaceState(filterValueTree);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}

//================================ Audio Processor Graph Functions ==============================================

void NewProjectAudioProcessor::initialiseGraph()
{
    mainProcessor->clear();
    
    audioInputNode  = mainProcessor->addNode (std::make_unique<AudioGraphIOProcessor> (AudioGraphIOProcessor::audioInputNode));
    audioOutputNode = mainProcessor->addNode (std::make_unique<AudioGraphIOProcessor> (AudioGraphIOProcessor::audioOutputNode));
    midiInputNode   = mainProcessor->addNode (std::make_unique<AudioGraphIOProcessor> (AudioGraphIOProcessor::midiInputNode));
    midiOutputNode  = mainProcessor->addNode (std::make_unique<AudioGraphIOProcessor> (AudioGraphIOProcessor::midiOutputNode));
    
    connectAudioNodes();
    connectMidiNodes();
}

void NewProjectAudioProcessor::connectAudioNodes()
{
    for(int channel = 0; channel < 2; ++channel)
    {
        mainProcessor->addConnection({{audioInputNode->nodeID, channel},
                                        {audioOutputNode->nodeID, channel}});
    }
}

void NewProjectAudioProcessor::connectMidiNodes()
{
    mainProcessor->addConnection({{midiInputNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex},
                                    {midiOutputNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex}});
}

void NewProjectAudioProcessor::updateGraph()
{
    bool hasChanged = false;
    
    int NumOptions = processorChoices.size();
    
    
    juce::Array<juce::AudioParameterChoice*> choices{dynamic_cast<juce::AudioParameterChoice*>(Processing_Chain.getParameter("slot1")),
                                                        dynamic_cast<juce::AudioParameterChoice*>(Processing_Chain.getParameter("slot2"))};
    
    
    juce::ReferenceCountedArray<Node> slots;
    slots.add(slot1Node);
    slots.add(slot2Node);
    
    
    for(int i = 0; i<NumOptions; i++)
    {
        auto& choice = choices.getReference(i);
        auto slot = slots.getUnchecked(i);
        
        if(choice->getIndex() == 0) //Empty
        {
            if(slot != nullptr)
            {
                mainProcessor->removeNode(slot.get());
                slots.set(i, nullptr);
                hasChanged = true;
            }
        }
        else if(choice->getIndex() == 1) //Filter
        {
            if(slot != nullptr)
            {
                if(slot->getProcessor()->getName() == "Filter")
                    continue;
                
                mainProcessor->removeNode(slot.get());
            }
            
            slots.set(i, mainProcessor->addNode(std::make_unique<FilterEffectAudioProcessor>(Filter_Parameters)));
            hasChanged = true;
        }
    }
    
    
    if(hasChanged)
    {
        for(auto connection : mainProcessor->getConnections())
        {
            mainProcessor->removeConnection(connection);
        }
        
        juce::ReferenceCountedArray<Node> activeSlots;
        
        for(auto slot : slots)
        {
            if(slot != nullptr)
            {
                activeSlots.add(slot);
                
                slot->getProcessor()->setPlayConfigDetails(getMainBusNumInputChannels(),
                                                           getMainBusNumOutputChannels(),
                                                           getSampleRate(), getBlockSize());
            }
        }
        
        if(activeSlots.isEmpty())
        {
            connectAudioNodes();
        }
        else
        {
            if(activeSlots.size() == 1)
            {
                for(int channel = 0; channel < 2; ++channel)
                {
                    mainProcessor->addConnection({{audioInputNode->nodeID, channel},
                                                    {activeSlots.getFirst()->nodeID, channel}});
                    
                    mainProcessor->addConnection({{activeSlots.getFirst()->nodeID, channel},
                                                    {audioOutputNode->nodeID, channel}});
                }
            }
            else
            {
                for(int i = 0; i < activeSlots.size()-1; ++i)
                {
                    for(int channel = 0; channel < 2; ++channel)
                    {
                        mainProcessor->addConnection({{activeSlots.getUnchecked(i)->nodeID, channel},
                                                        {activeSlots.getUnchecked(i + 1)->nodeID, channel}});
                    }
                    
                    for(int channel = 0; channel < 2; ++channel)
                    {
                        mainProcessor->addConnection({{audioInputNode->nodeID, channel},
                                                        {activeSlots.getFirst()->nodeID, channel}});
                        
                        mainProcessor->addConnection({{activeSlots.getLast()->nodeID, channel},
                                                        {audioOutputNode->nodeID, channel}});
                    }
                }
            }
    
            connectMidiNodes();
            
            for(auto node : mainProcessor->getNodes())
                node->getProcessor()->enableAllBuses();
        }
    }

    
    slot1Node = slots.getUnchecked(0);
    slot2Node = slots.getUnchecked(1);
}

