/*
  ==============================================================================

    FilterComponent.cpp
    Created: 9 Mar 2024 10:36:04pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& vts) 
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(cutoffFrequencySlider);
    
    cutoffFrequencySlider.setSliderStyle(
          juce::Slider::SliderStyle::LinearVertical);
      cutoffFrequencyAttachment.reset(
          new juce::AudioProcessorValueTreeState::SliderAttachment(
              vts, "lowpass_cutoff_frequency", cutoffFrequencySlider));
    
    addAndMakeVisible(cutoffFrequencyLabel);
     cutoffFrequencyLabel.setText("Cutoff Frequency",
                                  juce::dontSendNotification);
    
    setSize(150, 300);
}

FilterComponent::~FilterComponent()
{
}

void FilterComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::blue);   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 5);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("FilterComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void FilterComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    cutoffFrequencySlider.setBounds({15, 35, 100, 300});
      cutoffFrequencyLabel.setBounds({cutoffFrequencySlider.getX() + 30,cutoffFrequencySlider.getY() - 30,200, 50});

}
