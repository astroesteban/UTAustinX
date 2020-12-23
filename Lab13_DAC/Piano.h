#ifndef PIANO_H
#define PIANO_H
// Piano.h
// Runs on LM4F120 or TM4C123,
// edX lab 13
// There are four keys in the piano
// Daniel Valvano, Jonathan Valvano
// December 29, 2014

#define C_PERIOD  4790U  // 80Mz / (523.251Hz * len (wave_table))
#define D_PERIOD  4256U 
#define E_PERIOD  3792U 
#define G_PERIOD 	3189U 

#define SILENCE	0x00
#define C_NOTE	0x01
#define D_NOTE	0x02
#define E_NOTE	0x04
#define G_NOTE 	0x08

// **************Piano_Init*********************
// Initialize piano key inputs
// Input: none
// Output: none
void Piano_Init(void);

// **************Piano_In*********************
// Input from piano key inputs
// Input: none
// Output: 0 to 15 depending on keys
// 0x01 is key 0 pressed, 0x02 is key 1 pressed,
// 0x04 is key 2 pressed, 0x08 is key 3 pressed
unsigned long Piano_In(void);

#endif
