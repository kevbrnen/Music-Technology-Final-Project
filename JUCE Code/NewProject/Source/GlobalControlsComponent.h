/*
  ==============================================================================

    GlobalControlsComponent.h
    Created: 10 Mar 2024 9:08:45pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "HomeScreenComponent.h"

//==============================================================================
/*
*/
class GlobalControlsComponent  : public juce::Component
{
public:
    GlobalControlsComponent(juce::AudioProcessorValueTreeState& vts, HomeScreenComponent& hs): hs(hs)
    {
        gainSlider.setNumDecimalPlacesToDisplay(2);
        gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "global_gain", gainSlider));
        gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        gainSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 100, 20);
        gainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
        gainSlider.setRange(-48.0, 0.0);
        //gainSlider.setValue(-6.0);
        gainSlider.setTextValueSuffix("dB");
        gainSlider.setTextBoxIsEditable(true);
        addAndMakeVisible(gainSlider);
        
        
        gainLabel.setText("Plugin Gain", juce::dontSendNotification);
        gainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
        addAndMakeVisible(gainLabel);
        
        homeButton.setButtonText("Home");
        //homeButton.setClickingTogglesState(true); //All of this can be moved to the "Home Screen" When created
        homeButton.onClick = [this, &hs](){ //Enables/disables depending on button state
                hs.showHomeScreenComponents();
        };
        
        homeButton.setEnabled(true);
        homeButton.setVisible(true);
        addAndMakeVisible(homeButton);
    }

    ~GlobalControlsComponent() override
    {
    }

    void paint (juce::Graphics& g) override
    {

        g.fillAll (juce::Colours::lightgrey);   // clear the background

        g.setColour (juce::Colours::grey);
        g.drawRect (getLocalBounds(), 5);   // draw an outline around the component

        g.setColour (juce::Colours::black);
        g.setFont (14.0f);
        g.drawText ("GlobalControlsComponent", getLocalBounds(),
                    juce::Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
        gainSlider.setBounds(getLocalBounds().getWidth() * 3/4, getLocalBounds().getHeight()/4, getLocalBounds().getWidth() * 1/4, getLocalBounds().getHeight()/2);
        gainLabel.attachToComponent(&gainSlider, true);
        
        homeButton.setBounds(10, getLocalBounds().getHeight()/6, 100, 50);
    }

private:
    HomeScreenComponent& hs;
    
    juce::Slider gainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>gainAttachment;
    juce::Label gainLabel;
    
    juce::TextButton homeButton;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlobalControlsComponent)
};
