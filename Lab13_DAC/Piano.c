// Piano.c
// Runs on LM4F120 or TM4C123,
// edX lab 13
// There are four keys in the piano
// Daniel Valvano
// December 29, 2014

// Port E bits 3-0 have 4 piano keys

#include <stdint.h>

#include "Piano.h"
#include "tm4c123gh6pm.h"

unsigned char g_pianoKey = 4;

// **************Piano_Init*********************
// Initialize piano key inputs
// Input: none
// Output: none
void Piano_Init(void)
{
    // Enable the Port E Clock
    SYSCTL_RCGC2_R |= (1 << 4);

    // Add a delay to give the clock time to stabilize
    volatile unsigned long delay = 0;

    // Unlock Port E to enable write access to the GPIOCR register
    GPIO_PORTE_LOCK_R |= 0x4C4F434B;

    // Commit Port E pins 0-3
    GPIO_PORTE_CR_R |= 0x0F;

    // Disable analog functionality
    GPIO_PORTE_AMSEL_R &= ~0x0F;

    // Disable alternate functionality
    GPIO_PORTE_AFSEL_R &= ~0x0F;

    // Clear the PCTL register for PE0-PE3
    GPIO_PORTE_PCTL_R = 0x00;

    // Set PE0-PE3 as input
    GPIO_PORTE_DIR_R &= ~0x0F;

    // Enable the digial functions of PE0-PE3
    GPIO_PORTE_DEN_R |= 0x0F;

    // Enable the pull-down resistor for PE0-PE3
    GPIO_PORTE_PDR_R |= 0x0F;
}

// **************Piano_In*********************
// Input from piano key inputs
// Input: none
// Output: 0 to 15 depending on keys
// 0x01 is key 0 pressed, 0x02 is key 1 pressed,
// 0x04 is key 2 pressed, 0x08 is key 3 pressed
unsigned long Piano_In(void)
{
	return GPIO_PORTE_DATA_R & 0x0F;
}
