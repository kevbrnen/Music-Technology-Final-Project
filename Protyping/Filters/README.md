# Filters

[W3 EQ Cookbook](https://www.w3.org/TR/audio-eq-cookbook/) - Mostly used for getting filter coefficients

### [Low Pass](./LowPass.ipynb) -
LowPass.ipynb explores different methods of implementing a low pass filter. 
    From difference equation, transfer function and using built in scipy.signal functions. 
    I looked at the differences in using butterworth filters and chebyshev filters.
    I looked at the differences in orders of filters and steepness of the transition.


### [High Pass](./HighPass.ipynb) -
HighPass.ipynb looks at methods for implementing a high pass filter. I didn't do as much 
    experimentation here as it is almost the exact same as a low pass.
    I also looked at the differences in the orders of filters and the steepness of the transition.


### [Band Pass](./BandPass.ipynb) - 
BandPass.ipynb looks at implementations of band pass filters.
    Both W3 implementation and scipy butterworth function.
    I tested the differences of using bandwidth and Q controls.
    The bandwidth control keeps the maximum magnitude of the filter at 1 (at the cutoff) and
    sets the sidebands to 0 outside of the bandwidth.
    The Q control makes the bandwidth narrower with higher Q values, but sets the gain
    of the peak to the value of Q, and everything else to 1.

  Bandwidth control results in a quieter filtered signal while Q results in a louder signal.

  Bandwidth may result in more accurate filtering if that is what is needed, as Q will result
    in more sideband frequencies being made louder


### [Notch](./Notch.ipynb) - 
Notch.ipynb looks at notch/bandstop filter implementations.
    I tested the differences between bandwidth and Q, similar to BansPass.ipynb
    but couldnt find much of a difference.

  I tested the butterworth bandstop filter. I found it to be more accurate for lower orders 
    compared to the bandwidth and Q implementations, but not much other difference.


### [All Pass](./AllPass.ipynb) - 
AllPass.ipynb looks at first and second order allpass implementations
    from equations.

  Allpass filters dont necessarily do anything on their own, which is why the magnitude 
    response is flat. They do however introduce a phase shift of different amounts at different
    frequencies. This phase shift, as well as the group delay of the filter can be used for some
    interesting outcomes.


### [Shelving Filters](./Shelves.ipynb):
Shelves.ipynb looks at low and high shelf implementations.
    The Q control allows you to change the steepness of the transition, as well
    as introducing some resonance around the cutoff frequency.
    There is also control over amplitude (dB gain) of frequencies below the cutoff
    frequency in the case of a low shelf, and the frequencies above the cutoff
    frequency in the case of a high shelf.
    
