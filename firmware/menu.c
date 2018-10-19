/*******************************************************************************
 
 Author:      Chaos Developing
 Date:        2018-07-23
 Compiler:    avr-gcc
 Info:        Menu controller
 
 *******************************************************************************/

#include "lcd.h"
#include "menu.h"
#include "clock.h"
#include "keys.h"

/******************************************************************************/

void menu_init(void)
{
    clock_init();    // Setup Clock
    lcd_init();        // Initialize LCD
    key_init();     // Initialize IO-Ports
}

/******************************************************************************/

void menu_print()
{
    lcd_clear();
    lcd_setcursor(1,5);
    lcd_string("KB 31/1");
    lcd_setcursor(2,1);
    lcd_string("Z");lcd_data(225);lcd_string("hler starten");
    lcd_setcursor(3,1);
    lcd_string("Lampentest");
    lcd_setcursor(4,1);
    lcd_string("Hilfe");
}

/******************************************************************************/

void menu_printcursor(uint8_t selected_item)
{
    for(int i = 0; i < MENU_POINTS; i ++)
    {
        lcd_clearchar(i + 2, 0);
    }
    
    if (selected_item > 2 || selected_item < 0) selected_item = 0;
    
    lcd_setcursor(selected_item + 2, 0);
    lcd_data(126);
}

/******************************************************************************/

void menu_help()
{
    lcd_clear();
    lcd_setcursor(1,0);
    lcd_string("Dr");lcd_data(5);lcd_string("cken sie die");
    lcd_setcursor(2,0);
    lcd_string("taster UP oder");
    lcd_setcursor(3,0);
    lcd_string("DOWN um ein Wert");
    lcd_setcursor(4,0);
    lcd_string("zu ver"); lcd_data(225); lcd_string("ndern.");
    _delay_ms(4000);
    
    lcd_clear();
    lcd_setcursor(1,0);
    lcd_string("Dr");lcd_data(5);lcd_string("cken sie OK");
    lcd_setcursor(2,0);
    lcd_string("um ein Wert zu");
    lcd_setcursor(3,0);
    lcd_string("best"); lcd_data(225); lcd_string("tigen, oder");
    lcd_setcursor(4,0);
    lcd_string("ESC zum Abbruch");
    _delay_ms(4000);
    
    lcd_clear();
    lcd_setcursor(1,0);
    lcd_string("SGD Kipfenberg");
    lcd_setcursor(2,0);
    lcd_string("SW: ver.: 1.1");
    lcd_setcursor(3,0);
    lcd_string("Build: 28082018");
    lcd_setcursor(4,0);
    lcd_string("E-Werkstatt :)");
    _delay_ms(2000);
}

/******************************************************************************/

void menu_printtime(unsigned long int secs)
{
    unsigned long int hours = secs / 3600;
    secs -= hours * 3600;
    unsigned long int mins = secs / 60;
    secs -= mins * 60;
    
    for (uint8_t cursor_pos = 4; cursor_pos <= 10; cursor_pos += 3)
    {
        uint8_t value;
        
        lcd_setcursor(2,cursor_pos);
        switch (cursor_pos)
        {
            case 4:
                value = hours;
                break;
            case 7:
                value = mins;
                break;
            case 10:
                value = secs;
                break;
        }
        
        if (value > 9) lcd_decimal(value);
        else
        {
            lcd_data('0');
            lcd_decimal(value);
        }
        if (cursor_pos < 10)lcd_data(':');
    }
}

/******************************************************************************/

unsigned long int menu_setvalue() // what: 3600 hours, 60 mins, 1 secs
{
    unsigned long int timer = 0;
    uint8_t ok_pressed = 0;
    uint8_t cursor_pos = 0;
    int what = CLOCK_HOUR_S;
    
    do
    {
        ok_pressed = 0;
        switch (what) {
            case CLOCK_HOUR_S:
                cursor_pos = 5;
                break;
                
            case CLOCK_MIN_S:
                cursor_pos = 8;
                break;
                
            case 1:
                cursor_pos = 11;
                break;
        }
        
        lcd_command( LCD_SET_DISPLAY | LCD_DISPLAY_ON | LCD_CURSOR_ON | LCD_BLINKING_ON);
        
        while(!ok_pressed)
        {
            menu_printtime(timer);
            lcd_setcursor(2,cursor_pos);
            
            switch (key_get())
            {
                case DOWN:
                    if(timer > 0) timer -= what;
                    break;
                    
                case UP:
                    timer += what;
                    break;
                    
                case OK:
                    ok_pressed = 1;
                    break;
                    
                case ESC:
                    lcd_command( LCD_SET_DISPLAY | LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINKING_OFF);
                    return 0;
                    break;
                    
                case NOPE:
                    break;
            }
        }
        
        lcd_command( LCD_SET_DISPLAY | LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINKING_OFF);
        
        what /= 60;
    }
    while(what);
    
    return timer;
}

/******************************************************************************/

void menu_countdown()
{
    lcd_clear();
    lcd_setcursor(1,3);
    lcd_string("Count down");
    lcd_setcursor(4,1);
    lcd_string("ESC: abbrechen");
    unsigned long int i = menu_setvalue();
    
    if (i) clock_set(i);
    else
    {
        clock_stop();
        return;
    }
    
    
    
    for (Key k = NOPE;; k = key_get())
    {
        if (k == ESC)
        {
            clock_stop();
            return;
        }
        i = clock_get();
        menu_printtime(i);
        if (!i) break;
    }
    
    lcd_setcursor(3,3);
    lcd_string("Zum Beenden");
    lcd_setcursor(4,0);
    lcd_string("OK lang dr");lcd_data(5);lcd_string("cken");
    clock_stop();
    key_setAlarm();
    
    for (Key k = NOPE; k != OK; k = key_get())
    {
        lcd_setcursor(1,0);
        lcd_string("ZEIT IST AUS !!!");
        _delay_ms(500);
        
        lcd_clearrow(1,0);
        _delay_ms(400);
    }
    
    clock_stop();
    key_clearAlarm();
    
    while(key_get() == OK);
}

/******************************************************************************/

void menu_lamptest(void)
{
    lcd_clear();
    lcd_setcursor(1,3);
    lcd_string("Lampentest");
    
    for(uint8_t i = 4; i > 0; i --)
    {
        lcd_setcursor(3, 0);
        lcd_string("schliesst in ");
        lcd_decimal(i - 1);
        lcd_string(" sec...");
        key_setAlarm();
        _delay_ms(500);
        key_clearAlarm();
        _delay_ms(500);
    }
}

/******************************************************************************/

void menu_sub(uint8_t selected_item)
{
    switch (selected_item)
    {
        case 0:
            menu_countdown();
            break;
            
        case 1:
            menu_lamptest();
            break;
            
        case 2:
            menu_help();
            break;
    }
    
    menu_print();
    menu_printcursor(selected_item);
}

/******************************************************************************/

void menu_run(void)
{
    uint8_t selected_item = 0;
    menu_print();
    menu_printcursor(selected_item);
    
    while (1)
    {
        switch (key_get())
        {
            case DOWN:
                if(selected_item < MENU_POINTS - 1) selected_item ++;
                menu_printcursor(selected_item);
                break;
                
            case UP:
                if(selected_item > 0) selected_item --;
                menu_printcursor(selected_item);
                break;
                
            case OK:
                menu_sub(selected_item);
                break;
                
            case ESC:
                break;
                
            case NOPE:
                break;
        }
    }
}

