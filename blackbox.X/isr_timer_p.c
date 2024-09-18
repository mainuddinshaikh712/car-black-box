/*
 * File:   isr_timer_p.c
 * Author: mainuddin m shaikh
 *
 * Created on 22 April, 2024, 9:20 PM
 */


#include <xc.h>
#include"MAIN.h"

void __interrupt() isr()
{
    static unsigned long int count,count2,count3;
    if(TMR0IF)
    {
        TMR0 = TMR0+8;
        //for 1sec 20000
        if(++count == 20000)
        {
            count =0;
            at_fail--;
        }
        if(++count2 == 20000)
        {
            count2=0;
            E_P_T--;
        }
        if(++count3 == 10000)
        {
            blink = !blink;
            count3 = 0;
        }
        TMR0IF = 0;
        
       
        
    }
}

