/*
                         (_)                   | |              
 ___ _ __   __ _  ___ ___ _ _ ____   ____ _  __| | ___ _ __ ___ 
/ __| '_ \ / _` |/ __/ _ \ | '_ \ \ / / _` |/ _` |/ _ \ '__/ __|
\__ \ |_) | (_| | (_|  __/ | | | \ V / (_| | (_| |  __/ |  \__ \
|___/ .__/ \__,_|\___\___|_|_| |_|\_/ \__,_|\__,_|\___|_|  |___/
    | |                                                         
    |_|                                                      
*/

//
// Headers
//
#include "tm4c123gh6pm.h"
#include "joystick.h"

//
// Global Variables
//

//
// Function Definitions
//

// Initializes the system clock to use the main oscillator @80 MHz
// We could use a library which would make this more portable but where's the
// fun in that. Especially for learning embedded systems.
void systemClockInit(void);

//
// ENTRYPOINT
//
int main(void)
{
    // Setup the clock to 80 MHz
    systemClockInit();

    // * Main Game Loop
    while (1) {
    }
}


void systemClockInit(void)
{
    // Use RCC2
    SYSCTL_RCC2_R |= (1 << 31);
    // Bypass the PLL while initializing
    SYSCTL_RCC2_R |= (1 << 11);
    // Set the crystal value (see datasheet: 0x15 = 16.0 MHz)
    SYSCTL_RCC_R = (SYSCTL_RCC_R & ~0x000007C0) // clear XTAL field, bits 10-6
                   + +0x00000540;               // 10101, configure for 16 MHz crystal
    // Configure for main oscillator source
    SYSCTL_RCC2_R &= ~0x00000070;
    // Activate the PLL by clearing PWRDN2
    SYSCTL_RCC2_R &= ~(1 << 13);
    // Enable 400MHz PLL
    SYSCTL_RCC2_R |= 0x40000000;
    // Set the divisor to run at 80 MHz
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000) // clear system clock divider
                    + (4 << 22);                  // configure for 80 MHz clock
    // Wait for the PLL to be locked by polling PLLLRIS
    while ((SYSCTL_RIS_R & 0x00000040) == 0);
    // Disable the PLL bypass
    SYSCTL_RCC2_R &= ~(1 << 11);
}