/*
 * File:   adc_p.c
 * Author: mainuddin m shaikh
 *
 * Created on 21 April, 2024, 2:12 PM
 */


#include <xc.h>
#include "MAIN.h"

void init_adc()
{
    ADON =1;
    VCFG1=0;
    VCFG0=0;
    
    PCFG3=1;
    PCFG2=0;
    PCFG1=1;
    PCFG0=0;
    
    ADCS2=0;
    ADCS1=1;
    ADCS0=0;
    
    ADFM=1;
    ACQT2=1;
    ACQT1=0;
    ACQT0=0;
}

unsigned short read_adc(unsigned char channel)
{
    ADCON0 = (ADCON0 & 0XC3) | (channel << 2);
    GO =1;
    while(GO);
    return ((ADRESH& 0X03)<<8)|ADRESL;
}
