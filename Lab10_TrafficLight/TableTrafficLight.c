/*
PURPOSE:
    This lab has these major objectives:
        1) the understanding and implementing of indexed data structures;
        2) learning how to create a segmented software system;
        3) the study of real-time synchronization by designing a
            finite state machine controller.
    Software skills you will learn include:
            advanced indexed addressing, linked data structures,
            creating fixed-time delays using the SysTick timer, and debugging
            real-time systems. Please read the entire lab before starting.

DEV NOTES:
    1) When outputting to two ports, output to the car LEDs first,
    and output to Port F (walk, don't walk) second
    2) When outputting to a port, do not write intermediate values
    3) Do not bother being friendly, it just confuses the grader
*/

//
// Includes
//
#include <stdio.h>

#include "TExaS.h"
#include "tm4c123gh6pm.h"

//
// Local Defines
//
#define NUM_NEXT_STATES         8U

//
// Local Structs
//
typedef const struct State_t {
    unsigned long trafficLightOut;
    unsigned long walkLightOut;
    unsigned long waitTime;
    unsigned long nextState[NUM_NEXT_STATES];
} State_t;

//
// Function Prototypes
//
void DisableInterrupts(void);
void EnableInterrupts(void);
// Initializes and configures the oscillator
static void PLL_Init(void);
// Initializes and configures the GPIO ports
static void Port_Init(void);
// Initializes and configures the SysTick interrupt
static void SysTick_Init(void);
// Makes the system wait 10ms using the SysTick interrupt
static void SysTick_Wait10ms(unsigned long delay);
// Adds a delay of delay * 10ms
static void SysTick_Wait(unsigned long delay);

//
// Application Entrypoint
//
int main(void) {
    /*** Initialization ***/
    // Activate grader and set system clock to 80 MHz
    TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff);
    PLL_Init();
    Port_Init();
    SysTick_Init();

    /*** Required Variables ***/
    // Setup the state machine
    State_t stateMachine[] = {
        { 0x0C, 0x02, 20, { 0, 0, 1, 1, 1, 1, 1, 1 } },            // WestGreen
        { 0x14, 0x02, 30, { 2, 2, 2, 2, 4, 4, 4, 2 } },            // WestYellow
        { 0x21, 0x02, 20, { 2, 3, 2, 3, 3, 3, 3, 3 } },            // SouthGreen
        { 0x22, 0x02, 30, { 0, 0, 0, 0, 4, 4, 4, 4 } },            // SouthYellow
        { 0x24, 0x08, 20, { 4, 5, 5, 5, 4, 5, 5, 5 } },            // WalkGreen
        { 0x24, 0x00, 5, { 6, 6, 6, 6, 6, 6, 6, 6 } },            // WalkFlash[Off1]
        { 0x24, 0x02, 5, { 7, 7, 7, 7, 7, 7, 7, 7 } },            // WalkFlash[On1]
        { 0x24, 0x00, 5, { 8, 8, 8, 8, 8, 8, 8, 8 } },            // WalkFlash[Off2]
        { 0x24, 0x02, 5, { 9, 9, 9, 9, 9, 9, 9, 9 } },            // WalkFlash[On2]
        { 0x24, 0x00, 5, { 10, 10, 10, 10, 10, 10, 10, 10 } },    // WalkFlash[Off3]
        { 0x24, 0x02, 5, { 4, 0, 2, 0, 4, 0, 2, 0 } }             // WalkFlash[On3]
    };

    // Holds the current state of the FSM
    unsigned long currentState, buttonInput;

		EnableInterrupts();
		
    /*** Main Execution Loop ***/
    while (1) {
        // Set the traffic lights
        GPIO_PORTB_DATA_R = stateMachine[currentState].trafficLightOut;

        // Set the pedestrian lights
        GPIO_PORTF_DATA_R = stateMachine[currentState].walkLightOut;

        // Wait for the specified amount of time in the FSM
				SysTick_Wait10ms(stateMachine[currentState].waitTime);
				
				// Get the input from the buttons
				buttonInput = GPIO_PORTE_DATA_R & 0x07;

        // Update the current state based on what sensors are pressed
        currentState = stateMachine[currentState].nextState[buttonInput];
    }
}

