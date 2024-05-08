### LFO's
As an extra part of my project I wanted to look into creating some custom LFO's beyond the usual sine/saw/square waves. I thought using natural sounds like birdsong, waves crashing, wind rustling the trees as LFO's themselves could potentially make some effects, such as reverb, sound more convincing. Adding an extra more natural modulation to them that they may experience in the real world. Unfortunately I didnt get the time to explore this idea as much as I wanted to, but this folder contains one example of this.
I recorded birdsong and created an envelope follower in Python to follow the shape of the sound, resulting in a long yet different type of LFO.

* "Birds.wav" Is the original file I wanted to create an LFO from
* "EnvelopeFollowing.ipynb" Contains the envelope following code I created using a moving average filter.
* "Birds_LFO.wav" Is the resulting LFO file I used in my plug-in
