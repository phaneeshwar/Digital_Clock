/*
 * Declare.h
 *
 *  Created on: Mar 7, 2020
 *      Author: Eeshwar
 */

#ifndef DECLARE_H_
#define DECLARE_H_
// Variables declared for timer Operations
volatile unsigned int base_add = 0xFFFEC600;
volatile unsigned int load = 0x00;
volatile unsigned int control = 0x0003;
volatile unsigned int prescaler = 0xC7;
unsigned int timer_interrupt = 0;
// Declarations of other peripherals
volatile unsigned int *LEDR_ptr = (unsigned int *) 0xFF200000;
volatile unsigned int *pushbutton = (unsigned int *) 0xFF200050;
volatile unsigned int *SlideSwitch = (unsigned int *) 0xFF200040;
// Declarations of intermediate
unsigned int currentTimerValue;
unsigned int i = 0, k = 0, n = 0, key = 0, key0 = 0, key1 = 0, Alarm = 0, snooze = 0;
unsigned int taskLastTime[5];
unsigned int taskinterval[5];
unsigned int flag[4] = {0,0,0,0};
unsigned int nosec[5] = {1,60,3600,10,300}; // 1 sec,60sec,3600sec, 10sec to switch off the leds, 300 sec to snooze the time
unsigned int HexDisp[6] = {1,1,1,1,1,1};
unsigned int InitialValue[5];
unsigned int FinalValue[5];
unsigned int display[10] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0X6D, 0x7D, 0x07, 0x7F, 0x6F}; // Array of Hex values ranging from 0 to 9


#endif /* DECLARE_H_ */



