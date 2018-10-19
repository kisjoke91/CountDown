/*******************************************************************************
 
 Author:      Chaos Developing
 Date:        2018-07-23
 Compiler:    avr-gcc
 Info:        Count Down (like a kitchen timer)
 
 *******************************************************************************/

#include "menu.h"

int main()
{
    menu_init();    // Setup device
    menu_run();     // never ends
    
    while(1)
    {
        //if may ends ... :D
        menu_run();
    }
    
    return 0;
}

