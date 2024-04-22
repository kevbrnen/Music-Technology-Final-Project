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
Convolution_Parameters(*this, nullptr, juce::Identifier("Convolution_Parameters"), createConvolutionParameterLayout()),
DelayXpanse_Parameters(*this, nullptr, juce::Identifier("DelayXpanse_Parameters"), createXpanseParameterLayout()),
Degrade_Parameters(*this, nullptr, juce::Identifier("Degrade_Parameters"), createDegradeParameterLayout()),
filterEffect(Filter_Parameters), delayEffect(Delay_Parameters), convolutionEffect(Convolution_Parameters), xpanseEffect(DelayXpanse_Parameters), degradeEffect(Degrade_Parameters), mainProcessor(new juce::AudioProcessorGraph())
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
    auto slot2 = std::make_unique<juce::AudioParameterChoice>("slot2", "Slot2", processorChoices, 0);
    auto slot3 = std::make_unique<juce::AudioParameterChoice>("slot3", "Slot3", processorChoices, 0);
    //auto slot4 = std::make_unique<juce::AudioParameterChoice>("slot4", "Slot4", processorChoices, 0);
    
    //Efficiently add parameter to list
    params.push_back(std::move(slot1));
    params.push_back(std::move(slot2));
    params.push_back(std::move(slot3));
    //params.push_back(std::move(slot4));
    
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
                                                                     juce::NormalisableRange<float>(-48.0f, 10.0f), //Normalisable Range (Range of values for slider)
                                                                     0.0f, //Default Value
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
    
    auto filterGainParameter = std::make_unique<juce::AudioParameterFloat>("filter_gain", "Filter_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(),
                                                                           juce::AudioProcessorParameter::genericParameter, [](float value, int){return juce::String(value, 2);});
    auto filterLFOToggleParameter = std::make_unique<juce::AudioParameterBool>("filter_LFO_toggle", "Filter_LFO_Toggle", false);
    
    //Efficiently add parameter to list
    paramsFilt.push_back(std::move(cutoffFrequencyParameter));
    paramsFilt.push_back(std::move(filterToggleParameter));
    paramsFilt.push_back(std::move(filterGainParameter));
    paramsFilt.push_back(std::move(filterLFOToggleParameter));

    
    return {paramsFilt.begin(), paramsFilt.end()}; //Returning vector

}

//Delay Parameters
juce::AudioProcessorValueTreeState::ParameterLayout NewProjectAudioProcessor::createDelayParameterLayout()
{
    //Container for all parameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> paramsDelay;
    
    
    auto delayToggleParameter = std::make_unique<juce::AudioParameterBool>("delay_toggle", "Delay_Toggle", false);
    
    auto delayTimeParameter = std::make_unique<juce::AudioParameterFloat>("delay_time", "Delay_Time", juce::NormalisableRange{0.0f, 2999.f, 1.f, 0.4f, false}, 500.f);
    
    auto delayFeedbackParameter = std::make_unique<juce::AudioParameterFloat>("delay_fdbk", "Delay_FDBK", juce::NormalisableRange{0.0f, 0.999f, 0.001f, 1.f, false}, 0.5f);
    
    auto delayWetDryParameter = std::make_unique<juce::AudioParameterFloat>("delay_wetdry", "Delay_WetDry", juce::NormalisableRange{0.0f, 1.f, 0.001f, 1.f, false}, 0.5f);
    
    auto delayGainParameter = std::make_unique<juce::AudioParameterFloat>("delay_gain", "Delay_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(),
                                                                           juce::AudioProcessorParameter::genericParameter, [](float value, int){return juce::String(value, 2);});
    
    auto delayLFOToggleParameter = std::make_unique<juce::AudioParameterBool>("delay_LFO_toggle", "Delay_LFO_Toggle", false);
    
    
    paramsDelay.push_back(std::move(delayToggleParameter));
    paramsDelay.push_back(std::move(delayTimeParameter));
    paramsDelay.push_back(std::move(delayFeedbackParameter));
    paramsDelay.push_back(std::move(delayWetDryParameter));
    paramsDelay.push_back(std::move(delayGainParameter));
    paramsDelay.push_back(std::move(delayLFOToggleParameter));
    
    return {paramsDelay.begin(), paramsDelay.end()}; //Returning vector
}

