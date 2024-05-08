### [CircularBuffer.py](./Basic%20Delays/CircularBuffer.py):
    CircularBuffer.py has two classes. 

    CircularBuffer implements a basic circular buffer and provides related 
    methods for writing to and reading from it. It implements fractional delay
    meaning the buffer size being inputted when creating the buffer can also be floats.

    StereoDelay extends the functionality of CircularBuffer. It can take and
    return stereo audio. It works by setting up two circular buffers, one for 
    each channel, with their own delay times and operating on them individually.
    Solves the problem of having to do this in the main code.

    Examples of both of these classes in use can be seen in CircularBufferTest.ipynb.

### DigitalDelay - 
    DigitalDelay.ipynb uses the circular buffer class. It is similar to the testing
    implementations, but also includes wet/dry control and feedback amount control

### SpectralDelay - 
    SpectralDelay.ipynb uses cascading allpass filters and the associated delay 
    introduced and phase response to delay different frequencies by different amounts.

    A smaller amount, say 70 filters, works better with more transient sounds like
    drums. It adds an extra punch, but doesn't change the sound too much

    You can take it to an extreme if you use hundreds or thousands of filters, and 
    get some really unique effects with less transient, smoother sounds.
