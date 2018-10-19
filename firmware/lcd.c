/*******************************************************************************

Author:		Chaos Developing
Date:		2018-07-23
Compiler:	avr-gcc
Info:		keypad driver

*******************************************************************************/

#include "lcd.h"


//============================== EN jel kiadása ================================
static void lcd_enable()
{
    LCD_PORT |= (1<<LCD_EN);     // Enable értéke 1
    _delay_us( LCD_ENABLE_US );  // rövid szünet
    LCD_PORT &= ~(1<<LCD_EN);    // Enable törlése
}


//========================== 4 bites kimeneti művelet ==========================
static void lcd_out( uint8_t data )
{
    data &= 0xF0;                       // felső 4 Bit maszkolása

    LCD_PORT &= ~(0xF0>>(4-LCD_DB));    // maszk törlés
    LCD_PORT |= (data>>(4-LCD_DB));     // Bitek értékadása
    lcd_enable();
}

//==== LCD inicializálása. Az LCD használata előtt egyszer le kell futtatni ====
void lcd_init( void )
{
    // Használt PIN-ek kimenetre állítása
    uint8_t pins = (0x0F << LCD_DB) |           // 4 adatvezeték
                   (1<<LCD_RS) |                // R/S vezeték
                   (1<<LCD_EN);                 // Enable vezeték
    LCD_DDR |= pins;

    // Minden kimenet 0-ra állítása
    LCD_PORT &= ~pins;

    // Vár az LCD feléledésére
    _delay_ms( LCD_BOOTUP_MS );

    // Soft-Reset 3x egymás után.
    lcd_out( LCD_SOFT_RESET );
    _delay_ms( LCD_SOFT_RESET_MS1 );

    lcd_enable();
    _delay_ms( LCD_SOFT_RESET_MS2 );

    lcd_enable();
    _delay_ms( LCD_SOFT_RESET_MS3 );

    // 4-bites mód kiválasztása
    lcd_out( LCD_SET_FUNCTION |
             LCD_FUNCTION_4BIT );
    _delay_ms( LCD_SET_4BITMODE_MS );

    // 4-bites mód / 2 sor / 5x7
    lcd_command( LCD_SET_FUNCTION |
                 LCD_FUNCTION_4BIT |
                 LCD_FUNCTION_2LINE |
                 LCD_FUNCTION_5X7 );

    // Kijelző be / Kurzor ki / Villogás ki
    lcd_command( LCD_SET_DISPLAY |
                 LCD_DISPLAY_ON |
                 LCD_CURSOR_OFF |
                 LCD_BLINKING_OFF);

    // Kurzor növekvő / Nincs görgetés
    lcd_command( LCD_SET_ENTRY |
                 LCD_ENTRY_INCREASE |
                 LCD_ENTRY_NOSHIFT );
 	// Kijelző törlése
    lcd_clear();

    const uint8_t aa[] = {0x02,0x04,0x0E,0x01,0x0F,0x11,0x0F,0x00};    //á
    const uint8_t ee[] = {0x02,0x04,0x0E,0x11,0x1F,0x10,0x0E,0x00};    //é
    const uint8_t ii[] = {0x02,0x04,0x0C,0x04,0x04,0x04,0x0E,0x00};    //í
    const uint8_t oo[] = {0x02,0x04,0x0E,0x11,0x11,0x11,0x0E,0x00};    //ó
    const uint8_t uu[] = {0x02,0x04,0x11,0x11,0x11,0x13,0x0D,0x00};    //ú
    const uint8_t u2[] = {0x0A,0x00,0x11,0x11,0x11,0x13,0x0D,0x00};    //ü
    const uint8_t u3[] = {0x05,0x0A,0x11,0x11,0x11,0x13,0x0D,0x00};    //ű
    const uint8_t o2[] = {0x05,0x0A,0x0E,0x11,0x11,0x11,0x0E,0x00};    //ő

    lcd_generatechar(0, aa);
    lcd_generatechar(1, ee);
    lcd_generatechar(2, ii);
    lcd_generatechar(3, oo);
    lcd_generatechar(4, uu);
    lcd_generatechar(5, u2);
    lcd_generatechar(6, u3);
    lcd_generatechar(7, o2);

}


