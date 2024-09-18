/*
 * File:   timer0_p.c
 * Author: mainuddin m shaikh
 *
 * Created on 22 April, 2024, 9:05 PM
 */


#include <xc.h>
#include"MAIN.h"

void init_timer0()
{
    //intrupt
    GIE=0;
    PEIE=1;
    TMR0IE=1;
    TMR0IF=0;
    
    //TIMER0
    TMR0=6;
    TMR0ON=1;
    T08BIT=1;
    T0CS=0;
    PSA=1;
}
