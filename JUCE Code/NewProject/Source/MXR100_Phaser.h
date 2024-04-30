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

class MXR100_Phaser{
public:
    MXR100_Phaser(juce::AudioProcessorValueTreeState& vts): Phaser_Parameters(vts)
    {
        
    };
    
    ~MXR100_Phaser(){};
    
    void prepareToPlay(double sampleRate, int samplesPerBlock)
    {
        this->fs = sampleRate;
        calculateVariance(intensity);
        tempX[0] = 0;
        tempX[1] = 0;
        
        for(int i = 0; i < numAllpasses; ++i)
        {
            tempY[0][i] = 0;
            tempY[1][i] = 0;
        }
    };
    
    void processBlock(juce::AudioBuffer<float>& buffer)
    {
        auto leftChannel = buffer.getReadPointer(0);
        auto rightChannel = buffer.getReadPointer(1);
        
        float* leftChannelOut = buffer.getWritePointer(0);
        float* rightChannelOut = buffer.getWritePointer(1);
        
        
        for(int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto currentLFOval = calculateLFOVal();
            //auto currentLFOval = 0.2;
            
            auto sampL = leftChannel[sample];
            auto sampR = rightChannel[sample];
            
            auto wetL = sampL;
            auto wetR = sampR;
            
            
//            for(int i = 0; i < numAllpasses; ++i)
//            {
//                auto a1 = calculateCoefficient(currentLFOval, i);
//
//                auto L = (a1 * wetL) + tempX[0] - (a1 * tempY[0][i]);
//                auto R = (a1 * wetR) + tempX[1] - (a1 * tempY[1][i]);
//
//                //L = std::max(-1.0f, std::min(1.0f, L));
//                //R = std::max(-1.0f, std::min(1.0f, R));
//
//                tempX[0] = L;
//                tempX[1] = R;
//                tempY[0][i] = L;
//                tempY[1][i] = R;
//                wetL = L;
//                wetR = R;
//            }

            
//            auto a1 = calculateCoefficient(currentLFOval, 0);
//
//            auto L = (a1 * wetL) + tempX[0] - (a1 * tempY[0][0]);
//            auto R = (a1 * wetR) + tempX[1] - (a1 * tempY[1][0]);
//
//            //L = std::max(-1.0f, std::min(1.0f, L));
//            //R = std::max(-1.0f, std::min(1.0f, R));
//
//            tempX[0] = tempY[0][0];
//            tempX[1] = tempY[1][0];
//            tempY[0][0] = L;
//            tempY[1][0] = R;
//            wetL = L;
//            wetR = R;
            
//            a1 = calculateCoefficient(currentLFOval, 1);
//
//            L = (a1 * wetL) + tempX[0] - (a1 * tempY[0][1]);
//            R = (a1 * wetR) + tempX[1] - (a1 * tempY[1][1]);
//
//            //L = std::max(-1.0f, std::min(1.0f, L));
//            //R = std::max(-1.0f, std::min(1.0f, R));
//
//            tempX[0] = L;
//            tempX[1] = R;
//            tempY[0][1] = L;
//            tempY[1][1] = R;
//            wetL = L;
//            wetR = R;
//
//            a1 = calculateCoefficient(currentLFOval, 2);
//
//            L = (a1 * wetL) + tempX[0] - (a1 * tempY[0][2]);
//            R = (a1 * wetR) + tempX[1] - (a1 * tempY[1][2]);
//
//            //L = std::max(-1.0f, std::min(1.0f, L));
//            //R = std::max(-1.0f, std::min(1.0f, R));
//
//            tempX[0] = L;
//            tempX[1] = R;
//            tempY[0][2] = L;
//            tempY[1][2] = R;
//            wetL = L;
//            wetR = R;
//
//            a1 = calculateCoefficient(currentLFOval, 3);
//
//            L = (a1 * wetL) + tempX[0] - (a1 * tempY[0][3]);
//            R = (a1 * wetR) + tempX[1] - (a1 * tempY[1][3]);
//
//            //L = std::max(-1.0f, std::min(1.0f, L));
//            //R = std::max(-1.0f, std::min(1.0f, R));
//
//            tempX[0] = L;
//            tempX[1] = R;
//            tempY[0][3] = L;
//            tempY[1][3] = R;
//            wetL = L;
//            wetR = R;
            
            


            //APF 1
            auto a1 = calculateCoefficient(currentLFOval, 0);
            y_1_1L= y_1L;
            y_1L = (a1 * wetL) + tempL - (a1 * y_1_1L);
            
            y_1_1R= y_1R;
            y_1R = (a1 * wetR) + tempR - (a1 * y_1_1R);

            //APF 2
            a1 = calculateCoefficient(currentLFOval, 1);
            y_2_1L = y_2L;
            y_2L = (a1 * y_1L) + y_1_1L - (a1 * y_2_1L);
            
            y_2_1R = y_2R;
            y_2R = (a1 * y_1R) + y_1_1R - (a1 * y_2_1R);

            //APF 3
            a1 = calculateCoefficient(currentLFOval, 2);
            y_3_1L = y_3L;
            y_3L = (a1 * y_2L) + y_2_1L - (a1 * y_3_1L);
            
            y_3_1R = y_3R;
            y_3R = (a1 * y_2R) + y_2_1R - (a1 * y_3_1R);

            //APF 4
            a1 = calculateCoefficient(currentLFOval, 3);
            y_4_1L = y_4L;
            y_4L = (a1 * y_3L) + y_3_1L - (a1 * y_4_1L);
            
            y_4_1R = y_4R;
            y_4R = (a1 * y_3R) + y_3_1R - (a1 * y_4_1R);

            //APF 5
            a1 = calculateCoefficient(currentLFOval, 4);
            y_5_1L = y_5L;
            y_5L = (a1 * y_4L) + y_4_1L - (a1 * y_5_1L);
            
            y_5_1R = y_5R;
            y_5R = (a1 * y_4R) + y_4_1R - (a1 * y_5_1R);

            //APF 6
            a1 = calculateCoefficient(currentLFOval, 5);
            y_6_1L = y_6L;
            y_6L = (a1 * y_5L) + y_5_1L - (a1 * y_6_1L);
            
            y_6_1R = y_6R;
            y_6R = (a1 * y_5R) + y_5_1R - (a1 * y_6_1R);

            //APF 7
            a1 = calculateCoefficient(currentLFOval, 6);
            y_7_1L = y_7L;
            y_7L = (a1 * y_6L) + y_6_1L - (a1 * y_7_1L);
            
            y_7_1R = y_7R;
            y_7R = (a1 * y_6R) + y_6_1R - (a1 * y_7_1R);

            //APF 8
            a1 = calculateCoefficient(currentLFOval, 7);
            y_8_1L = y_8L;
            y_8L = (a1 * y_7L) + y_7_1L - (a1 * y_7_1L);
            
            y_8_1R = y_8R;
            y_8R = (a1 * y_7R) + y_7_1R - (a1 * y_7_1R);

            wetL = y_8L;
            wetR = y_8R;
            

            leftChannelOut[sample] = wetL + sampL;
            rightChannelOut[sample] = wetR + sampR;
            tempL = sampL;
            tempR = sampR;
        }
    };

