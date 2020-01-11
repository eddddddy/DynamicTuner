# Dynamic Tuner
C++ script that determines frequencies of all pitches in a given musical score based on maximizing the number of ideal ratios of intervals based on Pythagorean harmony.

Note that this implies that the same note might get mapped to different frequencies at different places, depending on which notes immediately surround it.
## Compiling
`make fixed` will compile the files into a statically-linkable library. Include any necessary header files in the project.
## Using realtime dynamic tuning
Sample classes Input, Controller, and Receiver are provided and can be overriden (most likely only the Input and the Receiver should be overriden). To demo the dynamic tuning on a Windows machine with OpenAL installed, compile the provided classes and link with the library, then start the input controller.
