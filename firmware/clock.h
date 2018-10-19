/*******************************************************************************

Author:		Chaos Developing
Date:		2018-07-23
Compiler:	avr-gcc
Info:		keypad driver

*******************************************************************************/

#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define CLOCK_HOUR_S    3600
#define CLOCK_MIN_S    60

////////////////////////////////////////////////////////////////////////////////

// Inicializálja a belső Timert, előállít pontosan 1s-ot.
void clock_init();

////////////////////////////////////////////////////////////////////////////////

void clock_set(unsigned long int timer);

////////////////////////////////////////////////////////////////////////////////

void clock_stop(void);

////////////////////////////////////////////////////////////////////////////////

unsigned long int clock_get();

////////////////////////////////////////////////////////////////////////////////

#endif
