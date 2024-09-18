/*
 * File:   external_EEPROM_P.c
 * Author: mainuddin m shaikh
 *
 * Created on 7 May, 2024, 8:51 PM
 */


#include <xc.h>
#include "MAIN.h"

void write_external_eeprom(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_WRITE_EEPROM);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
    for(int i = 3000;i--;);
}

unsigned char read_external_eeprom(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(SLAVE_WRITE_EEPROM);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(SLAVE_READ_EEPROM);
	data = i2c_read();
	i2c_stop();
    for(int i = 3000;i--;);

	return data;
}

