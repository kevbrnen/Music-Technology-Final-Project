"""CircularBuffer.py: Circular Buffer For Audio Processing"""
__author__ = "Kevin Brennan"
__date__ = "19th Feb 2024"

import numpy as np

class CircularBuffer:
    """A Circular Buffer For Audio Processing"""

    # Initializes a buffer of size "BufferSize" filled with zeros.
    # "bufferIndex" used to keep track of current position and with
    # "wrapping around" when index reaches end of buffer.
    def __init__(self, buffer_size: int):
        """Initializes a buffer of a given size.
        If using as delay, enter delay length as buffer_size in samples"""
        if buffer_size <= 0:
            raise ValueError("Buffer size must be a positive integer.")
        
        self.buffer_size = buffer_size
        self.buffer = np.zeros(buffer_size, dtype= float)
        self.buffer_index = 0

    # Returns the current sample from the buffer.
    def get_sample_from_buffer(self) -> float:
        """Returns the current sample from the buffer."""
        if self.buffer_index < self.buffer_size:
            return self.buffer[self.buffer_index]
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
        print(self.buffer)


    #### Implement Fractional Delay and make stereo ####