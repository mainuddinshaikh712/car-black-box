/*
 * File:   UART_P.c
 * Author: mainuddin m shaikh
 *
 * Created on 9 May, 2024, 7:12 PM
 */


#include <xc.h>
#include "MAIN.h"
void init_uart(void)
{
	/* Serial initialization */
	RX_PIN = 1;
	TX_PIN = 0;

	
	BRGH = 1;
	SPEN = 1;
	
	/* TX/RC7 and RX/RC6 act as serial port */
	/* Continous reception enable or disable */ 
	CREN = 1;
	
	

	/* Baud Rate Setting Register */
	/* Set to 10 for 115200, 64 for 19200 and 129 for 9600 */
	SPBRG = 129;
/* Receive Interruot Enable Bit (Enabling the serial port Interrupt) */
    RCIE = 1;


}

void putch(unsigned char byte) 
{
	/* Output one byte */
	/* Set when register is empty */
	while(!TXIF)
	{
		continue;
	}
	TXREG = byte;
}

int puts(const char *s)
{
	while(*s)		
	{
		putch(*s++);	
	}

	return 0;
}




