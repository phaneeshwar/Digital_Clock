/*
 * Timer11.h
 *
 *  Created on: Mar 2, 2020
 *      Author: Eeshwar
 */

#ifndef TIMER11_TIMER11_H_
#define TIMER11_TIMER11_H_

#include <stdbool.h>

#define TIMER_SUCCESS 0
#define TIMER_ERRORNOINT -1
#define INTERUPPT_HIGH 1
#define INTERUPPT_LOW 2

signed int Timer_intialise (unsigned int base_address);
bool Timer_isIntialised(void);

signed int Timer_load (unsigned int load);
signed int Timer_Prescaler (unsigned int prescaler);
signed int Timer_Control (unsigned int control);
signed int Timer_counterread (void);
signed int Timer_interuppt(void );
//signed int Timer_read_interuppt(void );
//signed int Timer_clear_interuppt(void);


#endif /* TIMER11_TIMER11_H_ */




