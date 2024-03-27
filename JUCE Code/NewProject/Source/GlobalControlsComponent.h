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
        gainSlider.setNumDecimalPlacesToDisplay(2);
        gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "global_gain", gainSlider));
        gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        gainSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 100, 20);
        gainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
        gainSlider.setRange(-48.0, 0.0);
        gainSlider.setValue(-6.0);
        gainSlider.setTextValueSuffix("dB");
        gainSlider.setTextBoxIsEditable(true);
        addAndMakeVisible(gainSlider);
        
        
        gainLabel.setText("Plugin Gain", juce::dontSendNotification);
        gainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
        addAndMakeVisible(gainLabel);

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
    }

private:
    juce::Slider gainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>gainAttachment;
    juce::Label gainLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlobalControlsComponent)
};
