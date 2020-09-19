// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// January 15, 2016
//      Jon Valvano and Ramesh Yerraballi

// ***** 1. Pre-processor Directives Section *****
#include <stdbool.h>

#include "TExaS.h"
#include "tm4c123gh6pm.h"

#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// ***** 3. Subroutines Section *****
void delayMs(unsigned long time);
void initPort(void);


// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).
int main(void){ 
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);  // activate grader and set system clock to 80 MHz
  
	initPort();
	
  EnableInterrupts();           // enable interrupts for the grader
	
	bool buttonPressed = false;
	
  while(true) {
		delayMs(1);
		
		buttonPressed = ( (GPIO_PORTE_DATA_R & 0x01) == 1 );
	  
		if (buttonPressed) {
			// Toggle LED
			GPIO_PORTE_DATA_R ^= (1 << 1);
		} else {
			// LED ON
			GPIO_PORTE_DATA_R |= (1 << 1);
		}
  }
}

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
    // Turn on the clock for Port E
    SYSCTL_RCGC2_R = 0x10;

    // Allow time for clock to start
    volatile unsigned long delay = SYSCTL_RCGC2_R;
	
		// Unlock Port E
		GPIO_PORTE_LOCK_R = 0x4C4F434B;
		
		// Allow changes to PE1, PE0
		GPIO_PORTE_CR_R |= 0x03;

    // Clear the PE0 and PE1 bits in Port E AMSEL to disable analog
    GPIO_PORTE_AMSEL_R = 0x00;

    // Clear the PE0 and PE1 bit fields in Port E PCTL to configure as GPIO
    GPIO_PORTE_PCTL_R &= 0xFFFFFF00;

    // Make PE0 an input and PE1 an output
    GPIO_PORTE_DIR_R &= ~(1 << 0);
    GPIO_PORTE_DIR_R |= (1 << 1);

    // Clear the PE0 and PE1 bits in Port E AFSEL to disable alternate functions
		GPIO_PORTE_AFSEL_R = 0x00;

    // Set the PE0 and PE1 bits in Port E DEN to enable digital
    GPIO_PORTE_DEN_R |= (1 << 0);
    GPIO_PORTE_DEN_R |= (1 << 1);

    // Set the PE1 bit in Port F DATA so the LED is initially ON
    GPIO_PORTE_DATA_R |= (1 << 1);
}
