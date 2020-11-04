/*
 * main.c
 *
 *  Created on: Mar 21, 2020
 *      Author: Eeshwar
 */
#include "HPS_Watchdog/HPS_Watchdog.h"
#include "Timer11/Timer11.h"
#include "Declare/Declare.h"    // All the declared variables are present in this header
#include <stdlib.h>

void exitOnFail(signed int status, signed int successStatus)
{
    if (status != successStatus)
    {
        exit((int)status);
    }
}

void initializeVariables(void)   // This function used for initialize the values of variables
{
	for (k = 0; k < 6; k++)
	{
		HexDisp[k] = 0;          // making the display variables to zero, This variable is used to display the values on the 7-Segment display
	}
	for (k = 0; k < 5; k++)
	{
		taskLastTime[k] = 0;
	}

}

void initialFinal (void)
{
	for (k = 0; k <= 3; k++)
		{
			InitialValue[k] = 0;    // this variable is used to store the value of
			FinalValue[k] = 0;      // this variable will store the alarm value set by the user
		}
}

// initialize the display to value Zero, So when this function is called all 7-Segments display number zero.
void initialize(void)
{
	volatile unsigned int *display03 = (unsigned int*) 0xFF200020;
	volatile unsigned int *display45 = (unsigned int*) 0xFF200030;
	*display03 = (0x3F << 0) | (0x3F << 8) | (0x3F << 16) | (0x3F << 24) ;
	*display45 = (0x3F << 0) | (0x3F << 8);

}

// this function is used for displaying the different values on the 7-Segment display

void displayHex (int a, int b, int n)

{
	volatile unsigned int *display03 = (unsigned int*) 0xFF200020; // Can access only first four 7-Segment Displays on board
	volatile unsigned int *display45 = (unsigned int*) 0xFF200030; // Can access the Last two 7-Segment Displays
	unsigned int value;

	if (n == 0)

	{
		value = *display03;
		value &= ~(0x7F << b);
		value |= (a << b);
		*display03 = value;
	}

	if (n == 1)

	{
		value = *display45;
	    value &= ~(0x7F << b);
		value |= (a << b);
		*display45 = value;

	}
}


void KeyFunction (int K, int K12)    // here K is a flag
{
	unsigned int j;

	if (K == 0)

	{
		K12 += 1;    // this function is used to count number of times the user had pressed the button, Once the Switch is turned on then this statement is used
	}

	if (K == 1)
	{
		for (j = 0; j <= 3; j++)
		{
			InitialValue[j] = HexDisp[j+2] - 1; // this would store the initial values till the alarm switch is turned on. Once the Switch is turned on then this statement is not in use
		}
	}
}

// This function is used to initialize different variables, this function is useful for setting up and switching of Alarm
void AlarmVariables (int x1, int x2, int x3, int x4)
{
	Alarm = x1;
	key   = x2;
	snooze = x3;
	*LEDR_ptr = x4;

}

