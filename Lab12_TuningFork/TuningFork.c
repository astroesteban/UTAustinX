// TuningFork.c Lab 12
// Runs on LM4F120/TM4C123
// Use SysTick interrupts to create a squarewave at 440Hz.
// There is a positive logic switch connected to PA3, PB3, or PE3.
// There is an output on PA2, PB2, or PE2. The output is
//   connected to headphones through a 1k resistor.
// The volume-limiting resistor can be any value from 680 to 2000 ohms
// The tone is initially off, when the switch goes from
// not touched to touched, the tone toggles on/off.
//                   |---------|               |---------|
// Switch   ---------|         |---------------|         |------
//
//                    |-| |-| |-| |-| |-| |-| |-|
// Tone     ----------| |-| |-| |-| |-| |-| |-| |---------------
//
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015

 Copyright 2016 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

#include <stdbool.h>

#include "TExaS.h"
#include "..//tm4c123gh6pm.h"

//
// global variables
//
static volatile unsigned long g_prevButtonState = 0;
static volatile bool g_signalOut = false;
static volatile unsigned long g_buttonState = 0;

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode

//
// function prototypes
//

// Used to initialize the SysTick with
void systickInit(void);

// input from PA3, output from PA2, SysTick interrupts
void Sound_Init(void)
{
	// Enable the port a clock
	SYSCTL_RCGC2_R |= (1 << 0);

	// Add a quick delay
	volatile unsigned long delay = 0;

	// Disable the analog function
	GPIO_PORTA_AMSEL_R &= ~(1 << 2);
	GPIO_PORTA_AMSEL_R &= ~(1 << 3);

	// GPIO clear bit PCTL
	GPIO_PORTA_PCTL_R = 0x00000000;

	// PA2 output, PA3 input
	GPIO_PORTA_DIR_R |= ~(1 << 3);
	GPIO_PORTA_DIR_R |= (1 << 2);

	// No alternate functions
	GPIO_PORTA_AFSEL_R &= ~(1 << 2);
	GPIO_PORTA_AFSEL_R &= ~(1 << 3);

	// Enable pull down resistors on PA3
	GPIO_PORTA_PDR_R |= (1 << 3);

	// Enable digital pins PA2-3
	GPIO_PORTA_DEN_R |= (1 << 2);
	GPIO_PORTA_DEN_R |= (1 << 3);
}

// called at 880 Hz
void SysTick_Handler(void)
{
	g_buttonState = GPIO_PORTA_DATA_R & 0x08;

	// check if the button was pressed
	if ((g_buttonState != 0) && (g_prevButtonState == 0))
	{
		// Check if the signal is already going out
		if (g_signalOut)
		{
			g_signalOut = false;
			GPIO_PORTA_DATA_R &= ~(1 << 2); // turn output off
		}
		else
		{
			g_signalOut = true;
		}
	}

	// if buzzer is on, toggle PA2
	if (g_signalOut)
	{
		GPIO_PORTA_DATA_R ^= 0x4;
	}

	g_prevButtonState = g_buttonState;
}

int main(void)
{
	// activate grader and set system clock to 80 MHz
	TExaS_Init(SW_PIN_PA3, HEADPHONE_PIN_PA2, ScopeOn);
	Sound_Init();
	systickInit();
	EnableInterrupts(); // enable after all initialization are done

	// Just loop waiting for the SysTick interrupt
	while (1)
	{
	}
}

//
// Function definitions
//
void systickInit(void)
{
	// Disable SysTick during configuration
	NVIC_ST_CTRL_R = 0;

	// Reload value for 1.13636 ms (assuming 80MHz)
	NVIC_ST_RELOAD_R = 90908;

	// Any write to current clears it
	NVIC_ST_CURRENT_R = 0;

	// Set the priority to zero
	NVIC_SYS_PRI3_R &= 0x00FFFFFF;

	// Enable SysTick with core clock
	NVIC_ST_CTRL_R |= (1 << 2); // set clk src to system clock
	NVIC_ST_CTRL_R |= (1 << 1); // enable the interrupt
	NVIC_ST_CTRL_R |= (1 << 0); // enable the counter
}
