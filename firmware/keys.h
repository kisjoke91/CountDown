/*******************************************************************************

Author:		Chaos Developing
Date:		2018-07-23
Compiler:	avr-gcc
Info:		Keypad driver

*******************************************************************************/

#ifndef _KEYS_H_
#define _KEYS_H_

#include <avr/io.h>
#include <util/delay.h>

// Applied PORT for the key inputs and alarm output
#define KEY_IO_PORT		PORTB
#define KEY_IO_DDR		DDRB
#define KEY_IO_PIN		PINB

// Definition of input pins
#define	KEY_ESC			1
#define	KEY_DOWN		2
#define	KEY_UP		    3
#define KEY_OK			4

// key_get() return values
typedef enum Key
{
    NOPE,
    ESC,
    DOWN,
    UP,
    OK
}Key;

// defines the deprell time intervall
#define KEY_PRELL_MS	250

// definition of alarm output pins
#define ALARM_OUT		0


//=============================== Handle routins ================================

// Sets inputs and outputs up
void key_init(void);

// polls the input and returns the code of pressed key
Key key_get(void);

// Sets the alarm output active
void key_setAlarm(void);

// Clears the alarm output
void key_clearAlaram(void);

#endif
