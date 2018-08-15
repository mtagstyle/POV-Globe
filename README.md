# Project Shiba

This repo will will be used to capture any information regarding the research, design, and implementation of a persistence-of-vision (PoV) display

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
* Milestone 1: System Design
  * Requirements created for overall system (1.5 Weeks)
  * System design documents created: Basic block diagram (2 weeks)
  * External Interfaces (0.5 weeks) (Logical Level)
  
* Milestone 2: Prototype Power Delivery
  * Power Delivery Solution
  * Design documents created
  * Prototype created
  
* Milestone 3: Mechanical Fixture Creation, Dynamics
  * Safety concerns noted
  * Mechanical drawings of how components are mounted
  * How components will move dynamically 

* Milestone 4: Rotating Component - Hardware
  * MCU selection, sensor selection, LED driver circuitry, Power Receiver selection, Radio ???
  * Design documents created
  * Prototype created
  
*  Milestone 5: Rotating Component - Software
  * Software Architecture
  * Software Implementation
  * Software Interfaces

# Design Overview

### The System

Shown above is the block diagram showing all of the components involved in the system. The following are the responsibilities of each components.

Component  | Responsibilities
---------- | -------------
Stationary Circuit  | This PCB is responsible for taking power from a wall-power supply. It is mounted on the stationary component of the display and primarily delivers power to the rotating component.
Rotating Circuit  | This PCB has 2 main responsibilities: The first responsibility is to take power from the stationary PCB and convert it to what it needs. The second responsibility is to hostthe micro controller and sensors which are used to drive the LED strips at certain intervals of time.
Mechanical Fixture | This component represents the fixture used to hold all of the circuit boards and the mechanics/interactions between the stationary and rotating component.
Firmware (Drivers) | This component is the code that is used to address/drive the segment of LEDs as well as detecting at what time intervals to turn them on/off.
Firmware (Applications/Middleware) | This component represents the design of the operating environment that the host environment will run on, how the drivers will interact with the application program, as well as the design of the interfaces that will be exposed to the outside world.



# Implementation Overview

### Folder Structure

- firmware: Contains all of the source code
- hardware: Contains all of the hardware design
- resource: A list of resources used for research 
- documents: Contains all design documents
- media: Contains journal entries and media
