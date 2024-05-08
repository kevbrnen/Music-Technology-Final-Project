
## Recorded Responses folder - 
- "Parish Hall" Contains some raw, untrimmed impulse response recordings created using the interrupted noise and sine sweep stimulus "IR_measurement_2022_stimulus_final.wav".
- "REW Responses" Contains the Room EQ Wizard impulse response measurement files I used to record responses for use in the Convolution Reverb Effect, using the Umik-1 acoustical measurement microphone.


## [ExtractingResponses.ipynb](/ExtractingResponses.ipynb) - 
ExtractingResponses.ipynb shows an example of how impulse responses can be extracted using different methods; 

- Claps/Balloon pops: where the sound itself is the impulse.

- Interrupted noise: where the decay of the noise after it stops is the impulse.

- Sine sweep: where we use inverse convolution with the original sweep to obtain the impulse.
