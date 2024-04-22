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
#include "ConvolutionReverbEffectComponent.h"
#include "DelayXpanseEffectComponent.h"
#include "DegradeEffectComponent.h"

//==============================================================================
/*
*/
class HomeScreenComponent  : public juce::Component
{
public:
    HomeScreenComponent(juce::AudioProcessorValueTreeState& Fvts, juce::AudioProcessorValueTreeState& Dvts, juce::AudioProcessorValueTreeState& Cvts, juce::AudioProcessorValueTreeState& Xvts, juce::AudioProcessorValueTreeState& DGvts): filtComponent(Fvts), delayComponent(Dvts), convolutionComponent(Cvts), delayXpanseComponent(Xvts), degradeComponent(DGvts)
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
        
//Convolution Reverb
        //Convolution Reverb Component
        addAndMakeVisible(convolutionComponent);
        convolutionComponent.setEnabled(false);
        convolutionComponent.setVisible(false);
        
        //Convolution Reverb Button
        Convolution_show_button.setButtonText("Convolution Reverb");
        Convolution_show_button.onClick = [this]()
        {
            hideHomeScreenComponents();
            convolutionComponent.setEnabled(true);
            convolutionComponent.setVisible(true);
            
            currentlyShowingComponent = 'C';
        };
        
        addAndMakeVisible(Convolution_show_button);
        
//DelayXpanse
        //DelayXpanse Component
        addAndMakeVisible(delayXpanseComponent);
        delayXpanseComponent.setEnabled(false);
        delayXpanseComponent.setVisible(false);
        
        //DelayXpanse Button
        DelayXpanse_show_button.setButtonText("Delay-Xpanse");
        DelayXpanse_show_button.onClick = [this]()
        {
            hideHomeScreenComponents();
            delayXpanseComponent.setEnabled(true);
            delayXpanseComponent.setVisible(true);
            
            currentlyShowingComponent = 'X';
        };
        
        addAndMakeVisible(DelayXpanse_show_button);
//Degrade
        //Degrade Component
        addAndMakeVisible(degradeComponent);
        degradeComponent.setEnabled(false);
        degradeComponent.setVisible(false);
        
        //Degrade Button
        Degrade_show_button.setButtonText("Degrade");
        Degrade_show_button.onClick = [this]()
        {
            hideHomeScreenComponents();
            degradeComponent.setEnabled(true);
            degradeComponent.setVisible(true);
            
            currentlyShowingComponent = 'G';
        };
        
        addAndMakeVisible(Degrade_show_button);

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
        convolutionComponent.setBounds(getLocalBounds());
        delayXpanseComponent.setBounds(getLocalBounds());
        degradeComponent.setBounds(getLocalBounds());
        
        
        auto sideMargin = getWidth()/10;
        auto topMargin = getHeight()/8;
        auto buttonWidth = getWidth()/8;
        //Row 1
        Filter_show_button.setBounds(sideMargin, topMargin, buttonWidth, buttonWidth);
        Delay_show_button.setBounds(sideMargin + buttonWidth + sideMargin, topMargin, buttonWidth, buttonWidth);
        Convolution_show_button.setBounds(sideMargin + buttonWidth + sideMargin + buttonWidth + sideMargin, topMargin, buttonWidth, buttonWidth);
        DelayXpanse_show_button.setBounds(sideMargin + buttonWidth + sideMargin + buttonWidth + sideMargin + buttonWidth + sideMargin, topMargin, buttonWidth, buttonWidth);
        //Row 2
        Degrade_show_button.setBounds(sideMargin, topMargin + buttonWidth + topMargin, buttonWidth, buttonWidth);
        

    }
    
    //Hide any of the home screens native components (Filter selection buttons, etc.)
    void hideHomeScreenComponents()
    {
        Filter_show_button.setEnabled(false);
        Filter_show_button.setVisible(false);
        
        Delay_show_button.setEnabled(false);
        Delay_show_button.setVisible(false);
        
        Convolution_show_button.setEnabled(false);
        Convolution_show_button.setVisible(false);
        
        DelayXpanse_show_button.setEnabled(false);
        DelayXpanse_show_button.setVisible(false);
        
        Degrade_show_button.setEnabled(false);
        Degrade_show_button.setVisible(false);
    
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
            case 'C': //Convolution Reverb
                convolutionComponent.setEnabled(false);
                convolutionComponent.setVisible(false);
                currentlyShowingComponent = 0;
                break;
            case 'X': //Convolution Reverb
                delayXpanseComponent.setEnabled(false);
                delayXpanseComponent.setVisible(false);
                currentlyShowingComponent = 0;
                break;
            case 'G': //Degrade Reverb
                degradeComponent.setEnabled(false);
                degradeComponent.setVisible(false);
                currentlyShowingComponent = 0;
                break;
            default:
                break;
        }
        
        Filter_show_button.setEnabled(true);
        Filter_show_button.setVisible(true);
        
        Delay_show_button.setEnabled(true);
        Delay_show_button.setVisible(true);
        
        Convolution_show_button.setEnabled(true);
        Convolution_show_button.setVisible(true);
        
        DelayXpanse_show_button.setEnabled(true);
        DelayXpanse_show_button.setVisible(true);
        
        Degrade_show_button.setEnabled(true);
        Degrade_show_button.setVisible(true);
        
    }

private:
    char currentlyShowingComponent = 0; //Keeps track of what Effect is currently on screen
    
    FilterComponent filtComponent;
    DelayEffectComponent delayComponent;
    ConvolutionReverbEffectComponent convolutionComponent;
    DelayXpanseEffectComponent delayXpanseComponent;
    DegradeEffectComponent degradeComponent;
    
    //Buttons to select Effects
    juce::TextButton Filter_show_button;
    juce::TextButton Delay_show_button;
    juce::TextButton Convolution_show_button;
    juce::TextButton DelayXpanse_show_button;
    juce::TextButton Degrade_show_button;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomeScreenComponent)
};