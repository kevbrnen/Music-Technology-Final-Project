# Reverbs

# [Convolution Reverb](./Convolution%20Reverb/)- 
[ConvolutionReverb_Balloon_pop.ipynb](./Convolution%20Reverb/ConvolutionReverb_Balloon_pop.ipynb) uses a balloon pop recorded in a hall as an impulse for convolution reverb. This implementation applies it with both time based and fft based convolution. 

CombFilterReverb - 
    CombFilterReverb.ipynb gives a comb filter implementation (Feedback delay), plotting of
    the response of the system, and a comb filter reverberator implementation.

DelayingAllpassReverb - 
    Based off the impulse response of an all pass filter. 
    DelayingAllpassReverb.ipynb gives an implementation of a delaying allpass filter.

FDN_Reverb - 
    Gives an FDN reverb implementation.
    Allows you to control gains, delays and feedback matrix coefficients.
    Work in progress.

LPFCombReverberator - 
    Similar implementation to the comb filter reverb except this implementation has a low pass
    filter in the feedback path to resemble how higher frequencies react in a real space, decaying
    faster than lower frequencies

SchroederReverb - 
    A parallel comb filter reverberator bank with two allpass filter reverberators in series.
    The comb filters produce a series of echoes while the allpass filters multiply and decay the echoes,
    to give us an impulse response with early reflections and late reverberations.

MoorersReverb - 
    Similar implementation to Schroeders reverb except it uses low pass comb reverberator in parallel 
    with one allpass reverberator
