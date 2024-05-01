/*
  ==============================================================================

    MXR100_Phaser.h
    Created: 30 Apr 2024 11:46:09am
    Author:  Kevin Brennan
 
    R. Kiiski, F. Esqueda, and V. Välimäki, “Time-variant gray-box mod-
    eling of a phaser pedal,” in Proc. Int. Conf. Digital Audio Effects
    (DAFx-16), Brno, Czech Republic, Sep. 2016.
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <cmath>
#include "SmoothingFilter.h"

class MXR100_Phaser{
public:
    MXR100_Phaser(juce::AudioProcessorValueTreeState& vts): Phaser_Parameters(vts), smoother(10)
    {
        speed = Phaser_Parameters.getRawParameterValue("phaser_lfo_speed");
        intensity = Phaser_Parameters.getRawParameterValue("phaser_intensity");
        wet_dry = Phaser_Parameters.getRawParameterValue("phaser_wet_dry");
        
    };
    
    ~MXR100_Phaser(){};
    
    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        this->fs = sampleRate;
        calculateVariance(intensity->load());
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        auto leftChannel = buffer.getReadPointer(0);
        auto rightChannel = buffer.getReadPointer(1);
        
        float* leftChannelOut = buffer.getWritePointer(0);
        float* rightChannelOut = buffer.getWritePointer(1);
        
        
        for(int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto newSpeed = speed->load();
            
            if(newSpeed != lastSpeed)
            {
                newSpeed = smoother.process(newSpeed);
                
                this->currentSpeed = newSpeed;
            }
            
            auto currentLFOval = calculateLFOVal();
            
            auto sampL = leftChannel[sample];
            auto sampR = rightChannel[sample];
            
            auto wetL = sampL;
            auto wetR = sampR;
            
            float tempL = 0.f;
            float tempR = 0.f;
            
            auto wd_amt = wet_dry->load();
            
            for (int i = 0; i < 10; ++i) {
                auto a1 = calculateCoefficient(currentLFOval, i);
                
                y_L = wd_amt * ((a1 * wetL) + tempL - (a1 * y_1L[i]));
                tempL = y_1L[i];
                y_1L[i] = y_L;
                
                y_R = wd_amt * ((a1 * wetR) + tempR - (a1 * y_1R[i]));
                tempR = y_1R[i];
                y_1R[i] = y_R;
                
                wetL = y_L;
                wetR = y_R;
            }
            
            leftChannelOut[sample] = wetL + sampL;
            rightChannelOut[sample] = wetR + sampR;
        }
    };

    float calculateLFOVal()
    {
        auto t = k/fs;
        k++;
        float val = (amplitude * sin(2 * M_PI * currentSpeed * t));
        val = std::max(-1.0f, std::min(1.0f, val));
        return val;
        
    };
    
    float calculateCoefficient(float lfoVal, int i)
    {
        float a1 = (std::tan(M_PI * ((centerFrequency + i*variance) + (lfoVal * variance)) / fs) - 1) / (std::tan(M_PI * ((centerFrequency + i*variance) + (lfoVal * variance)) / fs) + 1);
            
        return a1;
    };
    
    void calculateVariance(int intensity)
    {
        switch(intensity)
        {
            case 1:
                this->variance = 250;
                break;
            case 2:
                this->variance = 500;
                break;
            case 3:
                this->variance = 750;
                break;
            case 4:
                this->variance = 1000;
                break;
            default:
                this->variance = 100;
                break;
        }
    };
    
    juce::AudioProcessorValueTreeState& Phaser_Parameters;
    
private:
    juce::dsp::ProcessSpec Spec;
    
    float fs;
    float centerFrequency = 750;
    
    std::atomic<float>* speed = nullptr;
    float currentSpeed = 0.5;
    float lastSpeed;
    
    float amplitude = 3;
    float variance;
    std::atomic<float>* intensity = nullptr;
    int lastIntensity;
    int k = 0;
    
    std::atomic<float>* wet_dry = nullptr;
    
    const static int numAllpasses = 8;
    float tempX[2];
    float tempY[2][numAllpasses];
    
    float y_L = 0.f;
    float y_R = 0.f;
    float y_1L[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float y_1R[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    SmoothingFilter smoother;
    
    //Temporary variables
//    float y_1_1L = 0;
//    float y_1L = 0;
//    float y_2_1L = 0;
//    float y_2L = 0;
//    float y_3_1L = 0;
//    float y_3L = 0;
//    float y_4_1L = 0;
//    float y_4L = 0;
//    float y_5_1L = 0;
//    float y_5L = 0;
//    float y_6_1L = 0;
//    float y_6L = 0;
//    float y_7_1L = 0;
//    float y_7L = 0;
//    float y_8_1L = 0;
//    float y_8L = 0;
//
//    float y_1_1R = 0;
//    float y_1R = 0;
//    float y_2_1R = 0;
//    float y_2R = 0;
//    float y_3_1R = 0;
//    float y_3R = 0;
//    float y_4_1R = 0;
//    float y_4R = 0;
//    float y_5_1R = 0;
//    float y_5R = 0;
//    float y_6_1R = 0;
//    float y_6R = 0;
//    float y_7_1R = 0;
//    float y_7R = 0;
//    float y_8_1R = 0;
//    float y_8R = 0;
};
