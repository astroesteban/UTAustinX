# Requirements Document

## 1. Overview

### _1.1 Objectives_

The overall objective of this project is to integrate all the individual
components taught in both _UT.6.10x_ and _UT.6.20x_ into a single system. More
specifically, the objectives of this project are: 

1) Design, test, and debug a large C program
2) Review I/O interfacing techniques used in the class
3) Design a system that performs a useful task

The useful task for this project will be implementing the classic 
**Space Invaders** game on an embedded system.

### _1.2 Process_

Similar to the other labs that were completed in this course, this project will
have a starter Keil project that has been provided by the instructors. The 
starter project contains some art and sounds to get started.

### _1.3 Roles and Responsibilities_

This project will be developed individually by myself. The clients for this
project will be other classmates and the professors of the UT.6.20x course.

### _1.4 Interactions with Existing Systems_

This game will be developed using C17, CMake, and run on a Tiva LaunchPad. This
lab will combine the solutions for Lab 8 (switched, LED), Lab 12 (Interrupts),
Lab 13 (DAC & Sounds), and Lab 14 (ADC) into one system. The project will use
an analog joystick, a switch, two LEDs, one 4-bit DAC, and the SSD1306 OLED
display.

### _1.5 Terminology_

A bitmap image file, or **BMP**, is a simple file format to store graphical
images. BMP files are device-independent, meaning the file describes how an
image will look regardless of on to which display it is rendered. A **sprite**
is a virtual entity that is created, moves around the screen, and might
disappear. A **public function** is one that can be called by another module.
For example, since the main program is allowed to call `soundPlay`, `soundPlay` 
is a public function.

## 2. Function Description

### _2.1 Functionality_

The goal is to create Space Invaders for the Texas Instruments TIVA Launchpad.

The outputs will be:

* OLED Display
* LEDs
* Sound

The inputs will be:
* Buttons
* Joystick

The software will use an edge-triggered interrupt to execute software whenever
a button is pressed. It will use two periodic interrupts. ONe fixed-frequency 
periodic interrupt will be used to output sounds with the DAC. The second
periodic interrupt will move the sprite. Actual LCD output will always be
performed in the main program.

### _2.2 Scope_

The project will be developed in the following order:
1. Define the rules of gameplay.
2. Specify the modules.
3. Design the prototypes for the public functions.
4. Develop and test the modules individually.
5. Combine the modules and perform integration tests

### _2.3 Prototypes_

Each module will be individually tested. There will be about four modules but 
there may be more as the system is fleshed out. Finally, there will be a short
video showcasing the whole system and gameplay.

### _2.4 Performance_

The game will be easy to learn and fun to play. An optimization level of `-O3`
will be used to guarantee optimal compiler optimizations.

### _2.5 Usability_

The inputs will be two momentary switched. One switch will be the one on the 
joystick. The second switch will be attached externally. These will be used
for screen selection and the external one will be used for shooting. The 
joystick X directional will be used to move the player's sprite left and right.

The outputs of the system will be the LED that will display the game graphics
and a speaker connected to an audio jack.

### _2.6 Safety_

The static analysis tool _cppcheck_ will be used to perform static analysis on
all the source files. This will help reduce major bugs and weak points in the
code.

## 3. Deliverables

### _3.1 Reports_

* A 1-3 minute video demonstrating gameplay.
* A short description of the game will be submitted.

### _3.2 Audits_

There will be a place on the Lab 15 grading page in edX for other students and
the instructors to add comments and suggestions.

### _3.3 Outcomes_

Demonstrate mastery of all lessons learned in the UT Austin course.
