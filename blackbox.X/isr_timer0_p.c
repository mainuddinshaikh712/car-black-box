/*
 * File:   isr_timer0_p.c
 * Author: mainuddin m shaikh
 *
 * Created on 22 April, 2024, 9:05 PM
 */


#include <xc.h>
#include "MAIN.h"

void __interrupt() isr()
{
    static unsigned long int count;
    if(TMR0IF)
    {
        TMR0 = TMR0+8;
        //for 120sec 20000*120 = 2400000
        if(++count == 2400000)
        {
            at_faile=0;
        }
        TMR0IF = 0;
        
    }
}
