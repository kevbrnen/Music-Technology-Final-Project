/*
  ==============================================================================

    HomeScreenComponent.h
    Created: 30 Mar 2024 5:05:34pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
/*
*/
class HomeScreenComponent  : public juce::Component
{
public:
    HomeScreenComponent(juce::AudioProcessorValueTreeState& Fvts): filtComponent(Fvts)
    {
        
        //Filter Component
        addAndMakeVisible(filtComponent); //Add it
        filtComponent.setEnabled(false); //Deactivate it and make it invisible
        filtComponent.setVisible(false);
        
        //Filter Button
        Filter_show_button.setButtonText("Filter"); //Create a button to toggle the filter component on a*nd off
        //Filter_show_button.setClickingTogglesState(true);
        Filter_show_button.onClick = [this](){ //Enables/disables depending on button state
                hideHomeScreenComponents();
                filtComponent.setEnabled(true);
                filtComponent.setVisible(true);
                
                currentlyShowingComponent = 'F'; //Tells us what component is currently being shown
        };
        
        addAndMakeVisible(Filter_show_button);

    }

    ~HomeScreenComponent() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        auto purpleHue = juce::Colours::purple.getHue();
        g.fillAll(juce::Colour::fromHSV(purpleHue, 0.5f, 0.5f, 1.0f));
        
        g.setColour(juce::Colours::lightgrey);
        g.drawRect(getLocalBounds(), 5);
        
        g.setColour(juce::Colours::black);
        g.setFont(14.0f);
        g.drawText("HomeScreenComponent", getLocalBounds(), juce::Justification::centred, true);
        
    }

    void resized() override
    {
        filtComponent.setBounds(getLocalBounds());
        
        auto sideMargin = getWidth()/8;
        auto topMargin = getHeight()/8;
        auto buttonWidth = getWidth()/8;
        Filter_show_button.setBounds(sideMargin, topMargin, buttonWidth, buttonWidth);

    }
    
    void hideHomeScreenComponents()
    {
        Filter_show_button.setEnabled(false);
        Filter_show_button.setVisible(false);
    }
    
    void showHomeScreenComponents()
    {
        switch(currentlyShowingComponent)
        {
            case 'F':
                filtComponent.setEnabled(false);
                filtComponent.setVisible(false);
                currentlyShowingComponent = 0;
                break;
            default:
                break;
        }
        
        Filter_show_button.setEnabled(true);
        Filter_show_button.setVisible(true);
        
    }

private:
    char currentlyShowingComponent = 0;
    
    FilterComponent filtComponent;
    
    //Buttons to select Effects
    juce::TextButton Filter_show_button;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomeScreenComponent)
};
