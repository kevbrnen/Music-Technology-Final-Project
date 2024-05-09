/*
  ==============================================================================

    InfoComponent.h
    Created: 4 May 2024 3:19:16pm
    Author:  Kevin Brennan

    A class for displaying info about the plug-in and the effects
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "HomeScreenComponent.h"


class InfoComponent  : public juce::Component
{
public:
    InfoComponent(HomeScreenComponent& hs): hs(hs)
    {
        //Label to show info text
        addAndMakeVisible(plugin_InfoLabel);
        plugin_InfoLabel.setColour (juce::Label::backgroundColourId, juce::Colours::transparentBlack);
        
        //Info text for each part of the plugin
        plugin_InfoText << "From the home screen you can select what effects you want to use.\nThe 'Chain' button allows you to choose what order to process through the effects in. Duplicating effects can become unstable, each effect only has one instance.\nYou have a global plug-in gain control in the top right corner.";
        
        filter_InfoText << "Filter 1 (left) is always on.\nFilter 2 (right) can be toggled on/off.\nYou can select different types of filters using the dropdown menus.\nYou have control over the cutoff frequencies and resonances of each filter.\n\nYou can control cutoff frequencies and resonances using the LFO. Toggle it on and choose the amount each parameter is modulated around the set parameter values.";
        
        delay_InfoText << "Delay Time determines the current delay time.\nFeedback amount controls how much of the delayed sound is fed back into the delay.\nWet/Dry amount controls the mix between the original and delayed signals.\n\nYou can control delay time, feedback amount, and wet/dry amount using the LFO. Toggle it on and choose the amount each parameter is modulated around the set parameter values.";
        
        conv_InfoText << "Use the dropdown menu to change the impulse response to use for convolution.\nThe pre gain controls the level of the wet signal before any processing is applied.\nYou can add a pre delay to help improve the clarity of the resulting sound, or to create a greater sense of space.\nWet/Dry amount controls the mix between the original and convolved signals.";
        
        xpanse_InfoText << "Ping-Pong Delay:\nYou have individual control over left and right delay times and feedbacks. Inputs and feedback paths are crossed over. Having different delay times on each side creates the back and forth effect.\n\nSpectral Delay:\nThe three cutoff frequency filters control the frequencies of each of the 4 bands.\nWith each band you have control over the gain, delay time and feedback amounts. You can delay each band by different amounts, resulting in the spectral delay effect.";
        
        degrade_InfoText << "The degrade frequency determines the sample rate to downsample to.\nTry setting the pre (lowpass) filter frequency to half the set sample rate to try and avoid aliasing.\nYou can use the post (lowpass) filter to further remove higher frequencies or buzzing.\n\nYou can toggle the bitcrusher on and off. Lowering the bit rate reduces the dynamic range of audio, it will also result in quantization noise.";
        
        phaser_InfoText << "This Phaser is loosely based on an MXR-100 phaser pedal. It contains 10 allpass filters in series, the cutoff frequencies of which are controlled by the LFO.\nThe modulation speed, modulation width and Q value all control the LFO, how fast the phasing effect is.\nThe feedback amount controls how much of the phased signal is fed back into the allpass filter chain.\nThe intensity value decides how far apart the cutoff frequencies of the allpass filters are.\nTry adjusting the wet/dry amount to control how the original and phase shifted signals are mixed.";
        
        reverb_InfoText << "Schroeder Reverb:\nA simple Schroeder Reverberator implementation. The comb filter delay influences the characteristics of early reflections. The allpass feedback amount controls the late reflections.\nDelaying Allpass Reverb:\nTime control gives users control over the delay time of the allpass filter. The G value controls the feedback.\nFDN Reverb:\nA simple Feedback Delay Network implementation. The time knob controls the delay times of all delay lines. The feedback control controls the feedback matrix values.";
    
        distortion_InfoText << "The gain factor boosts the input signal before clipping is applied.\nThe threshold determines above what level distortion will be applied to the signal.\nYou can change the type of distortion using the dropdown menu on the right\nBe careful when changing distortion types as some are much louder than others. Use the effect gain to lower the volume if the effect becomes too loud.";

    }

    ~InfoComponent() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colours::black);   // clear the background

        g.setColour (juce::Colours::white);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        juce::Font boldFont ("Futura", 15.0f, juce::Font::bold);

        g.setFont (boldFont);
        
        //Change info text depending on the currently showing component
        switch(hs.getCurrentlyShowingComponent())
        {
            case 0:
                g.drawText ("Plug-in Info: ", 10, 10, 300, 25,
                                juce::Justification::topLeft, true);
                plugin_InfoLabel.setText(plugin_InfoText, juce::dontSendNotification);
                break;
            case 'F': //Filter
                g.drawText ("Filter Effect Info: ", 10, 10, 300, 25,
                            juce::Justification::topLeft, true);
                plugin_InfoLabel.setText(filter_InfoText, juce::dontSendNotification);
                break;
            case 'D': //Delay
                g.drawText ("Delay Effect Info: ", 10, 10, 300, 25,
                            juce::Justification::topLeft, true);
                plugin_InfoLabel.setText(delay_InfoText, juce::dontSendNotification);
                break;
            case 'C': //Convolution Reverb
                g.drawText ("Convolution Reverb Effect Info:", 10, 10, 300, 25,
                            juce::Justification::topLeft, true);
                plugin_InfoLabel.setText(conv_InfoText, juce::dontSendNotification);
                break;
            case 'X': //Delay Xpanse
                g.drawText ("Delay-Xpanse Effect Info: ", 10, 10, 300, 25,
                            juce::Justification::topLeft, true);
                plugin_InfoLabel.setText(xpanse_InfoText, juce::dontSendNotification);
                break;
            case 'G': //Degrade
                g.drawText ("Degrade Effect Info: ", 10, 10, 300, 25,
                            juce::Justification::topLeft, true);
                plugin_InfoLabel.setText(degrade_InfoText, juce::dontSendNotification);
                break;
            case 'P': //Phaser
                g.drawText ("Phaser Effect Info: ", 10, 10, 300, 25,
                            juce::Justification::topLeft, true);
                plugin_InfoLabel.setText(phaser_InfoText, juce::dontSendNotification);
                break;
            case 'R': //Reverb
                g.drawText ("Reverb Effect Info: ", 10, 10, 300, 25,
                            juce::Justification::topLeft, true);
                plugin_InfoLabel.setText(reverb_InfoText, juce::dontSendNotification);
                break;
            case 'Z': //Distortion
                g.drawText ("Distortion Effect Info: ", 10, 10, 300, 25,
                            juce::Justification::topLeft, true);
                plugin_InfoLabel.setText(distortion_InfoText, juce::dontSendNotification);
                break;
            default:
                break;
        }
        
        this->repaint();
    }

    void resized() override
    {
        plugin_InfoLabel.setBounds(getLocalBounds());
    }
    
private:
    
    HomeScreenComponent& hs;
    
    juce::Label plugin_InfoLabel;
    juce::String plugin_InfoText, filter_InfoText, delay_InfoText, conv_InfoText, xpanse_InfoText, degrade_InfoText, phaser_InfoText, reverb_InfoText, distortion_InfoText;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InfoComponent)
};
