"""CircularBuffer.py: Circular Buffer For Audio Processing"""
__author__ = "Kevin Brennan"
__date__ = "19th Feb 2024"

import numpy as np

class CircularBuffer:
    """A Circular Buffer For Audio Processing"""

    # Initializes a buffer of size "BufferSize" filled with zeros.
    # "bufferIndex" used to keep track of current position and with
    # "wrapping around" when index reaches end of buffer.
    # Sets the buffer length to 1+N where N is the integer part of the delay, 
    # also stores the fractional part of the delay for interpolation later
    def __init__(self, buffer_size: float):
        """Initializes a buffer of a given size.
        If using as delay, enter delay length as buffer_size in samples"""
        if buffer_size <= 0:
            raise ValueError("Buffer size must be a positive integer.")
        
        self.delay_int = int(buffer_size)
        self.delay_frac = buffer_size - self.delay_int

        self.buffer_size = self.delay_int + 1
        self.buffer = np.zeros(int(self.buffer_size), dtype= float)
        self.buffer_index = 0

    # Returns the current sample from the buffer. Implements fractional delay by linearly interpolating between the current sample 
    # to be taken and the next sample. Weighted using the fractional delay amount
    def get_sample_from_buffer(self) -> float:
        """Returns the current sample from the buffer."""

        next_index = (self.buffer_index + 1) % self.buffer_size

        if self.buffer_index < self.buffer_size:

            # Linearly interpolate between samples for fractional delay
            delayed_sample = ((1 - self.delay_frac) * self.buffer[self.buffer_index]) + (self.delay_frac * self.buffer[next_index])

            return delayed_sample
        else:
            raise IndexError("Buffer is empty.")
    
    # Places the input sample into the buffer at "bufferIndex".
    # If the current sample has not been pulled from the buffer
    # this will overwrite that sample, so this function should only
    # be called after the current sample has been retrieved.
    def push_sample_to_buffer(self, input: float) -> None:
        """Places the input sample into the buffer."""
        self.buffer[self.buffer_index] = input
        self.buffer_index = (self.buffer_index + 1) % self.buffer_size
         
    # Self Explanatory
    def print_buffer(self):
        """For testing, prints the whole buffer at this point in time"""
        print(self.buffer)

    #For getting the size of the buffer
    def buffer_len(self):
        """Returns the length of the buffer"""
        return self.buffer_size



class StereoDelay:
    """Uses circular buffer class but extends it to deal with stereo audio. Allows
    for different delay lengths for each channel"""
    
    # Initialization. Sets up two circular buffers, one for each channel.
    # Allows for individual buffer length control for each channel.
    def __init__(self, bufferSizeL: float, bufferSizeR: float):
        """Individual Buffer length control. Initializes two buffers of a given size, one for each channel.
        If using as delay, enter delay length as buffer_size in samples"""

        if ((bufferSizeL <= 0)|(bufferSizeR <= 0)):
            raise ValueError("Buffer size must be a positive integer.")
        
        # Create one circular buffer for each channel
        self.Lbuffer = CircularBuffer(bufferSizeL)
        self.Rbuffer = CircularBuffer(bufferSizeR)

    # Returns the current samples from the buffers in stereo array [Left, Right]
    def get_samples_from_buffer(self) -> float:
        """Returns current sample from each buffer"""

        L = self.Lbuffer.get_sample_from_buffer()
        R = self.Rbuffer.get_sample_from_buffer()

        return [L, R]
    

    # Places input samples in their respective buffers.
    # input must be stereo.
    def push_samples_to_buffer(self, input):
        """Places the input samples to their respective buffers"""
        if not isinstance(input, np.ndarray) or (input.shape != (2,)):
            raise ValueError("Input must be stereo")

        self.Lbuffer.push_sample_to_buffer(input[0])
        self.Rbuffer.push_sample_to_buffer(input[1])

    # Returns the length of the longest buffer
    # useful for creating an output array for offline 
    # audio processing
    def buffer_maxLen(self):
        """Returns the longest buffer length"""

        return max(self.Lbuffer.buffer_len(), self.Rbuffer.buffer_len())

