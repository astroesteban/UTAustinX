// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0 SS3 to be triggered by
// software and trigger a conversion, wait for it to finish,
// and return the result.
// Daniel Valvano
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

#include "ADC.h"
#include "..//tm4c123gh6pm.h"

// This initialization function sets up the ADC
// Max sample rate: <=125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source:  channel 1
// SS3 interrupts: enabled but not promoted to controller
void ADC0_Init(void)
{
    volatile unsigned long delay = 0;
    // We will be binding ADC0 to PE2

    // Enable the port clock for PE2
    SYSCTL_RCGC2_R |= (1 << 4);

    // Add a delay to allow the clock to stabilize
    delay = SYSCTL_RCGC2_R;

    // Make PE2 an input
    GPIO_PORTE_DIR_R &= ~(1 << 2);

    // Enable the alternate function for PE2
    GPIO_PORTE_AFSEL_R |= (1 << 2);

    // Disable the digital function for PE2
    GPIO_PORTE_DEN_R &= ~(1 << 2);

    // Enable the analog function of PE2
    GPIO_PORTE_AMSEL_R |= (1 << 2);

    // Enable the ADC0 clock
    SYSCTL_RCGC0_R |= (1 << 16);

    delay = SYSCTL_RCGC0_R;

    // Set the max sampling rate for ADC0 (Bits 8:9)
    SYSCTL_RCGC0_R &= ~0x300; // Set to 125k samples/second

    // Set sequencer 3 as the highest priority
    ADC0_SSPRI_R = 0x0123;

    // Disable sequencer 3 while we configure it
    ADC0_ACTSS_R &= ~(1 << 3);

    // Set the trigger as a software trigger (systick)
    ADC0_EMUX_R &= ~0xF000;

    // Clear the SS3 field
    ADC0_SSMUX3_R &= ~0x000F;
    // set channel Ain1 (PE2)
    ADC0_SSMUX3_R += 1;

    // No TS0 D0, yes IE0 END0
    ADC0_SSCTL3_R = 0x0006;

    // Enable sample sequencer 3
    ADC0_ACTSS_R |= 0x0008;
}

//------------ADC0_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
unsigned long ADC0_In(void)
{
    // Initiate SS3
    ADC0_PSSI_R |= (1 << 3);

    // wait for the conversion to be done
    while ((ADC0_RIS_R & 0x08) == 0)
        ;

    // Read the result
    unsigned long result = ADC0_SSFIFO3_R & 0xFFF;

    // Acknowledge completion
    ADC0_ISC_R |= (1 << 3);

    return result;
}
