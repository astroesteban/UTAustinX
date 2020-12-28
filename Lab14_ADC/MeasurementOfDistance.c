// MeasurementOfDistance.c
// Runs on LM4F120/TM4C123
// Use SysTick interrupts to periodically initiate a software-
// triggered ADC conversion, convert the sample to a fixed-
// point decimal distance, and store the result in a mailbox.
// The foreground thread takes the result from the mailbox,
// converts the result to a string, and prints it to the
// Nokia5110 LCD.  The display is optional.
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

// Slide pot pin 3 connected to +3.3V
// Slide pot pin 2 connected to PE2(Ain1) and PD3
// Slide pot pin 1 connected to ground

#include <stdbool.h>

#include "ADC.h"
#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "TExaS.h"

void EnableInterrupts(void);

//
// Globals
//

// units 0.001 cm
unsigned long Distance = 0;
// TRUE means data is ready, FALSE means data is not ready
static volatile bool g_dataReady = false;

//
// Function Definitions
//

//********Convert****************
// Convert a 12-bit binary ADC sample into a 32-bit unsigned
// fixed-point distance (resolution 0.001 cm).  Calibration
// data is gathered using known distances and reading the
// ADC value measured on PE1.
// Overflow and dropout should be considered
// Input: sample  12-bit ADC sample
// Output: 32-bit distance (resolution 0.001cm)
static unsigned long Convert(unsigned long sample)
{
    // distance = ((A * adc_data) >> 10) + B
    return ((950 * sample) >> 10) + 80; // For HITL
                                        // return sample * 0.4885;  // For simulator
}

// Initialize SysTick interrupts to trigger at 40 Hz, 25 ms
void SysTick_Init(void)
{
    // Disable the systick during initialization
    NVIC_ST_CTRL_R = 0;

    // Assuming the bus clock is 80MHz convert your target frequecy to
    // nanoseconds, and solve: (RELOAD+1)*12.5ns = target_freqency (ns)
    NVIC_ST_RELOAD_R = 1999999; // Interrupt every 40 Hz (25,000,000 ns)

    // Any write to current clears it
    NVIC_ST_CURRENT_R = 0;

    // Set the priority to zero
    //NVIC_SYS_PRI3_R &= 0x00FFFFFF;
    // Set the priority to one
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000;

    // Enable SysTick with system clock, interrupts, and counter
    NVIC_ST_CTRL_R |= 0x0007;
}

// executes every 25 ms, collects a sample, converts and stores in mailbox
void SysTick_Handler(void)
{
    // Toggle PF2
    GPIO_PORTF_DATA_R ^= 0x04;

    // Toggle PF2 again
    GPIO_PORTF_DATA_R ^= 0x04;

    // Sample the ADC
    unsigned long adcData = ADC0_In();

    // Convert the sampled data to distance
    Distance = Convert(adcData);

    // Set the flag to signify that the data is ready
    g_dataReady = true;

    // Toggle PF2 a third time
    GPIO_PORTF_DATA_R ^= 0x04;
}

//-----------------------UART_ConvertDistance-----------------------
// Converts a 32-bit distance into an ASCII string
// Input: 32-bit number to be converted (resolution 0.001cm)
// Output: store the conversion in global variable String[10]
// Fixed format 1 digit, point, 3 digits, space, units, null termination
// Examples
//    4 to "0.004 cm"
//   31 to "0.031 cm"
//  102 to "0.102 cm"
// 2210 to "2.210 cm"
//10000 to "*.*** cm"  any value larger than 9999 converted to "*.*** cm"
static void UART_ConvertDistance(unsigned char *stringBuffer, unsigned long n)
{
    // Set the " cm" part
    stringBuffer[5] = ' ';
    stringBuffer[6] = 'c';
    stringBuffer[7] = 'm';
    stringBuffer[8] = 0;
    stringBuffer[9] = 0;

    if (n > 9999)
    {
        for (int i = 0; i <= 4; i++)
        {
            if (i == 1)
            {
                stringBuffer[i] = '.';
            }
            else
            {
                stringBuffer[i] = '*';
            }
        }
    }
    else
    {
        for (int i = 4; i >= 0; i--)
        {
            if (i == 1)
            {
                stringBuffer[i] = '.';
            }
            else
            {
                unsigned char c = n % 10;
                n /= 10;
                stringBuffer[i] = c + 0x30;
            }
        }
    }
}

// Utilize the blue LED and SW1 for debugging
static void debugPortInit(void)
{
    volatile unsigned long delay;

    SYSCTL_RCGC2_R |= 0x20;          // enable port F clock
    delay = SYSCTL_RCGC2_R;          // allow time for clock to start
    GPIO_PORTF_LOCK_R |= 0x4C4F434B; // unlock GPIO port F
    GPIO_PORTF_CR_R |= 0x14;         // allow change to PF4, PF2
    GPIO_PORTF_DIR_R |= 0x04;        // PF4 input, PF2 output
    GPIO_PORTF_AFSEL_R &= ~0x14;     // disable alternate function on PF4, PF2
    GPIO_PORTF_PCTL_R = 0;           // disable PCTL
    GPIO_PORTF_DEN_R |= 0x14;        // enable digital I/O on PF4, PF2
    GPIO_PORTF_PUR_R |= 0x10;        // enable pull-up resistor on PF4
    GPIO_PORTF_AMSEL_R &= ~0x14;     // disable analog function on PF4, PF2
}

//
// Entrypoint
//

int main(void)
{
    TExaS_Init(ADC0_AIN1_PIN_PE2, UART0_Emulate_Nokia5110_NoScope);

    // initialize ADC0, channel 1, sequencer 3
    ADC0_Init();
    // initialize Nokia5110 LCD (optional)
    Nokia5110_Init();
    // initialize SysTick for 40 Hz interrupts
    SysTick_Init();
    // initialize profiling on PF2
    debugPortInit();

    EnableInterrupts();

    unsigned char stringBuffer[10];

    while (1)
    {
        g_dataReady = false;

        while (!g_dataReady)
            ; // Wait for the data to be ready

        // Output to the display
        UART_ConvertDistance(stringBuffer, Distance);
        Nokia5110_SetCursor(0, 0);
        Nokia5110_OutString(stringBuffer);
    } // end while
}
