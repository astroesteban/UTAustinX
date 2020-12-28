# Lab 14

> ADC and Data Acquisition

## Learning Objectives

* Develop a means for a digital computer to sense its analog world.
* Review digitization: Quantization, range, precision and resolution.
* Extend the Nyquist Theorem to cases the ADC is used to sense information.
* Study the basics of transducers: conversion of physical to electrical.
* Use an optical sensor to measure distance to an object.

## About the lab

The purpose of lab 14 was to provide:

1. An introduction to sampling analog signals using the ADC interface
2. The development of an ADC device driver
3. Learning data conversion and calibration techniques
4. The use of fixed-point numbers, which are integers used to represent non-integer values
5. The development of an interrupt-driven real-time sampling device driver
6. The development of a software system involving multiple files
7. Learn how to debug one module at a time

### Wiring

On the potentiometer we have:

* Pin 1 connected to GND
* Pin 3 connected to +3.3V
* Pin 2 connected to PE3

<br>

<img src="assets/assembly.jpg"
     alt="A picture of the Lab 14 breadboard setup" />

### Lessons Learned

I had the optimization level at its maximum, `-O3`, and I hadn't realized my
flag used for notifying the main thread when the data is ready from the SysTick
IRQ had not been declared as `volatile`. Thus it was optimized out. Another thing
I already knew given my experience was that there would be noise in the ADC in the 
hardware-in-the-loop implementation vs the software-in-the-loop implementation.