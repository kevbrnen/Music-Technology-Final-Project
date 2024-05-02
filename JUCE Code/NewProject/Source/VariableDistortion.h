/*
  ==============================================================================

    VariableDistortion.h
    Created: 2 May 2024 4:47:05pm
    Author:  Kevin Brennan
 
    A class for a variable distortion
    Allows users to select different types of distortion
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class VariableDistortion{
public:
    VariableDistortion(juce::AudioProcessorValueTreeState& vts): Distortion_Parameters(vts)
    {
        gain = Distortion_Parameters.getRawParameterValue("distortion_pregain");
        Type = Distortion_Parameters.getRawParameterValue("distortion_type");
        Thresh = Distortion_Parameters.getRawParameterValue("distortion_thresh");
    };
    
    ~VariableDistortion(){};
    
    //Pre-playback initialisation
    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {

    };
    
    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            for(int channel = 0; channel < buffer.getNumChannels(); ++channel)
            {
                auto* inData = buffer.getReadPointer(channel);
                auto* outData = buffer.getWritePointer(channel);
                
                
                if(Type->load() == 0) //Tanh
                {
                    outData[sample] = processTanh(gain->load(), inData[sample]);
                }
                else if(Type->load() == 1) //Cubic Soft
                {
                    outData[sample] = processCubic(gain->load(), inData[sample], Thresh->load());
                }
                else if(Type->load() == 2) //Square
                {
                    outData[sample] = processSquare(gain->load(), inData[sample], Thresh->load());
                }
                else if(Type->load() == 3) //Clip
                {
                    outData[sample] = processClipping(gain->load(), inData[sample], Thresh->load());
                }
                else if(Type->load() == 4) //Soft Saturation
                {
                    outData[sample] = processSoftSaturation(gain->load(), inData[sample], Thresh->load());
                }
            }
        }

    };
    
    float processTanh(float gain, float samp)
    {
        return std::tanh(gain * samp);
    }
    
    float processCubic(float gain, float samp, float thresh)
    {

        float temp = 0;
        samp *= gain;
        
        if(samp <= -(thresh))
        {
            temp = -1;
        }
        else if((-(thresh) <= samp) && (samp <= (thresh)))
        {
            temp = samp;
        }
        else if(samp >= (thresh))
        {
            temp = 1;
        }

        return temp;
    }
    
    float processSquare(float gain, float samp, float thresh)
    {
        float temp = 0;
        samp *= gain;
        
        if(samp < (thresh))
        {
            temp = -0.9;
        }
        else if(samp > (thresh))
        {
            temp = 0.9;
        }
        
        return temp;
    }
    
    float processClipping(float gain, float samp, float thresh)
    {
        float temp = 0;
        samp *= gain;
        
        if(samp <= -(thresh))
        {
            temp = -(2/3);
        }
        else if((-(thresh)<= samp) && (samp <= (thresh)))
        {
            temp = samp - (std::pow(samp, 3)/3);
        }
        else if(samp >= (thresh))
        {
            temp = 2/3;
        }

        return temp;
    }
    
    float processSoftSaturation(float gain, float samp, float thresh) //DAFX
    {
        float temp = 0;
        
        if(abs(samp) <= (thresh))
        {
                temp = 2*(samp * gain);
        }
        else if((thresh) <= abs(samp))
        {
            if(samp>0)
            {
                temp = (3 - pow((2 - 3*samp), 2))/3;
            }
            else
            {
                temp = -((3 - pow((2 -  3*samp), 2))/3);
            }
        }
        else if(abs(samp) >= (2 * thresh))
        {
            if(samp>0)
            {
                temp = 0.9;
            }
            else
            {
                temp = -0.9;
            }
        }

        return temp;
    }
        
    juce::AudioProcessorValueTreeState& Distortion_Parameters;
private:
    std::atomic<float>* gain = nullptr;
    std::atomic<float>* Type = nullptr;
    std::atomic<float>* Thresh = nullptr;
};

