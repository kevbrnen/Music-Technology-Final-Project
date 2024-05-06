/*
  ==============================================================================

    PluginParameterLayout.h
    Created: 25 Apr 2024 10:31:45am
    Author:  Kevin Brennan

    Class to set up Audio Processor Value Tree States
    Adds parameters to each tree which we can access and manipulate using the UI
    Also allows us to set the range of each parameter and the type of parameter it is
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
//==============================================================================
juce::StringArray processorChoices{"Empty", "Filter", "Delay", "Convolution", "Delay-Xpanse", "Degrade", "Phaser", "Reverb", "Distortion"};  //Effects that can be added to the chain

juce::StringArray Filter_Choices{"Allpass", "Lowpass", "Bandpass", "Highpass"}; //Filter types

juce::StringArray IR_Choices{"Ballinafad Castle", "Ballymote Castle", "Bedroom", "Church 1", "Church 2", "Shipping Container", "Wooden Hall", "Farmhouse", "Forest", "Friary", "Tent", "Tunnel"}; //Impulse response choices

juce::StringArray Xpanse_Choices{"Ping-Pong Delay", "Spectral Delay"}; //Delay Xpanse types

juce::StringArray Reverb_Choices{"Schroeder Reverb", "Delaying Allpass Reverb", "FDN Reverb"}; //Reverb types

juce::StringArray Distortion_Choices{"Tanh", "Cubic Soft", "Square", "Clipping", "Soft Saturation"}; //Distortion Types
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
    auto slot6 = std::make_unique<juce::AudioParameterChoice>("slot6", "Slot6", processorChoices, 6);
    auto slot7 = std::make_unique<juce::AudioParameterChoice>("slot7", "Slot7", processorChoices, 7);
    auto slot8 = std::make_unique<juce::AudioParameterChoice>("slot8", "Slot8", processorChoices, 8);
    
    //Efficiently add parameter to list
    params.push_back(std::move(slot1));
    params.push_back(std::move(slot2));
    params.push_back(std::move(slot3));
    params.push_back(std::move(slot4));
    params.push_back(std::move(slot5));
    params.push_back(std::move(slot6));
    params.push_back(std::move(slot7));
    params.push_back(std::move(slot8));
    
    return {params.begin(), params.end()};
    
}

//Global Parameters
juce::AudioProcessorValueTreeState::ParameterLayout createGlobalParameterLayout()
{
    //Container for all parameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    //gain parameter
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
    
    auto resonanceParameter = std::make_unique<juce::AudioParameterFloat>("resonance", "Resonance", juce::NormalisableRange{0.1f, 10.f, 0.001f, .6f, false}, 0.1f);
    
    auto filterToggleParameter = std::make_unique<juce::AudioParameterBool>("filter_toggle", "Filter_Toggle", false);
    
    auto typeSelector = std::make_unique<juce::AudioParameterChoice>("filter_types", "Filter_Types", Filter_Choices, 0);
    
    
    auto cutoffFrequencyParameter2 = std::make_unique<juce::AudioParameterFloat>("cutoff_frequency2", "Cutoff_Frequency2", juce::NormalisableRange{20.f, 20000.f, 0.1f, 0.2f, false}, 500.f);
    
    auto resonanceParameter2 = std::make_unique<juce::AudioParameterFloat>("resonance2", "Resonance2", juce::NormalisableRange{0.1f, 10.f, 0.001f, .6f, false}, 0.1f);
    
    auto filterToggleParameter2 = std::make_unique<juce::AudioParameterBool>("filter_toggle2", "Filter_Toggle2", false);
    
    auto typeSelector2 = std::make_unique<juce::AudioParameterChoice>("filter_types2", "Filter_Types2", Filter_Choices, 0);
    
    
    auto filterGainParameter = std::make_unique<juce::AudioParameterFloat>("filter_gain", "Filter_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(),
                                                                           juce::AudioProcessorParameter::genericParameter, [](float value, int){return juce::String(value, 2);});
    auto filterLFOToggleParameter = std::make_unique<juce::AudioParameterBool>("filter_LFO_toggle", "Filter_LFO_Toggle", false);
    
    //Efficiently add parameter to list
    paramsFilt.push_back(std::move(cutoffFrequencyParameter));
    paramsFilt.push_back(std::move(resonanceParameter));
    paramsFilt.push_back(std::move(filterToggleParameter));
    paramsFilt.push_back(std::move(filterGainParameter));
    paramsFilt.push_back(std::move(filterLFOToggleParameter));
    paramsFilt.push_back(std::move(typeSelector));
    
    paramsFilt.push_back(std::move(cutoffFrequencyParameter2));
    paramsFilt.push_back(std::move(resonanceParameter2));
    paramsFilt.push_back(std::move(filterToggleParameter2));
    paramsFilt.push_back(std::move(typeSelector2));

    
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
    
    auto convPreGainParameter = std::make_unique<juce::AudioParameterFloat>("conv_pre_gain", "Conv_Pre_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(),
                                                                           juce::AudioProcessorParameter::genericParameter, [](float value, int){return juce::String(value, 2);});
    
    auto preDelayTimeParameter = std::make_unique<juce::AudioParameterFloat>("pre_delay_time", "Pre_Delay_Time", juce::NormalisableRange{0.0f, 249.f, 1.f, 0.6f, false}, 0.f);
    
    auto preDelayFeedbackParameter = std::make_unique<juce::AudioParameterFloat>("pre_delay_fdbk", "Pre_Delay_Fdbk", juce::NormalisableRange{0.0f, 0.999f, 0.001f, 1.f, false}, 0.0f);
    
    auto convGainParameter = std::make_unique<juce::AudioParameterFloat>("conv_gain", "Conv_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(),
                                                                           juce::AudioProcessorParameter::genericParameter, [](float value, int){return juce::String(value, 2);});
    
    auto impulseSelector = std::make_unique<juce::AudioParameterChoice>("selector", "Selector", IR_Choices, 0);
    
    paramsConv.push_back(std::move(convToggleParameter));
    paramsConv.push_back(std::move(convWetDryParameter));
    paramsConv.push_back(std::move(convPreGainParameter));
    paramsConv.push_back(std::move(convGainParameter));
    paramsConv.push_back(std::move(impulseSelector));
    paramsConv.push_back(std::move(preDelayTimeParameter));
    paramsConv.push_back(std::move(preDelayFeedbackParameter));
    
    
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
    auto typeSelector = std::make_unique<juce::AudioParameterChoice>("xpanse_type", "Xpanse_Type", Xpanse_Choices, 0);
    
    auto pongdelayLParameter = std::make_unique<juce::AudioParameterFloat>("pong_delay_L_time", "Pong_Delay_L_Time", juce::NormalisableRange{0.0f, 1999.f, 1.f, 0.4f, false}, 200.f);
    
    auto pongfeedbackLParameter = std::make_unique<juce::AudioParameterFloat>("pong_delay_L_fdbk", "Pong_Delay_L_FDBK", juce::NormalisableRange{0.0f, 0.999f, 0.001f, 1.f, false}, 0.5f);
    
    auto pongdelayRParameter = std::make_unique<juce::AudioParameterFloat>("pong_delay_R_time", "Pong_Delay_R_Time", juce::NormalisableRange{0.0f, 1999.f, 1.f, 0.4f, false}, 200.f);
    
    auto pongfeedbackRParameter = std::make_unique<juce::AudioParameterFloat>("pong_delay_R_fdbk", "Pong_Delay_R_FDBK", juce::NormalisableRange{0.0f, 0.999f, 0.001f, 1.f, false}, 0.5f);
    
    
    auto band1Gain = std::make_unique<juce::AudioParameterFloat>("spec_band1_gain", "Spec_Band1_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(),
                                                                           juce::AudioProcessorParameter::genericParameter, [](float value, int){return juce::String(value, 2);});
    auto band2Gain = std::make_unique<juce::AudioParameterFloat>("spec_band2_gain", "Spec_Band2_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(),
                                                                           juce::AudioProcessorParameter::genericParameter, [](float value, int){return juce::String(value, 2);});
    auto band3Gain = std::make_unique<juce::AudioParameterFloat>("spec_band3_gain", "Spec_Band3_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(),
                                                                           juce::AudioProcessorParameter::genericParameter, [](float value, int){return juce::String(value, 2);});
    auto band4Gain = std::make_unique<juce::AudioParameterFloat>("spec_band4_gain", "Spec_Band4_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(),
                                                                           juce::AudioProcessorParameter::genericParameter, [](float value, int){return juce::String(value, 2);});
    
    auto band1time = std::make_unique<juce::AudioParameterFloat>("spec_band1_time", "Spec_Band1_Time", juce::NormalisableRange{0.0f, 1499.f, 1.f, 0.4f, false}, 200.f);
    auto band2time = std::make_unique<juce::AudioParameterFloat>("spec_band2_time", "Spec_Band1_Time", juce::NormalisableRange{0.0f, 1499.f, 1.f, 0.4f, false}, 200.f);
    auto band3time = std::make_unique<juce::AudioParameterFloat>("spec_band3_time", "Spec_Band1_Time", juce::NormalisableRange{0.0f, 1499.f, 1.f, 0.4f, false}, 200.f);
    auto band4time = std::make_unique<juce::AudioParameterFloat>("spec_band4_time", "Spec_Band1_Time", juce::NormalisableRange{0.0f, 1499.f, 1.f, 0.4f, false}, 200.f);
    
    auto band1fdbk = std::make_unique<juce::AudioParameterFloat>("spec_band1_fdbk", "Spec_Band1_Fdbk", juce::NormalisableRange{0.0f, 0.999f, 0.001f, 1.f, false}, 0.5f);
    auto band2fdbk = std::make_unique<juce::AudioParameterFloat>("spec_band2_fdbk", "Spec_Band2_Fdbk", juce::NormalisableRange{0.0f, 0.999f, 0.001f, 1.f, false}, 0.5f);
    auto band3fdbk = std::make_unique<juce::AudioParameterFloat>("spec_band3_fdbk", "Spec_Band3_Fdbk", juce::NormalisableRange{0.0f, 0.999f, 0.001f, 1.f, false}, 0.5f);
    auto band4fdbk = std::make_unique<juce::AudioParameterFloat>("spec_band4_fdbk", "Spec_Band4_Fdbk", juce::NormalisableRange{0.0f, 0.999f, 0.001f, 1.f, false}, 0.5f);
    
    auto cutoff1 = std::make_unique<juce::AudioParameterFloat>("spec_cutoff1", "Spec_Cutoff1", juce::NormalisableRange{20.f, 20000.f, 0.1f, 0.2f, false}, 500.f);
    auto cutoff2 = std::make_unique<juce::AudioParameterFloat>("spec_cutoff2", "Spec_Cutoff2", juce::NormalisableRange{20.f, 20000.f, 0.1f, 0.2f, false}, 2000.f);
    auto cutoff3 = std::make_unique<juce::AudioParameterFloat>("spec_cutoff3", "Spec_Cutoff3", juce::NormalisableRange{20.f, 20000.f, 0.1f, 0.2f, false}, 6000.f);
    
    paramsXpanse.push_back(std::move(xpanseToggleParameter));
    paramsXpanse.push_back(std::move(xpanseWetDryParameter));
    paramsXpanse.push_back(std::move(xpanseGainParameter));
    paramsXpanse.push_back(std::move(typeSelector));
    
    paramsXpanse.push_back(std::move(pongdelayLParameter));
    paramsXpanse.push_back(std::move(pongfeedbackLParameter));
    paramsXpanse.push_back(std::move(pongdelayRParameter));
    paramsXpanse.push_back(std::move(pongfeedbackRParameter));
    
    paramsXpanse.push_back(std::move(band1Gain));
    paramsXpanse.push_back(std::move(band2Gain));
    paramsXpanse.push_back(std::move(band3Gain));
    paramsXpanse.push_back(std::move(band4Gain));
    paramsXpanse.push_back(std::move(band1time));
    paramsXpanse.push_back(std::move(band2time));
    paramsXpanse.push_back(std::move(band3time));
    paramsXpanse.push_back(std::move(band4time));
    paramsXpanse.push_back(std::move(band1fdbk));
    paramsXpanse.push_back(std::move(band2fdbk));
    paramsXpanse.push_back(std::move(band3fdbk));
    paramsXpanse.push_back(std::move(band4fdbk));
    paramsXpanse.push_back(std::move(cutoff1));
    paramsXpanse.push_back(std::move(cutoff2));
    paramsXpanse.push_back(std::move(cutoff3));
    
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
    
    auto bitcrushToggleParameter = std::make_unique<juce::AudioParameterBool>("bitcrush_toggle", "Bitcrush_Toggle", false);
    
    auto bitRate = std::make_unique<juce::AudioParameterFloat>("bitcrush_rate", "Bitcrush_Rate", juce::NormalisableRange{1.f, 24.f, 1.f, 1.f, false}, 24.f);
    
    paramsDegrade.push_back(std::move(degradeToggleParameter));
    paramsDegrade.push_back(std::move(degradeWetDryParameter));
    paramsDegrade.push_back(std::move(degradeGainParameter));
    paramsDegrade.push_back(std::move(PRE_cutoffFrequencyParameter));
    paramsDegrade.push_back(std::move(POST_cutoffFrequencyParameter));
    paramsDegrade.push_back(std::move(downsampleFrequency));
    paramsDegrade.push_back(std::move(bitcrushToggleParameter));
    paramsDegrade.push_back(std::move(bitRate));
    
    return {paramsDegrade.begin(), paramsDegrade.end()}; //Returning vector
}

//Phaser Parameters
juce::AudioProcessorValueTreeState::ParameterLayout createPhaserParameterLayout()
{
    //Container for all parameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> paramsPhaser;
    
    auto phaserToggleParameter = std::make_unique<juce::AudioParameterBool>("phaser_toggle", "Phaser_Toggle", false);
    
    auto phaserGainParameter = std::make_unique<juce::AudioParameterFloat>("phaser_gain", "Phaser_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(), juce::AudioProcessorParameter::genericParameter,[](float value, int){return juce::String(value, 2);});
    
    auto phaserIntensity = std::make_unique<juce::AudioParameterFloat>("phaser_intensity", "Phaser_Intensity", juce::NormalisableRange{1.f, 4.f, 1.f, 1.f, false}, 1.f);
    
    auto phaserSpeed = std::make_unique<juce::AudioParameterFloat>("phaser_lfo_speed", "Phaser_LFO_Speed", juce::NormalisableRange{0.f, 10.f, 0.001f, 0.6f, false}, 0.5f);
    
    auto phaserWetDryParameter = std::make_unique<juce::AudioParameterFloat>("phaser_wet_dry", "Phaser_Wet_Dry", juce::NormalisableRange{0.0f, 1.f, 0.001f, 1.f, false}, 0.5f);
    
    auto phaserQParameter = std::make_unique<juce::AudioParameterFloat>("phaser_q", "Phaser_Q", juce::NormalisableRange{0.0f, 10.f, 0.001f, 1.f, false}, 1.f);
    
    auto phaserModWidthParameter = std::make_unique<juce::AudioParameterFloat>("phaser_mod_width", "Phaser_Mod_Width", juce::NormalisableRange{0.0f, 1.f, 0.001f, 1.f, false}, 0.5f);
    
    auto phaserFDBKParameter = std::make_unique<juce::AudioParameterFloat>("phaser_fdbk", "Phaser_FDBK", juce::NormalisableRange{0.0f, 1.f, 0.001f, 1.f, false}, 0.5f);
    
    
    
    paramsPhaser.push_back(std::move(phaserToggleParameter));
    paramsPhaser.push_back(std::move(phaserGainParameter));
    paramsPhaser.push_back(std::move(phaserIntensity));
    paramsPhaser.push_back(std::move(phaserSpeed));
    paramsPhaser.push_back(std::move(phaserWetDryParameter));
    paramsPhaser.push_back(std::move(phaserQParameter));
    paramsPhaser.push_back(std::move(phaserModWidthParameter));
    paramsPhaser.push_back(std::move(phaserFDBKParameter));

    
    return {paramsPhaser.begin(), paramsPhaser.end()}; //Returning vector
}

//Reverb Parameters
juce::AudioProcessorValueTreeState::ParameterLayout createReverbParameterLayout()
{
    //Container for all parameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> paramsReverb;
    
    auto reverbToggleParameter = std::make_unique<juce::AudioParameterBool>("reverb_toggle", "Reverb_Toggle", false);
    
    auto reverbGainParameter = std::make_unique<juce::AudioParameterFloat>("reverb_gain", "Reverb_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(), juce::AudioProcessorParameter::genericParameter,[](float value, int){return juce::String(value, 2);});
    
    auto reverbWetDryParameter = std::make_unique<juce::AudioParameterFloat>("reverb_wetdry", "Reverb_Wetdry", juce::NormalisableRange{0.0f, 1.f, 0.001f, 1.f, false}, 0.5f);
    
    auto reverbSelector = std::make_unique<juce::AudioParameterChoice>("reverb_type", "Reverb_Type", Reverb_Choices, 0);
    
    
    auto preGainParameter = std::make_unique<juce::AudioParameterFloat>("reverb_pre_gain", "Reverb_Pre_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(),
                                                                           juce::AudioProcessorParameter::genericParameter, [](float value, int){return juce::String(value, 2);});
    auto preTimeParameter = std::make_unique<juce::AudioParameterFloat>("reverb_pre_delay", "Reverb_Pre_Delay", juce::NormalisableRange{0.0f, 2000.f, 1.f, 0.4f, false}, 0.f);
    
    
    
    auto PRE_cutoffFrequencyParameter = std::make_unique<juce::AudioParameterFloat>("reverb_pre_cutoff", "Reverb_Pre_Cutoff", juce::NormalisableRange{20.f, 20000.f, 0.1f, 0.5f, false}, 20000.f);
    
    auto resonancePre = std::make_unique<juce::AudioParameterFloat>("reverb_pre_resonance", "Reverb_Pre_Resonance", juce::NormalisableRange{0.f, 10.f, 0.01f, 1.f, false}, 0.1f);
    
    auto preTypeSelector = std::make_unique<juce::AudioParameterChoice>("reverb_pre_filter_type", "Reverb_Pre_Filter_Type", Filter_Choices, 0);
    
    
    
    
    auto APF1fdbk = std::make_unique<juce::AudioParameterFloat>("apf1_fdbk", "Apf1_Fdbk", juce::NormalisableRange{0.0f, 0.999f, 0.001f, 1.f, false}, 0.1f);
    
    auto combTimeParameter = std::make_unique<juce::AudioParameterFloat>("comb_delay_time", "Comb_Delay_Time", juce::NormalisableRange{1.0f, 2000.f, 1.f, 0.4f, false}, 100.f);
    
    
    
    auto dapTimeParameter = std::make_unique<juce::AudioParameterFloat>("reverb_dap_delay_time", "Reverb_DAP_Delay_Time", juce::NormalisableRange{0.0f, 2000.f, 1.f, 0.4f, false}, 0.0f);
    
    auto dapGParameter = std::make_unique<juce::AudioParameterFloat>("reverb_dap_g", "Reverb_DAP_G", juce::NormalisableRange{0.0f, 0.999f, 0.001f, 0.4f, false}, 0.0f);
    
    
    
    auto fdnTimeParameter = std::make_unique<juce::AudioParameterFloat>("fdn_time", "FDN_Time", juce::NormalisableRange{0.0f, 2000.f, 1.f, 0.4f, false}, 25.f);
    
    auto fdnFDBKParameter = std::make_unique<juce::AudioParameterFloat>("fdn_fdbk", "FDN_FDBK", juce::NormalisableRange{-0.999f, 0.999f, 0.001f, 1.f, false}, 0.0f);
    
    
    paramsReverb.push_back(std::move(reverbToggleParameter));
    paramsReverb.push_back(std::move(reverbGainParameter));
    paramsReverb.push_back(std::move(reverbWetDryParameter));
    paramsReverb.push_back(std::move(reverbSelector));
    
    paramsReverb.push_back(std::move(preGainParameter));
    paramsReverb.push_back(std::move(preTimeParameter));
    
    paramsReverb.push_back(std::move(PRE_cutoffFrequencyParameter));
    paramsReverb.push_back(std::move(resonancePre));
    paramsReverb.push_back(std::move(preTypeSelector));
    
    paramsReverb.push_back(std::move(APF1fdbk));
    paramsReverb.push_back(std::move(combTimeParameter));
    
    paramsReverb.push_back(std::move(dapTimeParameter));
    paramsReverb.push_back(std::move(dapGParameter));
    
    paramsReverb.push_back(std::move(fdnTimeParameter));
    paramsReverb.push_back(std::move(fdnFDBKParameter));
    
    return {paramsReverb.begin(), paramsReverb.end()}; //Returning vector
}

//Distortion Parameters
juce::AudioProcessorValueTreeState::ParameterLayout createDistortionParameterLayout()
{
    //Container for all parameters
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> paramsDistortion;
    
    auto distToggleParameter = std::make_unique<juce::AudioParameterBool>("distortion_toggle", "Distortion_Toggle", false);
    
    auto distGainParameter = std::make_unique<juce::AudioParameterFloat>("distortion_gain", "Distortion_Gain", juce::NormalisableRange<float>(-48.0f, 10.0f), 0.0f, juce::String(), juce::AudioProcessorParameter::genericParameter,[](float value, int){return juce::String(value, 2);});
    
    auto distWetDryParameter = std::make_unique<juce::AudioParameterFloat>("distortion_wetdry", "Distortion_Wetdry", juce::NormalisableRange{0.0f, 1.f, 0.001f, 1.f, false}, 0.f);
    
    auto distSelector = std::make_unique<juce::AudioParameterChoice>("distortion_type", "Distortion_Type", Distortion_Choices, 0);
    
    auto preGain = std::make_unique<juce::AudioParameterFloat>("distortion_pregain", "Distortion_PreGain", juce::NormalisableRange{0.1f, 10.f, 0.001f, 1.f, false}, 1.f);
    
    auto thresh = std::make_unique<juce::AudioParameterFloat>("distortion_thresh", "Distortion_thresh", juce::NormalisableRange{0.f, 1.f, 0.001f, 1.f, false}, 0.5f);
    
    auto PRE_cutoffFrequencyParameter = std::make_unique<juce::AudioParameterFloat>("distortion_pre_cutoff", "Distortion_Pre_Cutoff", juce::NormalisableRange{20.f, 20000.f, 0.1f, 0.5f, false}, 20000.f);
    
    auto POST_cutoffFrequencyParameter = std::make_unique<juce::AudioParameterFloat>("distortion_post_cutoff", "Distortion_Post_Cutoff", juce::NormalisableRange{20.f, 20000.f, 0.1f, 0.5f, false}, 20000.f);
    
    auto resonancePre = std::make_unique<juce::AudioParameterFloat>("distortion_pre_resonance", "Distortion_Pre_Resonance", juce::NormalisableRange{0.f, 10.f, 0.01f, 1.f, false}, 0.1f);
    
    auto resonancePost = std::make_unique<juce::AudioParameterFloat>("distortion_post_resonance", "Distortion_Post_Resonance", juce::NormalisableRange{0.f, 10.f, 0.01f, 1.f, false}, 0.1f);
    
    auto preTypeSelector = std::make_unique<juce::AudioParameterChoice>("pre_filter_type", "Pre_Filter_Type", Filter_Choices, 0);
    
    auto postTypeSelector = std::make_unique<juce::AudioParameterChoice>("post_filter_type", "Post_Filter_Type", Filter_Choices, 0);
    
    
    paramsDistortion.push_back(std::move(distToggleParameter));
    paramsDistortion.push_back(std::move(distGainParameter));
    paramsDistortion.push_back(std::move(distWetDryParameter));
    paramsDistortion.push_back(std::move(distSelector));
    paramsDistortion.push_back(std::move(preGain));
    paramsDistortion.push_back(std::move(thresh));
    paramsDistortion.push_back(std::move(PRE_cutoffFrequencyParameter));
    paramsDistortion.push_back(std::move(POST_cutoffFrequencyParameter));
    paramsDistortion.push_back(std::move(resonancePre));
    paramsDistortion.push_back(std::move(resonancePost));
    paramsDistortion.push_back(std::move(preTypeSelector));
    paramsDistortion.push_back(std::move(postTypeSelector));
    
    return {paramsDistortion.begin(), paramsDistortion.end()}; //Returning vector
}
