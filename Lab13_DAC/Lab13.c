// Lab13.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// edX Lab 13
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC
// Port E bits 3-0 have 4 piano keys

#include "tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"

// basic functions defined at end of startup.s
void DisableInterrupts(void);
void EnableInterrupts(void);
void delay(unsigned long msec);

int main(void)
{
    // Real Lab13
    // for the real board grader to work
    // you must connect PD3 to your DAC output
    TExaS_Init(SW_PIN_PE3210, DAC_PIN_PB3210, ScopeOn); // activate grader and set system clock to 80 MHz
                                                        // PortE used for piano keys, PortB used for DAC
                                                        // initialize SysTick timer and DAC
    Sound_Init();
    Piano_Init();
    EnableInterrupts();

    while (1) {
        // input from keys to select tone
        unsigned long keyPress = Piano_In();
			
				switch (keyPress) {
					case SILENCE: Sound_Off(); break;
					case C_NOTE: Sound_Tone(C_PERIOD); break;
					case D_NOTE: Sound_Tone(D_PERIOD); break;
					case E_NOTE: Sound_Tone(E_PERIOD); break;
					case G_NOTE: Sound_Tone(G_PERIOD); break;
					default: Sound_Off();
				}

				// Add a delay for the switch bouncing
				delay(10);
    }
}

// Inputs: Number of msec to delay
// Outputs: None
void delay(unsigned long msec)
{
    while (msec > 0)
    {                                // repeat while there are still delay
        unsigned long count = 16000; // about 1ms
        while (count > 0)
        {
            count--;
        } // This while loop takes approximately 3 cycles
        msec--;
    }
}
