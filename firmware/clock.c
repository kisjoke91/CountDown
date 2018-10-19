/*******************************************************************************
 
 Author:      Chaos Developing
 Date:        2018-07-23
 Compiler:    avr-gcc
 Info:        Clock
 
 *******************************************************************************/


#include "lcd.h"
#include "clock.h"
#include "keys.h"

unsigned long int seconds_reaming;

/******************************************************************************/

// Inicializálja a belső Timert, előállít pontosan 1s-ot 8 MHz esetén!
void clock_init()
{
    TCCR1B |= (1 << WGM12);
    TIMSK |= (1 << OCIE1A);
    OCR1A = 31249;
    TCCR1B |= (1 << CS12);
    
    seconds_reaming = 0;
}

/******************************************************************************/

unsigned long int clock_get()
{
    return seconds_reaming;
}

/******************************************************************************/

void clock_set(unsigned long int timer)
{
    cli();
    seconds_reaming = timer;
    sei();
}


/******************************************************************************/


void clock_stop(void)
{
    cli();
    key_clearAlarm();
}

/******************************************************************************/

ISR(TIMER1_COMPA_vect)
{
    if (seconds_reaming == 0)
    {
        cli();
        key_setAlarm();
    }
    else seconds_reaming --;
}

