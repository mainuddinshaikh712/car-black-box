/* 
 * File:   MAIN.h
 * Author: mainuddin m shaikh
 *
 * Created on 18 April, 2024, 8:36 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#include "string.h"
// DASH BOARD //
extern short main_flag,at_fail,E_P_T;
unsigned int sec=0,min=0,hr=0,blink=0,event_count=1;
char *key;
char dashboard[]="TIME      EV SP";
char time[]= "00:00:00";
unsigned char clock_reg[3];
char *event[]={"ON","GN","GR","G1","G2","G3","G4","G5","CO"};
char *menu[] = {"View log","Set Time","Download Log","Clear Log","Change Password"};
char usr_pass[5]="0000";
char orig_pass[5]="0000";
unsigned char data[11] = {0}; //reading data from EEPROM in data string


//requirements
/*1*/void dash_board(void) ;
     /*1.1*/static void get_time(void);
     /*1.2*/void store_in_eeprom(int speed);
/*2*/void login_screen(void);
/*3*/void main_menu(void);
    /*3.1 */void change_password(void);
            void fetch_password(void);
    /*3.2*/void set_time(void);
    /*3.3*/void view_log(void);
    /*3.4*/void read_from_EEPROM(char *j);
    /*3.5*/void downlode_log(void);
    /*3.6*/void clear_log(void);



//  CLCD CONFIG //
#ifndef LCD_H
#define LCD_H



#define CLCD_PORT			PORTD
#define CLCD_EN				RC2
#define CLCD_RS				RC1
#define CLCD_RW				RC0
#define CLCD_BUSY			RD7
#define PORT_DIR			TRISD7


#define HI												1
#define LO												0

#define INPUT											0xFF
#define OUTPUT											0x00

#define DATA_COMMAND									1
#define INSTRUCTION_COMMAND								0
#define _XTAL_FREQ                  20000000
#define LINE1(x)									(0x80 + (x))
#define LINE2(x)										(0xC0 + (x))

#define TWO_LINE_5x8_MATRIX_8_BIT					clcd_write(0x38, INSTRUCTION_COMMAND)
#define CLEAR_DISP_SCREEN				                clcd_write(0x01, INSTRUCTION_COMMAND)
#define CURSOR_HOME							clcd_write(0x02, INSTRUCTION_COMMAND)
#define DISP_ON_AND_CURSOR_OFF						clcd_write(0x0C, INSTRUCTION_COMMAND)
#define EIGHT_BIT_MODE   0x33
#define LEVEL 0
#define STATE_CHANGE 1




// CLCD
void init_clcd(void);
void clcd_print(const unsigned char *data, unsigned char addr);
void clcd_putch(const unsigned char data, unsigned char addr);
void clcd_write(unsigned char bit_values, unsigned char control_bit);

//ADC
void init_adc();
unsigned short read_adc(unsigned char channel);



//matrix keypad
void init_matrix_keypad();
unsigned char read_switches(unsigned char detection);
unsigned char scan_key();

//timer0
void init_timer0();
void __interrupt() isr();

//i2c

void init_i2c(void);
void i2c_start(void);
void i2c_rep_start(void);
void i2c_stop(void);
void i2c_write(unsigned char data);
unsigned char i2c_read(void);

//ds1307

#define SLAVE_READ		0xD1
#define SLAVE_WRITE		0xD0


#define SEC_ADDR		0x00
#define MIN_ADDR		0x01
#define HOUR_ADDR		0x02
#define DAY_ADDR		0x03
#define DATE_ADDR		0x04
#define MONTH_ADDR		0x05
#define YEAR_ADDR		0x06
#define CNTL_ADDR		0x07

void init_ds1307(void);
void write_ds1307(unsigned char address1,  unsigned char data);
unsigned char read_ds1307(unsigned char address1);


//EEPROM
#define SLAVE_READ_EEPROM		0xA1
#define SLAVE_WRITE_EEPROM		0xA0

unsigned char read_external_eeprom(unsigned char address);
void write_external_eeprom(unsigned char address, unsigned char data);


//UART
#define RX_PIN					TRISC7
#define TX_PIN					TRISC6

void init_uart(void);
void putch(unsigned char byte);
int puts(const char *s);


#endif
#endif	/* MAIN_H */

