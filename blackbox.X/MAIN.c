/*
 * File:   MAIN.c
 * Author: mainuddin m shaikh
 *
 * Created on 18 April, 2024, 8:36 AM
 */


#include <xc.h>
#include"MAIN.h"
short main_flag=0;
short at_fail=120,E_P_T=4;

static void init_config(void)
{
    init_clcd();
    init_adc();
    init_matrix_keypad();
    init_timer0();
    init_i2c();
	init_ds1307();
   write_external_eeprom(101,'0');// NOTE: uncomment this while uploading 
   write_external_eeprom(102,'0');  //     program for first time.
   write_external_eeprom(103,'0');    //   after changing password comment this and
   write_external_eeprom(104,'0');      // re upload the program your password remain same. 
    fetch_password();
  
}
void main(void) 
{
    init_config();
    
    while(1)
    {
        if(main_flag >= 2)
        {
           key = read_switches(LEVEL);  
        }
        else
        {
            key = read_switches(STATE_CHANGE);
        }
        
        
        
        if (main_flag == 0)
        {
            dash_board();
        }
        if (main_flag == 1)
        {
            login_screen();
        }
        if(main_flag == 2)
        {
            main_menu();
        }
        if(main_flag == 10)
        {
            view_log();
        }
        if(main_flag == 11)
        {
            set_time();
        }
        if(main_flag == 12)
        {
            downlode_log();
        }
        if(main_flag == 13)
        {
            clear_log();
        }
        if(main_flag == 14)
        {
            change_password();
        }
        
    }
    return;
}
