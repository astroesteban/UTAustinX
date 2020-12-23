// DAC.c
// Runs on LM4F120 or TM4C123,
// edX lab 13
// Implementation of the 4-bit digital to analog converter
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC

#include "DAC.h"
#include "tm4c123gh6pm.h"

// **************DAC_Init*********************
// Initialize 4-bit DAC
// Input: none
// Output: none
void DAC_Init(void)
{
    // Enable the Port B Clock
    SYSCTL_RCGC2_R |= (1 << 1);

    // Add a delay to give time to the clk to stabilize
    volatile unsigned long delayVar = 0;

    // Unlock GPIO Port B
    GPIO_PORTB_LOCK_R |= 0x4C4F434B;

    // Allow change to PB3-PB0
    GPIO_PORTB_CR_R |= 0x0F;

    // Disable analog functionality
    GPIO_PORTB_AMSEL_R &= ~0x0F;

    // GPIO clear bit PCTL
    GPIO_PORTB_PCTL_R &= 0x00;

    // Set P0-3 as output
    GPIO_PORTB_DIR_R |= 0x0F;

    // No alternate functions
    GPIO_PORTB_AFSEL_R &= 0x00;

    // Enable digital pins P0-3
    GPIO_PORTB_DEN_R |= 0x0F;

    // Enable 8-mA drive select on PB3-PB0
    GPIO_PORTB_DR8R_R |= 0x0F;
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15
// Output: none
void DAC_Out(unsigned long data)
{
    // Write all the values of the sine wave array
    GPIO_PORTB_DATA_R = data;
}