//Convolution Reverb Parameters
juce::AudioProcessorValueTreeState::ParameterLayout NewProjectAudioProcessor::createConvolutionParameterLayout()
{
    //Container for all parameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> paramsConv;
    
    auto convToggleParameter = std::make_unique<juce::AudioParameterBool>("convolution_toggle", "Convolution_Toggle", false);
    
    auto convWetDryParameter = std::make_unique<juce::AudioParameterFloat>("convolution_wetdry", "Convolution_WetDry", juce::NormalisableRange{0.0f, 1.f, 0.001f, 1.f, false}, 0.5f);
    
    auto convGainParameter = std::make_unique<juce::AudioParameterFloat>("conv_gain", "Conv_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(),
                                                                           juce::AudioProcessorParameter::genericParameter, [](float value, int){return juce::String(value, 2);});
    
    auto impulseSelector = std::make_unique<juce::AudioParameterChoice>("selector", "Selector", IR_Choices, 0);
    
    paramsConv.push_back(std::move(convToggleParameter));
    paramsConv.push_back(std::move(convWetDryParameter));
    paramsConv.push_back(std::move(convGainParameter));
    paramsConv.push_back(std::move(impulseSelector));
    
    
    return {paramsConv.begin(), paramsConv.end()}; //Returning vector
}

//Delay-Xpanse Parameters
juce::AudioProcessorValueTreeState::ParameterLayout NewProjectAudioProcessor::createXpanseParameterLayout()
{
    //Container for all parameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> paramsXpanse;
    
    auto xpanseToggleParameter = std::make_unique<juce::AudioParameterBool>("xpanse_toggle", "Xpanse_Toggle", false);
    
    auto xpanseWetDryParameter = std::make_unique<juce::AudioParameterFloat>("xpanse_wetdry", "Xpanse_WetDry", juce::NormalisableRange{0.0f, 1.f, 0.001f, 1.f, false}, 0.5f);
    
    auto xpanseGainParameter = std::make_unique<juce::AudioParameterFloat>("xpanse_gain", "Xpanse_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(),
                                                                           juce::AudioProcessorParameter::genericParameter, [](float value, int){return juce::String(value, 2);});
    
    auto pongdelayLParameter = std::make_unique<juce::AudioParameterFloat>("pong_delay_L_time", "Pong_Delay_L_Time", juce::NormalisableRange{0.0f, 1999.f, 1.f, 0.4f, false}, 200.f);
    
    auto pongfeedbackLParameter = std::make_unique<juce::AudioParameterFloat>("pong_delay_L_fdbk", "Pong_Delay_L_FDBK", juce::NormalisableRange{0.0f, 0.999f, 0.001f, 1.f, false}, 0.5f);
    
    auto pongdelayRParameter = std::make_unique<juce::AudioParameterFloat>("pong_delay_R_time", "Pong_Delay_R_Time", juce::NormalisableRange{0.0f, 1999.f, 1.f, 0.4f, false}, 200.f);
    
    auto pongfeedbackRParameter = std::make_unique<juce::AudioParameterFloat>("pong_delay_R_fdbk", "Pong_Delay_R_FDBK", juce::NormalisableRange{0.0f, 0.999f, 0.001f, 1.f, false}, 0.5f);
    
    paramsXpanse.push_back(std::move(xpanseToggleParameter));
    paramsXpanse.push_back(std::move(xpanseWetDryParameter));
    paramsXpanse.push_back(std::move(xpanseGainParameter));
    paramsXpanse.push_back(std::move(pongdelayLParameter));
    paramsXpanse.push_back(std::move(pongfeedbackLParameter));
    paramsXpanse.push_back(std::move(pongdelayRParameter));
    paramsXpanse.push_back(std::move(pongfeedbackRParameter));
    
    return {paramsXpanse.begin(), paramsXpanse.end()}; //Returning vector
}

