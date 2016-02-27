# Microfluidics

Tiny droplets of water manipulated and controlled by a computer.

Microfluidics is a branch of engineering that deals with the manipulation of tiny drops of liquid.  It has many applications in biology and chemistry -- test tubes are way too big and bulky for modern batch processes!  While at Bell Labs, I and collaborator Tom Krupenkin developed a device to control the movement of tiny water droplets using electrostatic forces. 

In the enclosed video, a microscope shows one such droplet moving across the surface of an electrode array.  The droplet is a few mm in size.   During the demonstration, I am controlling the movement of the droplet with a mouse and GUI software (not shown in the video).


The droplet control software is written in C++.  It uses a GUI system to generate a display that mimics the appearance of the electrode array under the microscope.  In response to user input with the mouse, the software delivers control voltages to the device that cause the droplet to move in real time.  


