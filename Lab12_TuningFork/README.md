# Lab 12

> Interrupts: Tuning Fork

## Overview

The learning objectives of this chapter were to:

* Appreciate the need to perform multiple tasks concurrently.
* Understand real-time performance measures such as bandwidth and latency.
* Learn how interrupts can be used to minimize latency.
* Study the basics of interrupt programming: arm, enable, trigger, vector, priority, acknowledge.
* Understand how to use SysTick to create periodic interrupts.
* Use SysTick to create sounds and spin motors.

## About the lab

The goal of this lab was to implement the learning objectives of chapter 12 by
having a switch as input control the output to a speaker/headphones.

I accomplished this lab by doing the following:

1. The positive logic switch
    1. Connect the GND pin on your MCU to GND on the breadboard.
    2. Connect the +3.3V pin to one of the legs on your button.
    3. Connect the opposite leg of the switch to pin PA3 *before* the resistor.
    4. Connect a 10-kilohm resistor to the same leg that is connected to PA3 to GND.
2. The sleeve, ring, tip audio jack
    1. Connect pin PA2 along with a 1.5-kilohm resistor in between to the sleeve (middle) of the jack.
    2. Connec the ring and tip pins of the jack to ground.
3. Play some beats...or just some loud sound.

<br>
<img src="assets/one.png"
     alt="Overview of MCU Wiring"
     style="float: left; margin-right: 10px;" />

Here's another view

<br>

<img src="assets/two.png"
     alt="Another overview of MCU wiring"
     style="float: left; margin-right: 10px;" />

<br>

And here is a cool video:

<a href="https://youtu.be/M9ce_Ca4KyM" title="Lab 12"><img src="https://youtu.be/M9ce_Ca4KyM" alt="Lab 12 Video" /></a>


### Lessons Learned

#### Software

* The biggest trouble I had with this lab was with the SysTick initialization. I 
  forgot to set the priority to zero in order to make it higher than the "main"
  program's priority. This resulted in some confusing bug where I could see
  the button's current in the oscilloscope but when I pushed it the current would
  go up and nothing would happen. I then debugged it by creating a temporary variable
  in the `SysTick_Handler` function that would get increased by one every time the
  handler was called. I watched the variable in the debugger and realized it was
  never incremented. This narrowed down the scope of the problem to the `systickInit`
  function. I double-checked my code and realized the priority of the interrupt
  was not set.

#### Hardware

* The roadblock for me in this lab in terms of hardware was understanding the 
  schematic. I would say my schematic reading abilities are weak. I could not
  get it hooked up right. The challenge to myself is to read up on schematics so
  that I can improve in this area.