//Degrade Parameters
juce::AudioProcessorValueTreeState::ParameterLayout NewProjectAudioProcessor::createDegradeParameterLayout()
{
    //Container for all parameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> paramsDegrade;
    
    auto degradeToggleParameter = std::make_unique<juce::AudioParameterBool>("degrade_toggle", "Degrade_Toggle", false);
    
    auto degradeWetDryParameter = std::make_unique<juce::AudioParameterFloat>("degrade_wetdry", "Degrade_WetDry", juce::NormalisableRange{0.0f, 1.f, 0.001f, 1.f, false}, 0.5f);
    
    auto degradeGainParameter = std::make_unique<juce::AudioParameterFloat>("degrade_gain", "Degrade_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(), juce::AudioProcessorParameter::genericParameter,[](float value, int){return juce::String(value, 2);});

    auto PRE_cutoffFrequencyParameter = std::make_unique<juce::AudioParameterFloat>("pre_cutoff_frequency", "Pre_Cutoff_Frequency", juce::NormalisableRange{20.f, 20000.f, 0.1f, 0.5f, false}, 20000.f);
    
    auto POST_cutoffFrequencyParameter = std::make_unique<juce::AudioParameterFloat>("post_cutoff_frequency", "Post_Cutoff_Frequency", juce::NormalisableRange{20.f, 20000.f, 0.1f, 0.5f, false}, 20000.f);
    
    auto downsampleFrequency = std::make_unique<juce::AudioParameterFloat>("degrade_frequency", "Degrade_Frequency", juce::NormalisableRange{10.f, 48000.f, 0.1f, 0.6f, false}, 48000.f);
    
    paramsDegrade.push_back(std::move(degradeToggleParameter));
    paramsDegrade.push_back(std::move(degradeWetDryParameter));
    paramsDegrade.push_back(std::move(degradeGainParameter));
    paramsDegrade.push_back(std::move(PRE_cutoffFrequencyParameter));
    paramsDegrade.push_back(std::move(POST_cutoffFrequencyParameter));
    paramsDegrade.push_back(std::move(downsampleFrequency));
    
    return {paramsDegrade.begin(), paramsDegrade.end()}; //Returning vector
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
    //mainProcessor->setPlayConfigDetails(getMainBusNumInputChannels(),
                                        //getMainBusNumOutputChannels(),
                                        //sampleRate, samplesPerBlock);
    //mainProcessor->prepareToPlay(sampleRate, samplesPerBlock);
    //initialiseGraph();
    
    filterEffect.prepareToPlay(sampleRate, samplesPerBlock);
    delayEffect.prepareToPlay(sampleRate, samplesPerBlock);
    convolutionEffect.prepareToPlay(sampleRate, samplesPerBlock);
    xpanseEffect.prepareToPlay(sampleRate, samplesPerBlock);
    degradeEffect.prepareToPlay(sampleRate, samplesPerBlock);
    
    //Create Spec to set up effects
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;
    
    
    //Global Effects
    lastGain = juce::Decibels::decibelsToGain(*Global_Parameters.getRawParameterValue("global_gain") + 0.0);
    
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
    //Block Processing Setup
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
//Audio processor graph setup and processing
    //updateGraph(); //Check for any changes to the graph
    //mainProcessor->processBlock(buffer, midiMessages); //Process this block through the graph
    
    filterEffect.processBlock(buffer, midiMessages);
    delayEffect.processBlock(buffer, midiMessages);
    convolutionEffect.processBlock(buffer, midiMessages);
    xpanseEffect.processBlock(buffer, midiMessages);
    degradeEffect.processBlock(buffer, midiMessages);
    
    
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
    return new NewProjectAudioProcessorEditor(*this, Global_Parameters, Filter_Parameters, Processing_Chain, Delay_Parameters, Convolution_Parameters, DelayXpanse_Parameters, Degrade_Parameters);
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

        // Set the state of Global_Parameters and Filter_Parameters using their respective value trees
        Global_Parameters.replaceState(globalValueTree);
        Processing_Chain.replaceState(chainValueTree);
        Filter_Parameters.replaceState(filterValueTree);
        Delay_Parameters.replaceState(delayValueTree);
        Convolution_Parameters.replaceState(convValueTree);
        DelayXpanse_Parameters.replaceState(xpanseValueTree);
        Degrade_Parameters.replaceState(degradeValueTree);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
//================================ Audio Processor Graph Functions ==============================================
//Following Cascading Plugin Effects tutorial

//Initial setup for graph
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

//Connect audio input to output (if no nodes in graph for example)
void NewProjectAudioProcessor::connectAudioNodes()
{
    for(int channel = 0; channel < 2; ++channel)
    {
        mainProcessor->addConnection({{audioInputNode->nodeID, channel},
                                        {audioOutputNode->nodeID, channel}});
    }
}

//Connect midi nodes (not really used in this plugin)
void NewProjectAudioProcessor::connectMidiNodes()
{
    mainProcessor->addConnection({{midiInputNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex},
                                    {midiOutputNode->nodeID, juce::AudioProcessorGraph::midiChannelIndex}});
}

//Update the graph
void NewProjectAudioProcessor::updateGraph()
{
    bool hasChanged = false; //Flag to say wether processing chain has changed or not
    
    
    juce::Array<juce::AudioParameterChoice*> choices{dynamic_cast<juce::AudioParameterChoice*>(Processing_Chain.getParameter("slot1")),
                                                     dynamic_cast<juce::AudioParameterChoice*>(Processing_Chain.getParameter("slot2")),
                                                     dynamic_cast<juce::AudioParameterChoice*>(Processing_Chain.getParameter("slot3")),
                                                     dynamic_cast<juce::AudioParameterChoice*>(Processing_Chain.getParameter("slot4"))};
    
    
    juce::ReferenceCountedArray<Node> slots;
    slots.add(slot1Node);
    slots.add(slot2Node);
    slots.add(slot3Node);
    //slots.add(slot4Node);
    
    //Loop through all choice parameters (the dropdown boxes in the chain menu) and check if any have changed, and if so what they have changed to
    //Null for empty, Filter for filter etc.
    for(int i = 0; i<slots.size()-1; i++)
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
        else if(choice->getIndex() == 2) //Delay
        {
            if(slot != nullptr)
            {
                if(slot->getProcessor()->getName() == "Delay")
                    continue;
                
                mainProcessor->removeNode(slot.get());
            }
            
            slots.set(i, mainProcessor->addNode(std::make_unique<DelayEffectAudioProcessor>(Delay_Parameters)));
            hasChanged = true;
        }
//        else if(choice->getIndex() == 3) //Convolution Reverb
//        {
//            if(slot != nullptr)
//            {
//                if(slot->getProcessor()->getName() == "Convolution")
//                    continue;
//
//                mainProcessor->removeNode(slot.get());
//            }
//
//            slots.set(i, mainProcessor->addNode(std::make_unique<ConvolutionReverbEffectAudioProcessor>(Convolution_Parameters)));
//            hasChanged = true;
//        }
        else if(choice->getIndex() == 4) //Convolution Reverb
        {
            if(slot != nullptr)
            {
                if(slot->getProcessor()->getName() == "Delay-Xpanse")
                    continue;

                mainProcessor->removeNode(slot.get());
            }

            slots.set(i, mainProcessor->addNode(std::make_unique<DelayXpanseEffectAudioProcessor>(DelayXpanse_Parameters)));
            hasChanged = true;
        }

    }
    
    
    //If a slot has changed update the graph
    if(hasChanged)
    {
        //Remove all graph connections
        for(auto connection : mainProcessor->getConnections())
        {
            mainProcessor->removeConnection(connection);
        }
        
        //Add all active (non empty) slots to an array
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
        
        //If there are no active slots (all empty) connect audio to output
        if(activeSlots.isEmpty())
        {
            connectAudioNodes();
        }
        else
        {
            if(activeSlots.size() == 1) //If there is one active slot
            {
                //Connect input and output through the active node
                for(int channel = 0; channel < 2; ++channel)
                {
                    mainProcessor->addConnection({{audioInputNode->nodeID, channel},
                                                    {activeSlots.getFirst()->nodeID, channel}});
                    
                    mainProcessor->addConnection({{activeSlots.getFirst()->nodeID, channel},
                                                    {audioOutputNode->nodeID, channel}});
                }
            }
            else //If there are multiple active slots
            {
                for(int i = 0; i < activeSlots.size()-1; ++i)
                {
                    //Connect input and output through the active nodes
                    
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
    slot3Node = slots.getUnchecked(2);
    //slot4Node = slots.getUnchecked(3);
}