//============================= Egy adatbájt küldése ===========================
void lcd_data( uint8_t data )
{
    LCD_PORT |= (1<<LCD_RS);    // RS 1

    lcd_out( data );            // elsőzör a felső,
    lcd_out( data<<4 );         // majd az alsó 4 Bit küldése

    _delay_us( LCD_WRITEDATA_US );
    _delay_us( LCD_WRITEDATA_US );
}


//========================== Parancs küldése az LCD-re =========================
void lcd_command( uint8_t data )
{
    LCD_PORT &= ~(1<<LCD_RS);    // RS 0

    lcd_out( data );             // zuerst a felső,
    lcd_out( data<<4 );          // majd az alsó 4 Bit küldése

    _delay_us( LCD_COMMAND_US );
    _delay_us( LCD_COMMAND_US );
}


//========================== Kijelző tartalmának törlése =======================
void lcd_clear( void )
{
    lcd_command( LCD_CLEAR_DISPLAY );
    _delay_ms( LCD_CLEAR_DISPLAY_MS );
    _delay_ms( LCD_CLEAR_DISPLAY_MS );
}


//========================== Kurzor Home parancs küldése =======================
void lcd_home( void )
{
    lcd_command( LCD_CURSOR_HOME );
    _delay_ms( LCD_CURSOR_HOME_MS );
    _delay_ms( LCD_CURSOR_HOME_MS );
}


//====================== Kurzor tetszőleges pozícióba ugrik ====================
void lcd_setcursor( uint8_t row, uint8_t column )
{
    uint8_t data;

    switch (row)
    {
        case 1:    // 1. sor
            data = LCD_SET_DDADR + LCD_DDADR_LINE1 + column;
            break;

        case 2:    // 2. sor
            data = LCD_SET_DDADR + LCD_DDADR_LINE2 + column;
            break;

        case 3:    // 3. sor
            data = LCD_SET_DDADR + LCD_DDADR_LINE3 + column;
            break;

        case 4:    // 4. sor
            data = LCD_SET_DDADR + LCD_DDADR_LINE4 + column;
            break;

        default:
            return;  // Rossz sor esetén
    }

    lcd_command( data );
}


//========= Karakterlánc kiíratása az aktuális kurzorpozíciótól kezdve =========
void lcd_string(const char *data)
{
    while(*data)
    {
        lcd_data(*data ++);
    }
}


//========================== Egyéni karakter definiálása =======================
void lcd_generatechar( uint8_t code, const uint8_t *data )
{
    // Karakter címének beállítása
    lcd_command( LCD_SET_CGADR | (code<<3) );

    // Bitminta átvitele
    for ( uint8_t i=0; i<8; i++ )
    {
        lcd_data( data[i] );
    }

    lcd_command(LCD_SET_DDADR); //DRAM auf 0 setzen
}


//====================== töröl egy sort az adott oszloptól  ====================
void lcd_clearrow(unsigned char row, unsigned char column)
{
    lcd_setcursor(row,column);
    uint8_t i = column;
    while(i < 16)
    {
        lcd_string(" ");
        _delay_us(1);
        i++;
    }
    lcd_setcursor(row,column);
}


//=================== Kiír egy decimális számot a kijelzőre ====================
void lcd_decimal(long int number)
{
    char buffer [10];
    ltoa(number,buffer,10);
    lcd_string(buffer);
}


//==================== Kitöröl egy karaktert a megadot pozición ================
void lcd_clearchar(unsigned char row, unsigned char column)
{
    lcd_setcursor(row,column);
    lcd_string(" ");
}
