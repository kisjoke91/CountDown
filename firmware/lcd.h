/*******************************************************************************

Author:		Chaos Developing
Date:		2018-07-23
Compiler:	avr-gcc
Info:		keypad driver

*******************************************************************************/

#ifndef _LCD_H_
#define _LCD_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

//============ A vezérlő mikrokontroller órajele Hz-ben megadva ================
#ifndef F_CPU
#define F_CPU 8000000
#endif


/*========================= Az LCD lábkiosztása ================================
A kijelző minden lába ugyanarra a portra legyen kötve, és a
4 adat vezeték egymástkövető sorrendben, egymásmellett legyenek.
*/
//  LCD DB4-DB7 <-->  PORTD Bit PD0-PD3
#define LCD_PORT      PORTD
#define LCD_DDR       DDRD
#define LCD_DB        PD2

//  LCD RS      <-->  PORTD Bit PD4     (RS: 1=Data, 0=Command)
#define LCD_RS        PD0

//  LCD EN      <-->  PORTD Bit PD5     (EN: 1-Impuls for Data)
#define LCD_EN        PD1


//========= Futási idők: MS = miliszekundum, US = mikroszekundum) ==============

#define LCD_BOOTUP_MS           15
#define LCD_ENABLE_US           20
#define LCD_WRITEDATA_US        46
#define LCD_COMMAND_US          42

#define LCD_SOFT_RESET_MS1      5
#define LCD_SOFT_RESET_MS2      1
#define LCD_SOFT_RESET_MS3      1
#define LCD_SET_4BITMODE_MS     5

#define LCD_CLEAR_DISPLAY_MS    2
#define LCD_CURSOR_HOME_MS      2


/*====================== Az LCD sorainak definiálása ===========================
Az itt megadott értékek 4 X 16 karatkeres kijelző esetén érvényesek.
Más kijelző méret esetén a mérethez tartozó értékek kerülnek ide
*/

#define LCD_DDADR_LINE1         0x00
#define LCD_DDADR_LINE2         0x40
#define LCD_DDADR_LINE3         0x10
#define LCD_DDADR_LINE4         0x50


//=============================== Kezelő rutinok ===============================

// LCD inicializálása. Az LCD használata előtt egyszer le kell futtatni
void lcd_init( void );


// kijelző tartalmának törlése
void lcd_clear( void );


// Kurzor a bal felső sarokba ugrik
void lcd_home( void );


// Kurzor tetszőleges pozícióba ugrik
void lcd_setcursor( uint8_t row, uint8_t column );


// Egyetlen karakter kiíratása az aktuális kurzor pozícióba
void lcd_data( uint8_t data );


// Karakterlánc kiíratása az aktuális kurzorpozíciótól kezdve
void lcd_string( const char *data );


/*  Egyéni karakter definiálása
	A data egy tömbre[7] kell mutasson, ami definiálja a karaktert
*/
void lcd_generatechar( uint8_t code, const uint8_t *data );


// Elküld egy parancsot az LCD -nek
void lcd_command( uint8_t data );


// töröl egy sort az adott oszloptól
void lcd_clearrow(unsigned char row, unsigned char column);


// Kitöröl egy karaktert a megadot pozición
void lcd_clearchar(unsigned char row, unsigned char column);


// Kiír egy decimális számot a kijelzőre
void lcd_decimal(long int number);


//=============== LCD utasítások. Az lcd_command argumentumai ==================

// Kijelző törlése -------------- 0b00000001
#define LCD_CLEAR_DISPLAY       0x01

// Kurzor bal felső sarokba------ 0b0000001x
#define LCD_CURSOR_HOME         0x02

// Beviteli mód beállítása ------ 0b000001xx
#define LCD_SET_ENTRY           0x04

#define LCD_ENTRY_DECREASE      0x00
#define LCD_ENTRY_INCREASE      0x02
#define LCD_ENTRY_NOSHIFT       0x00
#define LCD_ENTRY_SHIFT         0x01

// Kijelző beállítása ----------- 0b00001xxx
#define LCD_SET_DISPLAY         0x08

#define LCD_DISPLAY_OFF         0x00
#define LCD_DISPLAY_ON          0x04
#define LCD_CURSOR_OFF          0x00
#define LCD_CURSOR_ON           0x02
#define LCD_BLINKING_OFF        0x00
#define LCD_BLINKING_ON         0x01

// Eltolás beállítása ----------- 0b0001xxxx
#define LCD_SET_SHIFT           0x10

#define LCD_CURSOR_MOVE         0x00
#define LCD_DISPLAY_SHIFT       0x08
#define LCD_SHIFT_LEFT          0x00
#define LCD_SHIFT_RIGHT         0x04

// Működés beállítása ----------- 0b001xxxxx
#define LCD_SET_FUNCTION        0x20

#define LCD_FUNCTION_4BIT       0x00
#define LCD_FUNCTION_8BIT       0x10
#define LCD_FUNCTION_1LINE      0x00
#define LCD_FUNCTION_2LINE      0x08
#define LCD_FUNCTION_5X7        0x00
#define LCD_FUNCTION_5X10       0x04

#define LCD_SOFT_RESET          0x30

// CG RAM Address beállítása ---- 0b01xxxxxx  (Character Generator RAM)
#define LCD_SET_CGADR           0x40

#define LCD_GC_CHAR0            0
#define LCD_GC_CHAR1            1
#define LCD_GC_CHAR2            2
#define LCD_GC_CHAR3            3
#define LCD_GC_CHAR4            4
#define LCD_GC_CHAR5            5
#define LCD_GC_CHAR6            6
#define LCD_GC_CHAR7            7

// DD RAM Addressbeállítása------ 0b1xxxxxxx  (Display Data RAM)
#define LCD_SET_DDADR           0x80

#endif