//
// Function Definitions
//
void PLL_Init(void) {
    // Use RCC2 by setting the USERCC2 bit
    SYSCTL_RCC2_R |= 0x80000000;

    // Bypass PLL while initializing
    SYSCTL_RCC2_R |= (1 << 11);

    // Clear the XTAL field
    SYSCTL_RCC_R &= ~0x00000070;

    // Configure for 16 MHz source
    SYSCTL_RCC_R |= (0x15 << 6);

    // Configure for main oscillator source
    SYSCTL_RCC_R &= ~0x00000070;

    // Activate PLL by clearing PWRDN
    SYSCTL_RCC_R &= ~(1 << 13);

    // Set the desired system divider (400 MHz)
    SYSCTL_RCC_R |= 0x40000000;

    // Clear system clock divider && configure for 80 MHz clock
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000) + (4 << 22);

    // Wait for the PLL to lock by polling PLLLRIS
    while ( (SYSCTL_RIS_R & 0x00000040) == 0 );

    // Enable use of PLL by clearing BYPASS
    SYSCTL_RCC2_R &= ~0x00000800;
}

void Port_Init(void) {
    // Enable clocks
    // Activate Clock for Port B, E, F
        SYSCTL_RCGC2_R = 0x32;
    // Give the clock some time to stabilize
    volatile unsigned long delay = SYSCTL_RCGC2_R;
    (void)delay; // Get rid of the "unused" error

    // Initialize Port B (Traffic Lights)
    GPIO_PORTB_LOCK_R = 0x4C4F434B;         // Unlock port
    GPIO_PORTB_CR_R = 0x3F;                 // Allow changes  to PB5-0
    GPIO_PORTB_AMSEL_R &= ~0x3F;            // Disable analog function
    GPIO_PORTB_PCTL_R = 0x00000000;         // GPIO clear bit PCTL
    GPIO_PORTB_DEN_R = 0x3F;                // Enable digital I/O on PF5-0
    GPIO_PORTB_DIR_R = 0x3F;                // PF5, PF4, PF3, PF2, PF1, PF0 OUTPUT

    // Initialize Port E (Sensors)
    GPIO_PORTE_LOCK_R = 0x4C4F434B;
    GPIO_PORTE_CR_R = 0x07;
    GPIO_PORTE_AMSEL_R &= ~0x07;
    GPIO_PORTE_PCTL_R = 0x00000000;
    GPIO_PORTE_DEN_R = 0x07;
    GPIO_PORTE_DIR_R &= ~0x07;

    // Initialize Port F (Walk Lights)
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x0A;
    GPIO_PORTF_AMSEL_R &= ~0x0A;
    GPIO_PORTF_PCTL_R = 0x00000000;
    GPIO_PORTF_DEN_R = 0x0A;
    GPIO_PORTF_DIR_R = 0x0A;
}

void SysTick_Init(void) {
    // Disable SysTick during setup
    NVIC_ST_CTRL_R = 0;
    // Enable SysTick with core clock
    NVIC_ST_CTRL_R = 0x00000005;
}

// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)
void SysTick_Wait(unsigned long delay) {
	// Number of counts to wait
  NVIC_ST_RELOAD_R = delay-1;
	// Any value written to CURRENT clears
  NVIC_ST_CURRENT_R = 0;
	// Wait for count flag
  while((NVIC_ST_CTRL_R & 0x00010000) == 0);
}

// 10000us equals 10ms
void SysTick_Wait10ms(unsigned long delay){
  unsigned long i;
	
  for (i = 0; i < delay; i++) {
    SysTick_Wait(800000);  // wait 10ms
  }
}
