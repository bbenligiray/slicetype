## SliceType: Fast Gaze Typing with a Merging Keyboard

The soft keyboard designed for gaze typing proposed in the following paper:

[Benligiray, B.; Topal, C.; Akinlar, C., "SliceType: Fast gaze typing with a merging keyboard," Journal on Multimodal Interfaces, 2018.](https://arxiv.org/abs/1706.02499)

The .zip file contains the soft keyboard built for Windows.
I also uploaded the source code to refer to it myself.
I don't provide a makefile at the moment, but it only needs freeglut to build.
It uses Win32 API, so it will take some work to port.

#### What is this?

* SliceType is a soft keyboard designed for fast and comfortable gaze typing.
* Our main objective in designing SliceType was to use the screen are efficiently. We achieved this by determining the keys that will not be used for the next input, and allocating their space to adjacent keys with a merging animation.
* As a result of the key merging functionality, the target keys are more likely to be larger. Large keys are fast to navigate towards, and easy to dwell on even, when there is eye tracking jitter.
* In addition, Slicetype uses a word completion proposal and selection scheme that is ideal for gaze typing by dwelling.
* The advantages of the features above are quantified using the Fitts' law.
* SliceType is compared with two other gaze typing keyboards, Dasher and GazeTalk. The user experiment done with 37 novice users showed that, SliceType provided the highest text entry rate, and is preferred over the other keyboards.

<p align="center">
  <img src="https://user-images.githubusercontent.com/19530665/27763730-ee36c938-5e91-11e7-885a-fe518de4ec5d.png"/>
</p>

#### Videos

[SliceType: Fast Gaze Typing with a Merging Keyboard](https://www.youtube.com/watch?v=3IM0-utYrDg)

[SliceType - Advanced User Performance](https://www.youtube.com/watch?v=um75h3hHcwo)

[Slicetype - Gaze Typing Example (Turkish)](https://www.youtube.com/watch?v=WlAcx398p-A)