int main(void)
{
	*LEDR_ptr = 0x0; // Making all Leds on Board in Off state
	initialFinal();

	for (i = 0 ; i < 5; i++)

	{
	    taskLastTime[i] = Timer_counterread();
	    taskinterval[i] = 1125000 * nosec[i];    // Initialize different Interval values
	 }

	load = 0xFFFFFFFF;
	exitOnFail (Timer_intialise(base_add),TIMER_SUCCESS);
	        exitOnFail (Timer_load(load),TIMER_SUCCESS);
	        exitOnFail (Timer_Prescaler(prescaler),TIMER_SUCCESS);
	        exitOnFail (Timer_Control(control),TIMER_SUCCESS);
	        ResetWDT();

    initialize();

    while(1)

    {
    	signed int currentTimerValue = Timer_counterread();

   //***************** MODE 0 Standard Mode *************************************

    	if ((taskLastTime[0] - currentTimerValue) >= taskinterval[0]) // This Task Scheduler is True for Every Second, And useful for displaying the Seconds of the digital clock, (task 1 for Mode 0)

    	{

    	if (HexDisp[0] == 10)   // To Check if the units of Seconds counter is above 9, It initialize the variable HexDisp[0](first 7-Segment Display) to zero and also display 0 after it reaches 9.

    	{
    	    HexDisp[0] = 0; // Initialize the variable to zero when it is above 9


    	    displayHex(display[HexDisp[1]],8,0); // it will display the tens digit of the Seconds hand of the digital clock, here 8 Represents the amount shift within
    	    HexDisp[1] += 1; // Increment the variable(tens digit of the Seconds Hand of the clock) for every 10 seconds.

    	 }

    	if (HexDisp[1] == 6) // To check if Tens digit of the seconds counter is above 5. It will initialize the variable to zero

    	   {
    	      HexDisp[1] = 0; //Initialize the variable to zero when it is above 5

    	   }

    	 displayHex(display[HexDisp[0]],0,0);   // displayHex: it is a function Displays the value in the 7-Segment, display(): array has the list of hex values,
    	                                        // HexDisp(): it defines which element in the display array to be displayed.

    	 taskLastTime[0] = taskLastTime[0] - taskinterval[0]; // Update the taskLastTime0
    	 HexDisp[0] += 1; // Increment the variable for every second

    	 if (snooze == 1)  // This Conditional Statement is specifically used when Alarm Switch is turned On

    	    {
    	     	n += 1;
    	     }
          }

    	if (((taskLastTime[1] - currentTimerValue) >= taskinterval[1]))  // This task is activated For Every 60 Seconds, as 1 min = 60 seconds.  (task 2 for Mode 0)

    	          {

    				 if (HexDisp[2] == 10)     // to check if the units of the Minutes counter is above 9, HexDisp[2] is used for displaying digit in the third 7-segment display

    				 {
    					 HexDisp[2] = 0;  //initialize to zero,

    					 displayHex(display[HexDisp[3]],24,0);   // display function, it displays tens place of Minutes, fourth 7-segment display
    					 HexDisp[3] += 1; // increment tens position Minutes counter for every 600 seconds
    				 }

    				 if (HexDisp[3] == 6) // to check if the tens of the Minutes counter is above 5, HexDisp[3] is used for displaying digit in the fourth 7-segment display

    				 {
    					 HexDisp[3] = 0; // Initialize to zero once it is greater than 5
    				 }

    				 displayHex(display[HexDisp[2]],16,0); // display function, it displays units place of Minutes, third 7-segment display

    				 taskLastTime[1] = taskLastTime[1] - taskinterval[1]; // update the taskLastTime1
    				 HexDisp[2] += 1;  // units place of Minutes positIncrement the variable for every 60 seconds


    			 }

    			 if ((taskLastTime[2] - currentTimerValue) >= taskinterval[2]) // This task is activated For Every 3600 Seconds, as 60 min = 3600 seconds.  (task 3 for Mode 0)
    			   {
    				 if (HexDisp[4] == 10)  // to check if the units of the hours counter is above 9, HexDisp[4] is used for displaying digit in the fifth 7-segment display

    				 {

    					 HexDisp[4] = 0;   // initialize to zero after reaches to 9

    					 displayHex(display[HexDisp[5]],8,1); // display function, it displays tens place of hours, sixth 7-segment display
    					 HexDisp[5] += 1;  // increment the variable
    				 }

    				 if (HexDisp[5] == 3 && HexDisp[4] == 4)  // to check when the timer goes beyond 23:59:59

    				 {
    					 initializeVariables(); // initialize all the variables
    					 initialize(); // initialize the display
    				 }

    				 displayHex(display[HexDisp[4]],0,1); // display function, it displays units place of hours, fifth 7-segment display

    				 taskLastTime[2] = taskLastTime[2] - taskinterval[2]; // update the count
    				 HexDisp[4] += 1;

    			 }

// *********************** MODE 1 Time Edit MODE *********************************************

    			 // To Avoid Debouncing of the switch
    			 if ((*pushbutton & 0x01) && flag[3] == 1)

    			    {
    				    flag[0] = 1;

    			    }

    			 if ((*pushbutton & 0x02) && flag[3] == 1)

    			     {
    			     	 flag[1] = 1;

    			      }
                 // This function is for PushButton0 , To change the Minutes
    			    if (flag[0] && !(*pushbutton & 0x01))

    			 	     {
    				             flag[0] = 0;

    			 				 if (HexDisp[2] == 10) // to check if the units of the Minutes counter is above 9, HexDisp[2] is used for displaying digit in the third 7-segment display

    			 				 {

    			 				 HexDisp[2] = 0; // Initialize to zero once it is greater than 9

    			 				 displayHex(display[HexDisp[3]],24,0);
    			 				 HexDisp[3] += 1;

    			 				 }

    			 				 if (HexDisp[3] == 6) // to check if the tens of the Minutes counter is above 5, HexDisp[3] is used for displaying digit in the fourth 7-segment display

    			 				 {
    			 				     HexDisp[3] = 0; // Initialize to zero once it is greater than 5

    			 				 }

    			 				 displayHex(display[HexDisp[2]],16,0);

    			 				 taskLastTime[2] += taskinterval[1]; // hour time is incremented whenever pushbutton0 is pressed

    			 				 HexDisp[2] += 1;

    			 				 KeyFunction (key,key0); // this function has dual functionality and used for storing the current time as initial variable.
    			 				                         // other functionality is that when alarm is ON, it would count number of times the user pressed the button
    			 		 }

               // This function is for PushButton1, To change the Hours
    			   if (flag[1] && !(*pushbutton & 0x02))

    			      {
    				    flag[1] = 0;

    			 	    if (HexDisp[4] == 10) // to check if the units of the hours is above 9, HexDisp[4] is used for displaying digit in the fifth 7-segment display

    			 	       {
    			 	    	HexDisp[4] = 0;
    			 	        displayHex(display[HexDisp[5]],8,1); // display function, it displays tens place of hours, sixth 7-segment display
    			 		    HexDisp[5] += 1;

    			 		    }

    			 	   if (HexDisp[5] == 3 && HexDisp[4] == 4)

    			 		 		 {
    			 		 			HexDisp[4] = 0;
    			 		 			HexDisp[5] = 0;

    			 		 			displayHex(display[HexDisp[5]],8,1); // display function, it displays tens place of hours, sixth 7-segment display
    			 		 			HexDisp[5] += 1;
                                 }

    			 		 		 displayHex(display[HexDisp[4]],0,1); // display function, it displays units place of hours, fifth 7-segment display

    			 		 		 HexDisp[4] += 1; // increment the value

    			 		 		 KeyFunction (key,key1); // this function has dual functionality and used for storing the current time as initial variable.
			                         // other functionality is that when alarm is ON, it would count number of times the user pressed the button
    			 		 }

// ************************************** MODE 2 Alarm Mode *************************************//

    			   if (*SlideSwitch && 0x01)

    			      {

    			      	key = 0;

    			        if (Alarm == 1 && HexDisp[2] == FinalValue[0] && HexDisp[3] == FinalValue[1] && HexDisp[4] == FinalValue[2] && HexDisp[5] == FinalValue[3]) // to check if the current time is equal to
    			        	// the set time

    			      	   {
    			             AlarmVariables (0,1,1,0xFF); // function call to initialize the values of the variables, alarm = 0, key =1 ,snooze = 1, Led On
    			      	   }
    			      }

    			  if (!(*SlideSwitch && 0x01))

    			     {

    			       AlarmVariables (0,1,0,0x00); // function call to initialize the values of the variables, alarm = 0, key =1 ,snooze = 0, Led Off
    			       KeyFunction (key,key1); // In this case this function will help to store the initial value imediatly t
    			       n = 0;
    			       flag[3] = 1;  // This flag is used when alarm function is set, it make sure that Standard mode and snooze mode function properly. Once the alarm is set then Mode 1 doesnot function

    			      }

    			  if ((*pushbutton & 0x0C) && (*SlideSwitch && 0x01) && flag[3] == 1)

    			      {
    			      	flag[2] = 1;

    			      	}


    			   if (flag[2] && !(*pushbutton & 0x0C) )

    			           {
    				             flag[2] = 0;
                                 flag[3] = 0; // initialize to zero, By this variable mode 1 is not accessable until the switch is deactivated

    			 			     for (i = key0; i > 0; i--)
    			 				  {
    			 					 taskLastTime[2] = taskLastTime[2] - taskinterval[1]; // to reduce the time interval of hour , depending on number of times the pushbutton0 is changed
    			 				  }

    			 				  key0 = 0;

    			 				  Alarm = 1;

    			 				  for (i = 0; i <= 3; i++)

    			 				  {
    			 					 FinalValue[i] = HexDisp[i + 2];    // Alarm value is stored in a variable call FinalValue
    			 					 HexDisp[i + 2] = InitialValue [i]; // initialize the value before setting up the alarm to HexDisp
    			 				  }
                                 // display the current time
    			 				 displayHex(display[HexDisp[2]],16,0);
    			 				 displayHex(display[HexDisp[3]],24,0);
    			 				 displayHex(display[HexDisp[4]],0,1);
    			 				 displayHex(display[HexDisp[5]],8,1);

    			 				 for (i = 0; i <= 3; i ++)

    			 				 {
    			 					HexDisp[i + 2] = InitialValue [i] + 1;
    			 				 }
    			 			}

  //******************************* Mode 3 Alarm Snooze ***********************************//

                if (((taskLastTime[3] - currentTimerValue) >= taskinterval[3]) && n>9) // n variable is used to avoid congestion between this task and when the alarm is set(it acts like a delay only for first call)
                                                                                       // it checks for every 10 secs
                   {
                    	AlarmVariables (0,1,1,0x00);                           // turn off the led
                    	taskLastTime[3] = taskLastTime[3] - taskinterval[3];

                   }

                if (((taskLastTime[4] - currentTimerValue) >= taskinterval[4]) && snooze == 1) // Snnoze for every 5 min

                    {
                    	AlarmVariables (0,1,1,0xFF);    // turn on the led
                    	taskLastTime[4] = taskLastTime[4] - taskinterval[4];
                    }
//*****************************************************************************************//
    	Timer_interuppt(); // we clear the private timer interrupt flag if it is set, hence this function is used
    	ResetWDT();       // Reset Watchdog Timer
     }
}
