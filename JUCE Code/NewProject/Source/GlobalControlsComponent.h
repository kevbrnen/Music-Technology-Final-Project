/*
  ==============================================================================

    GlobalControlsComponent.h
    Created: 10 Mar 2024 9:08:45pm
    Author:  Kevin Brennan
    
    The global controls component bar at the top of the plugin
  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginEditor.h"
#include "HomeScreenComponent.h"
#include "ProcessingChainSelectionComponent.h"
#include "InfoComponent.h"

//==============================================================================
/*
*/
class GlobalControlsComponent  : public juce::Component
{
public:
    GlobalControlsComponent(juce::AudioProcessorValueTreeState& vts, HomeScreenComponent& hs, ProcessingChainSelectionComponent& pc, InfoComponent& ic): hs(hs), pc(pc), ic(ic)
    {
//Gain Slider
        gainSlider.setNumDecimalPlacesToDisplay(2);
        gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "global_gain", gainSlider));
        gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        gainSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 100, 20);
        gainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
        gainSlider.setRange(-48.0, 10.0);
        gainSlider.setTextValueSuffix("dB");
        gainSlider.setTextBoxIsEditable(true);
        addAndMakeVisible(gainSlider);
        
        gainLabel.setText("Plugin Gain", juce::dontSendNotification);
        gainLabel.setColour(juce::Label::textColourId, juce::Colours::black);
        addAndMakeVisible(gainLabel);
        
//Home Button
        homeButton.setButtonText("Home");
        homeButton.onClick = [this, &hs](){ //Enables/disables depending on button state
                hs.showHomeScreenComponents();
        };
        homeButton.setEnabled(true);
        homeButton.setVisible(true);
        addAndMakeVisible(homeButton);
        
//Chain Menu Button
        chainMenuButton.setButtonText("Chain");
        chainMenuButton.setClickingTogglesState(true);
        chainMenuButton.onClick = [this, &pc](){
            pc.setEnabled(chainMenuButton.getToggleState());
            pc.setVisible(chainMenuButton.getToggleState());

        };
        chainMenuButton.setEnabled(true);
        chainMenuButton.setVisible(true);
        addAndMakeVisible(chainMenuButton);
        
//Info Button
        infoButton.setButtonText("Info");
        infoButton.setClickingTogglesState(true);
        infoButton.onClick = [this, &ic](){
            ic.setEnabled(infoButton.getToggleState());
            ic.setVisible(infoButton.getToggleState());

        };
        infoButton.setEnabled(true);
        infoButton.setVisible(true);
        addAndMakeVisible(infoButton);
        

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
        
        juce::Font boldFont ("Futura", 35.0f, juce::Font::bold);

        g.setFont (boldFont);
        
        switch(hs.getCurrentlyShowingComponent())
        {
            case 0:
                g.drawText ("Home", getLocalBounds(),
                                juce::Justification::centred, true);
                break;
            case 'F': //Filter
                g.drawText ("Filter", getLocalBounds(),
                            juce::Justification::centred, true);
                break;
            case 'D': //Delay
                g.drawText ("Delay", getLocalBounds(),
                            juce::Justification::centred, true);
                break;
            case 'C': //Convolution Reverb
                g.drawText ("Convolution Reverb", getLocalBounds(),
                            juce::Justification::centred, true);
                break;
            case 'X': //Delay Xpanse
                g.drawText ("Delay-Xpanse", getLocalBounds(),
                            juce::Justification::centred, true);
                break;
            case 'G': //Degrade
                g.drawText ("Degrade", getLocalBounds(),
                            juce::Justification::centred, true);
                break;
            case 'P': //Phaser
                g.drawText ("Phaser", getLocalBounds(),
                            juce::Justification::centred, true);
                break;
            case 'R': //Reverb
                g.drawText ("Reverb", getLocalBounds(),
                            juce::Justification::centred, true);
                break;
            case 'Z': //Distortion
                g.drawText ("Distortion", getLocalBounds(),
                            juce::Justification::centred, true);
                break;
            default:
                break;
        }
        
        this->repaint();
        
    }

    void resized() override
    {
        //Setting bounds of components
        
        gainSlider.setBounds(getLocalBounds().getWidth() * 3/4, getLocalBounds().getHeight()/4, getLocalBounds().getWidth() * 1/4, getLocalBounds().getHeight()/2);
        gainLabel.attachToComponent(&gainSlider, true);
        
        auto buttonWidth = 100;
        auto buttonHeight = 50;
        homeButton.setBounds(10, getLocalBounds().getHeight()/6, buttonWidth, buttonHeight);
        chainMenuButton.setBounds(10 + buttonWidth + 10, getLocalBounds().getHeight()/6, buttonWidth, buttonHeight);
        infoButton.setBounds(10 + buttonWidth + 10 + buttonWidth + 10, getLocalBounds().getHeight()/6, buttonWidth, buttonHeight);
        
    }

private:
    HomeScreenComponent& hs;
    ProcessingChainSelectionComponent& pc;
    InfoComponent& ic;
    
    juce::Slider gainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>gainAttachment;
    juce::Label gainLabel;
    
    juce::TextButton homeButton;
    juce::TextButton chainMenuButton;
    juce::TextButton infoButton;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlobalControlsComponent)
};
