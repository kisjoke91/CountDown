/*******************************************************************************
 
 Author:      Chaos Developing
 Date:        2018-07-23
 Compiler:    avr-gcc
 Info:        keypad driver
 
 *******************************************************************************/

#include "keys.h"

//=========================== initialize io system =============================
void key_init(void)
{
    // Clearing the data direction resgister
    KEY_IO_DDR = 0x00;
    
    // Setting the alarm output pin
    KEY_IO_DDR |= (1 << ALARM_OUT);
    
    // Sets the inputs up
    KEY_IO_DDR &= ~((1 << KEY_ESC) |
                    (1 << KEY_DOWN) |
                    (1 << KEY_UP) |
                    (1 << KEY_OK));
    
    KEY_IO_PORT |= ((1 << KEY_ESC) |
                    (1 << KEY_DOWN) |
                    (1 << KEY_UP) |
                    (1 << KEY_OK));
    
    KEY_IO_PORT &= ~(1 << ALARM_OUT);
}

//======================= Checks if a button is pressed ========================
Key key_get(void)
{
    Key ret = NOPE;
    
    if (!(KEY_IO_PIN & (1 << KEY_ESC))) ret = ESC;
    else if (!(KEY_IO_PIN & (1 << KEY_DOWN))) ret = DOWN;
    else if (!(KEY_IO_PIN & (1 << KEY_UP))) ret = UP;
    else if (!(KEY_IO_PIN & (1 << KEY_OK))) ret = OK;
    
    // Deprelling of the input
    _delay_ms(KEY_PRELL_MS);
    
    return ret;
}


//============================ Puts alarm relay on =============================
void key_setAlarm(void)
{
    KEY_IO_PORT |= (1 << ALARM_OUT);
}


//============================ Puts alarm relay off ============================
void key_clearAlarm(void)
{
    KEY_IO_PORT &= ~(1 << ALARM_OUT);
}