    float calculateLFOVal()
    {
        auto t = k/fs;
        k++;
        float val = (amplitude * sin(2 * M_PI * speed * t));
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
    float speed = 0.5;
    float amplitude = 3;
    float variance;
    int intensity = 3;
    int k = 0;
    
    const static int numAllpasses = 8;
    float tempX[2];
    float tempY[2][numAllpasses];
    
    float tempL = 0;
    float tempR = 0;
    
    //Temporary variables
    float y_1_1L = 0;
    float y_1L = 0;
    float y_2_1L = 0;
    float y_2L = 0;
    float y_3_1L = 0;
    float y_3L = 0;
    float y_4_1L = 0;
    float y_4L = 0;
    float y_5_1L = 0;
    float y_5L = 0;
    float y_6_1L = 0;
    float y_6L = 0;
    float y_7_1L = 0;
    float y_7L = 0;
    float y_8_1L = 0;
    float y_8L = 0;
    
    float y_1_1R = 0;
    float y_1R = 0;
    float y_2_1R = 0;
    float y_2R = 0;
    float y_3_1R = 0;
    float y_3R = 0;
    float y_4_1R = 0;
    float y_4R = 0;
    float y_5_1R = 0;
    float y_5R = 0;
    float y_6_1R = 0;
    float y_6R = 0;
    float y_7_1R = 0;
    float y_7R = 0;
    float y_8_1R = 0;
    float y_8R = 0;
};
