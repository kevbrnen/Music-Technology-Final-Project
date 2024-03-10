/*
  ==============================================================================

    GlobalControlsComponent.h
    Created: 10 Mar 2024 9:08:45pm
    Author:  Kevin Brennan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class GlobalControlsComponent  : public juce::Component
{
public:
    GlobalControlsComponent(juce::AudioProcessorValueTreeState& vts)
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        addAndMakeVisible(gainSlider);
        
        gainSlider.setSliderStyle(
              juce::Slider::SliderStyle::LinearVertical);
          gainAttachment.reset(
              new juce::AudioProcessorValueTreeState::SliderAttachment(
                  vts, "global_gain", gainSlider));
        
        addAndMakeVisible(gainLabel);
        gainLabel.setText("Gain", juce::dontSendNotification);

    }

    ~GlobalControlsComponent() override
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

        g.setColour (juce::Colours::grey);
        g.drawRect (getLocalBounds(), 5);   // draw an outline around the component

        g.setColour (juce::Colours::white);
        g.setFont (14.0f);
        g.drawText ("GlobalControlsComponent", getLocalBounds(),
                    juce::Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
        gainSlider.setBounds({15, 35, 100, 300});
          gainLabel.setBounds({gainSlider.getX() + 30,gainSlider.getY() - 30,200, 50});


    }

private:
    juce::Slider gainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>gainAttachment;
    juce::Label gainLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlobalControlsComponent)
};
