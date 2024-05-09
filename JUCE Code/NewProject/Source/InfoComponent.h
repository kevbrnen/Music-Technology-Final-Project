/*
  ==============================================================================

    InfoComponent.h
    Created: 4 May 2024 3:19:16pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "HomeScreenComponent.h"

//==============================================================================
/*
*/
class InfoComponent  : public juce::Component
{
public:
    InfoComponent(HomeScreenComponent& hs): hs(hs)
    {
        addAndMakeVisible(plugin_InfoLabel);
        plugin_InfoLabel.setColour (juce::Label::backgroundColourId, juce::Colours::transparentBlack);
        
        
        plugin_InfoText << "From the home screen you can select what effects you want to use.\nThe 'Chain' button allows you to choose what order to process through the effects in. Duplicating effects can become unstable, each effect only has one instance.\nYou have a global plug-in gain control in the top right corner.";
        
        filter_InfoText << "";
        
        delay_InfoText << "";
        
        conv_InfoText << "";
        
        xpanse_InfoText << "";
        
        degrade_InfoText << "";
        
        phaser_InfoText << "";
        
        reverb_InfoText << "";
        
        distortion_InfoText << "";

    }

    ~InfoComponent() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.fillAll (juce::Colours::black);   // clear the background

        g.setColour (juce::Colours::white);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        juce::Font boldFont ("Futura", 15.0f, juce::Font::bold);

        g.setFont (boldFont);
        
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
