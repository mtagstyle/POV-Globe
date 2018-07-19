# PoV Globe

This repo will will be used to capture any information regarding the research, design, and implementation of a globe-shaped persistence-of-vision (PoV) display

# What is it?

A pov display uses the persistence of vision ([wiki](https://en.wikipedia.org/wiki/Persistence_of_vision)) optical illusion in order to display an image or video driven by a microcontroller. 

By moving a section of LEDs in a portion of space and turning them on at certain intervals of time, the brain will perceive that section as an entire image. Generally, this involves rotating a strip of LEDs in a circle.

Examples:
- https://www.youtube.com/watch?v=57carjNxI9A
- https://www.youtube.com/watch?v=Z_ug4ZURRfQ
- https://www.youtube.com/watch?v=970p4y_iFDA

# Motivations
- Potential EiE project that can be interfaced with the engenuics development board
- Beakerhead display
- Honing technical skills (PCB Design, Mechanical Design, System Design, Firmware Development, Power Electronics)

# Goal(s)
* Milestone 1: Proof-of-concept prototype
  * A base station that is able to rotate a rotating component
  * A rotating component that can display a "hello world" type of example
  
* Milestone 2: Final Design
  * Improve any limitations discovered in milestone 1
  * Have an actual formalized design based offf of the prototype
  * Add in any functionality that may be missing from milestone 1
  
* Milestone 3: Adding new features
  * Improve any limitations discovered in milestone 2
  * Brainstorm
  * IoT integration: Support for Wifi, Bluetooth, ANT?
  * Supply an API that may send image data to the POV display  
  

# Design Overview

# Implementation Overview

### Folder Structure

- firmware: Contains all of the source code
- hardware: Contains all of the hardware design
- resource: A list of resources used for research 
- documents: Contains all design documents
- media: Contains journal entries and media
