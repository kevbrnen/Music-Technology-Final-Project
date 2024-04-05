/*
  ==============================================================================

    HomeScreenComponent.h
    Created: 30 Mar 2024 5:05:34pm
    Author:  Kevin Brennan

    Home Screen Component UI
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FilterComponent.h"
#include "DelayEffectComponent.h"

//==============================================================================
/*
*/
class HomeScreenComponent  : public juce::Component
{
public:
    HomeScreenComponent(juce::AudioProcessorValueTreeState& Fvts, juce::AudioProcessorValueTreeState& Dvts): filtComponent(Fvts), delayComponent(Dvts)
    {
//Filter
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

//Delay
        //Delay Component
        addAndMakeVisible(delayComponent);
        delayComponent.setEnabled(false);
        delayComponent.setVisible(false);
        
        //Delay Button
        Delay_show_button.setButtonText("Delay");
        Delay_show_button.onClick = [this]()
        {
            hideHomeScreenComponents();
            delayComponent.setEnabled(true);
            delayComponent.setVisible(true);
            
            currentlyShowingComponent = 'D';
        };
        
        addAndMakeVisible(Delay_show_button);

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
        delayComponent.setBounds(getLocalBounds());
        
        auto sideMargin = getWidth()/8;
        auto topMargin = getHeight()/8;
        auto buttonWidth = getWidth()/8;
        Filter_show_button.setBounds(sideMargin, topMargin, buttonWidth, buttonWidth);
        Delay_show_button.setBounds(sideMargin + buttonWidth + sideMargin, topMargin, buttonWidth, buttonWidth);
        

    }
    
    //Hide any of the home screens native components (Filter selection buttons, etc.)
    void hideHomeScreenComponents()
    {
        Filter_show_button.setEnabled(false);
        Filter_show_button.setVisible(false);
        
        Delay_show_button.setEnabled(false);
        Delay_show_button.setVisible(false);
    }
    
    //Shows all home screen native components and hides whatever effect component was showing
    void showHomeScreenComponents()
    {
        switch(currentlyShowingComponent)
        {
            case 'F': //Filter
                filtComponent.setEnabled(false);
                filtComponent.setVisible(false);
                currentlyShowingComponent = 0;
                break;
            case 'D': //Delay
                delayComponent.setEnabled(false);
                delayComponent.setVisible(false);
                currentlyShowingComponent = 0;
                break;
            default:
                break;
        }
        
        Filter_show_button.setEnabled(true);
        Filter_show_button.setVisible(true);
        
        Delay_show_button.setEnabled(true);
        Delay_show_button.setVisible(true);
        
    }

private:
    char currentlyShowingComponent = 0; //Keeps track of what Effect is currently on screen
    
    FilterComponent filtComponent;
    DelayEffectComponent delayComponent;
    
    //Buttons to select Effects
    juce::TextButton Filter_show_button;
    juce::TextButton Delay_show_button;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomeScreenComponent)
};
