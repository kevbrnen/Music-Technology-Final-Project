/*
  ==============================================================================

    SmoothingFilter.h
    Created: 5 Apr 2024 2:46:54pm
    Author:  Kevin Brennan
 
    A very simple one pole low pass filter to try and smooth out artefacts from
    changing values.
  ==============================================================================
*/

#pragma once
#include <math.h>

class SmoothingFilter
{
public:
    SmoothingFilter(){a0 = 1.0; b1 = 0.0; z1 = 0.0;};
    SmoothingFilter(double Fc) {z1 = 0.0; setFc(Fc);};
    ~SmoothingFilter(){};
    
    //Sets the cutoff frequency and recalculates the coefficients
    void setFc(float newFc)
    {
        b1 = exp(-2.0 * M_PI * newFc);
        a0 = 1.0 - b1;
        
        cutoff = newFc;
    };
    
    //Processes the input sample
    float process(float sample)
    {
        return z1 = sample * a0 + z1 * b1;
    };
    
private:
    float cutoff;
    double a0, b1, z1;
};
