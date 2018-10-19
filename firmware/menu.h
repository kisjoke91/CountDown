/*******************************************************************************

Author:		Chaos Developing
Date:		2018-07-23
Compiler:	avr-gcc
Info:		keypad driver

*******************************************************************************/

#ifndef _MENU_H_
#define _MENU_H_

#define MENU_POINTS     3

void menu_init(void);
void menu_run();
void menu_printtime(unsigned long int secs);

#endif
