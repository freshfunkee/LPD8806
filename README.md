# LPD8806
C++ library used for sending colour patterns to LPD8806 digital RGB LED strip from a Raspberry Pi

This code was used to write lighting patterns for stage lights. A switch was used to toggle patterns throughout the program.
This was a personal program I was using while playing gigs. It is by no means perfect, optimised or commented well, as I never intended on sharing the code. But, it could be of some use for anyone trying to run a C++ application using the gpio ports on a Raspberry Pi

LPD8806.cpp:
Contains strip setup and LED animation functions

Colour.cpp:
Contains various classes for different methods of defining colours (RGB, HSV, etc.)

main.cpp:
Example program showing how to send and update animations. Uses GPIO 4 as a switch to toggle animations
