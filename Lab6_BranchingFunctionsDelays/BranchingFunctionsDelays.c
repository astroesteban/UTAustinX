// BranchingFunctionsDelays.c Lab 6
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to 
// toggle an LED while a button is pressed and 
// turn the LED on when the button is released.  
// This lab will use the hardware already built into the LaunchPad.
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED
// built-in connection: PF3 connected to green LED
// built-in connection: PF4 connected to negative logic momentary switch, SW1

#include "TExaS.h"

#include <stdbool.h>


#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control


// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// function prototypes
void delayMs(unsigned long time);
void initPort(void);

//
// Entrypoint
//

int main(void){ unsigned long volatile delay;
	// Activate grader and set SysClk to 80 MHz
  TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);
	
  // Initialize GPIO
	initPort();
	
	unsigned long stateSW1 = 0;
	
  while (1) {
		// read PF4 (SW1 switch)
		stateSW1 = ((GPIO_PORTF_DATA_R & 0x10) >> 4);
		
		if (stateSW1 == 0) {	// SW1 is pressed
			// Blink the blue LED
			delayMs(1);
			GPIO_PORTF_DATA_R ^= (1 << 2);
			delayMs(1);
		} else {
			// Keep the blue LED on
			GPIO_PORTF_DATA_R |= (1 << 2);
		}
  }
}


//
// Function definitions
//

// Adds a delay of time * 1333333
void delayMs(unsigned long time)
{
	unsigned long i;
	
	while (time > 0) {
		i = 1333333;  // this number means 100ms
		
		while (i > 0) {
			--i;
		}
		
		--time;
	}
}

void initPort(void)
{
	// Turn on the clock for Port F
	SYSCTL_RCGC2_R = 0x20;
	
	// Allow time for clock to start
	unsigned long delay = SYSCTL_RCGC2_R;
	
	// Clear the PF4 and PF2 bits in Port F AMSEL to disable analog
	GPIO_PORTF_AMSEL_R = 0x00;
	
	// Clear the PF4 and PF2 bit fields in Port F PCTL to configure as GPIO
	GPIO_PORTF_PCTL_R = 0x00000000;
	
	// Set the Port F direction register so 
  // 		PF4 is an input and 
  // 		PF2 is an output
	GPIO_PORTF_DIR_R &= ~(1 << 4);
	GPIO_PORTF_DIR_R |= (1 << 2);
	
	// Clear the PF4 and PF2 bits in Port F AFSEL to disable alternate functions
	GPIO_PORTF_AFSEL_R &= ~(1 << 4);
	GPIO_PORTF_AFSEL_R &= ~(1 << 2);
	
	// Set the PF4 and PF2 bits in Port F DEN to enable digital
	GPIO_PORTF_DEN_R |= (1 << 4);
	GPIO_PORTF_DEN_R |= (1 << 2);
	
	// Set the PF4 bit in Port F PUR to activate an internal pullup resistor
	GPIO_PORTF_PUR_R |= (1 << 4);
	
	// Set the PF2 bit in Port F DATA so the LED is initially ON
	GPIO_PORTF_DATA_R |= (1 << 2);
}
