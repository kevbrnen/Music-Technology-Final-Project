/*
  ==============================================================================

    PluginParameterLayout.h
    Created: 25 Apr 2024 10:31:45am
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
//==============================================================================
juce::StringArray processorChoices{"Empty", "Filter", "Delay", "Convolution", "Delay-Xpanse", "Degrade"};  //Effects that can be added to the chain

juce::StringArray IR_Choices{"Church-1", "Shipping Container", "Hall-1", "Tent"};
//==============================================================================

//Processing Chain
juce::AudioProcessorValueTreeState::ParameterLayout createProcessingParameterLayout()
{
    //Container for all parameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    auto slot1 = std::make_unique<juce::AudioParameterChoice>("slot1", "Slot1", processorChoices, 1);
    auto slot2 = std::make_unique<juce::AudioParameterChoice>("slot2", "Slot2", processorChoices, 2);
    auto slot3 = std::make_unique<juce::AudioParameterChoice>("slot3", "Slot3", processorChoices, 3);
    auto slot4 = std::make_unique<juce::AudioParameterChoice>("slot4", "Slot4", processorChoices, 4);
    auto slot5 = std::make_unique<juce::AudioParameterChoice>("slot5", "Slot5", processorChoices, 5);
    
    //Efficiently add parameter to list
    params.push_back(std::move(slot1));
    params.push_back(std::move(slot2));
    params.push_back(std::move(slot3));
    params.push_back(std::move(slot4));
    params.push_back(std::move(slot5));
    
    return {params.begin(), params.end()};
    
}

//Global Parameters
juce::AudioProcessorValueTreeState::ParameterLayout createGlobalParameterLayout()
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
juce::AudioProcessorValueTreeState::ParameterLayout createFilterParameterLayout()
{
    //Container for all parameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> paramsFilt;
    
    auto cutoffFrequencyParameter = std::make_unique<juce::AudioParameterFloat>("cutoff_frequency", "Cutoff_Frequency", juce::NormalisableRange{20.f, 20000.f, 0.1f, 0.2f, false}, 500.f);
    
    auto resonanceParameter = std::make_unique<juce::AudioParameterFloat>("resonance", "Resonance", juce::NormalisableRange{0.f, 10.f, 0.01f, 1.f, false}, 0.1f);
    
    auto filterToggleParameter = std::make_unique<juce::AudioParameterBool>("filter_toggle", "Filter_Toggle", false);
    
    auto filterGainParameter = std::make_unique<juce::AudioParameterFloat>("filter_gain", "Filter_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(),
                                                                           juce::AudioProcessorParameter::genericParameter, [](float value, int){return juce::String(value, 2);});
    auto filterLFOToggleParameter = std::make_unique<juce::AudioParameterBool>("filter_LFO_toggle", "Filter_LFO_Toggle", false);
    
    //Efficiently add parameter to list
    paramsFilt.push_back(std::move(cutoffFrequencyParameter));
    paramsFilt.push_back(std::move(resonanceParameter));
    paramsFilt.push_back(std::move(filterToggleParameter));
    paramsFilt.push_back(std::move(filterGainParameter));
    paramsFilt.push_back(std::move(filterLFOToggleParameter));

    
    return {paramsFilt.begin(), paramsFilt.end()}; //Returning vector

}

//Delay Parameters
juce::AudioProcessorValueTreeState::ParameterLayout createDelayParameterLayout()
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
juce::AudioProcessorValueTreeState::ParameterLayout createConvolutionParameterLayout()
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
juce::AudioProcessorValueTreeState::ParameterLayout createXpanseParameterLayout()
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
juce::AudioProcessorValueTreeState::ParameterLayout createDegradeParameterLayout()
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
