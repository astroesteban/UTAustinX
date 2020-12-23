// Sound.c
// Runs on LM4F120 or TM4C123,
// edX lab 13
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// This routine calls the 4-bit DAC

#include <stdbool.h>

#include "Sound.h"
#include "DAC.h"
#include "tm4c123gh6pm.h"

// Used in the systick handler for output
static bool g_soundOut = false;

// The number of data points in the sine wave array
#define SINE_WAVE_LEN		32U

// a smooth digital value for analog sine wave
static const unsigned char g_sineWave[SINE_WAVE_LEN] = {
	8, 9, 11, 12, 13, 14, 14, 15, 15, 15,
	14, 14, 13, 12, 11, 9, 8, 7, 5, 4,
	3, 2, 2, 1, 1, 1, 2, 2, 3, 4,
	5,7
};

// keep track of what sine wave value to output next
static unsigned char g_sineWaveIndex = 0;

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Also calls DAC_Init() to initialize DAC
// Input: none
// Output: none
void Sound_Init(void)
{
    // Initialize the DAC
    DAC_Init();

    // Disable the interrupt on startup
    NVIC_ST_CTRL_R = 0;

    // Set the initial reload value to zero
    NVIC_ST_RELOAD_R = 0;

    // Write any number to current to clear it
    NVIC_ST_CURRENT_R = 0;

    // Set the priority to 1
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000;
	
		// Enable SysTick with core clock and interrupts
		NVIC_ST_CTRL_R = 0x0007;
}

// **************Sound_Tone*********************
// Change Systick periodic interrupts to start sound output
// Input: interrupt period
//           Units of period are 12.5ns
//           Maximum is 2^24-1
//           Minimum is determined by length of ISR
// Output: none
void Sound_Tone(unsigned long tone)
{
	if (NVIC_ST_RELOAD_R == tone) { return; }
	
	g_soundOut = tone != 0 ? true : false;
	
	// Always disable interrupts while you configure them
	NVIC_ST_CTRL_R = 0;
	// Clear current
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_RELOAD_R = tone;
	NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R & 0x00FFFFFF;  // priority 0
	// Enable with interrupts
	NVIC_ST_CTRL_R = 0x00000007;
}

// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(void)
{
	Sound_Tone(0);
	DAC_Out(0);
}

// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void)
{
	if (g_soundOut) {
		g_sineWaveIndex = (++g_sineWaveIndex) % SINE_WAVE_LEN;
		DAC_Out(g_sineWave[g_sineWaveIndex]);
	}
	else {
		DAC_Out(0);
	}
}
