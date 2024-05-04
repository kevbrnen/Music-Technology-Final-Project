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
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

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
        g.drawRect (getLocalBounds(), 5);   // draw an outline around the component

        juce::Font boldFont ("Futura", 15.0f, juce::Font::bold);

        g.setFont (boldFont);
        
        switch(hs.getCurrentlyShowingComponent())
        {
            case 0:
                g.drawText ("Plug-in Info: ", getLocalBounds(),
                                juce::Justification::centred, true);
                break;
            case 'F': //Filter
                g.drawText ("Filter Effect Info: ", getLocalBounds(),
                            juce::Justification::centred, true);
                break;
            case 'D': //Delay
                g.drawText ("Delay Effect Info: ", getLocalBounds(),
                            juce::Justification::centred, true);
                break;
            case 'C': //Convolution Reverb
                g.drawText ("Convolution Reverb Effect Info:", getLocalBounds(),
                            juce::Justification::centred, true);
                break;
            case 'X': //Delay Xpanse
                g.drawText ("Delay-Xpanse Effect Info: ", getLocalBounds(),
                            juce::Justification::centred, true);
                break;
            case 'G': //Degrade
                g.drawText ("Degrade Effect Info: ", getLocalBounds(),
                            juce::Justification::centred, true);
                break;
            case 'P': //Phaser
                g.drawText ("Phaser Effect Info: ", getLocalBounds(),
                            juce::Justification::centred, true);
                break;
            case 'R': //Reverb
                g.drawText ("Reverb Effect Info: ", getLocalBounds(),
                            juce::Justification::centred, true);
                break;
            case 'Z': //Distortion
                g.drawText ("Distortion Effect Info: ", getLocalBounds(),
                            juce::Justification::centred, true);
                break;
            default:
                break;
        }
        
        this->repaint();
    }

    void resized() override
    {
    }
    
private:
    
    HomeScreenComponent& hs;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InfoComponent)
};
