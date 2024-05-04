/*
  ==============================================================================

    MXR100_Phaser.h
    Created: 30 Apr 2024 11:46:09am
    Author:  Kevin Brennan
 
    A grey box model of the MXR100 Phaser pedal
 
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
        q_val = Phaser_Parameters.getRawParameterValue("phaser_q");
        mod_width = Phaser_Parameters.getRawParameterValue("phaser_mod_width");
        fdbkAmt = Phaser_Parameters.getRawParameterValue("phaser_fdbk");
        
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
            
            auto fdbk = fdbkAmt->load();
            
            auto wetL = (fdbkL * fdbk) + (sampL * (1-fdbk));
            auto wetR = (fdbkR * fdbk) + (sampR * (1-fdbk));

            auto wd_amt = wet_dry->load();

            
            for (int i = 0; i < 10; ++i)
            {
                calculateCoefficient(currentLFOval, i);
                calculateVariance(intensity->load());
                
                auto wL = wetL - (this->c1 * d1L[i]) - (this->c2 * d2L[i]);
                auto yL = (wL * this->c2) + (this->c1 * d1L[i]) + d2L[i];
                d2L[i] = d1L[i];
                d1L[i] = wL;
                wetL = yL;
                
                auto wR = wetR - (this->c1 * d1R[i]) - (this->c2 * d2R[i]);
                auto yR = (wR * this->c2) + (this->c1 * d1R[i]) + d2R[i];
                d2R[i] = d1R[i];
                d1R[i] = wR;
                wetR = yR;
                
            }
            
            fdbkL = wetL;
            fdbkR = wetR;
            
            leftChannelOut[sample] = (wetL * wd_amt) + (sampL * (1-wd_amt));
            rightChannelOut[sample] = (wetR * wd_amt) + (sampR * (1-wd_amt));
        }
    };

    float calculateLFOVal()
    {
        //Spectral Music Design - Lazzarini
        auto val = ((mod_width->load())/2) * (1 + sin(this->phase));
        
        this->phase += (2 * M_PI * (this->currentSpeed/this->fs));
        
        if(this->phase >= (2*M_PI))
        {
            this->phase -= (2*M_PI);
        }
        else if(this->phase < 0)
        {
            this->phase += (2*M_PI);
        }
        
        
        return val;
    };
    
    void calculateCoefficient(float lfoVal, int i)
    {
        auto B = (centerFrequency + i*variance)/(q_val->load());
        float o = 0;
        if(i == 0)
        {
            o = (2 * M_PI * (lfoVal * (centerFrequency))/fs);
        }
        else
        {
            o = (2 * M_PI * (centerFrequency + (lfoVal * (centerFrequency + i*variance)))/fs);
        }
        
        this->c2 = std::exp((-2 * M_PI * B)/fs);
        this->c1 = -(1 + this->c2) * std::cos(o);
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
    float centerFrequency = 1000;
    
    std::atomic<float>* speed = nullptr;
    float currentSpeed = 0.5f;
    float lastSpeed;
    float phase = 0.f;
    std::atomic<float>* mod_width = nullptr;
    
    float variance;
    std::atomic<float>* intensity = nullptr;
    int lastIntensity;
    int k = 0;
    
    float fdbkL = 0;
    float fdbkR = 0;
    std::atomic<float>* fdbkAmt = nullptr;
    
    std::atomic<float>* q_val = nullptr;
    float c2;
    float c1;
    
    std::atomic<float>* wet_dry = nullptr;
    
    float d1L[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float d2L[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float d1R[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float d2R[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    SmoothingFilter smoother;
    
};
