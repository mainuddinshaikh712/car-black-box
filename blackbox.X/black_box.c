/*
 * File:   black_box.c
 * Author: mainuddin m shaikh
 *
 * Created on 18 April, 2024, 8:36 AM
 */


#include <xc.h>
#include"MAIN.h"
static int i=0; //i variable to save the index number of gears
void dash_board(void) 
{
    
    unsigned short speed = read_adc(4)/10;
    clcd_print(dashboard,LINE1(0));
    get_time();
    clcd_print(time,LINE2(0));
    if(key == (char*)1)
    {
        if(i != 8)
        {
            if(i<7)
            {
                i++;
                event_count++;
                store_in_eeprom(speed);
            }
        }
        else
        {
            i=1;
        }
    }
    if(key == (char*)2)
    {
        if(i!= 8)
        {
            if(i>1)
            {
                i--;
                store_in_eeprom(speed);
            }
        }
        else    
        {
            i=1;       
        }
    }
    if(key == (char*)3)
    {
        i=8;
        event_count++;
        store_in_eeprom(speed);
    }
    if (key == (char*)11 || key == (char*)12)
    {
        CLEAR_DISP_SCREEN;
        for(short i=1000;i!=0;i--);
        main_flag = 1;
        key=0xFF;
        return;
    }
    
    clcd_print(event[i],LINE2(10));
    
    if(speed > 99)
        speed = 99;
    clcd_putch((speed / 10) + 48, LINE2(13));
    clcd_putch((speed % 10) + 48, LINE2(14));
    
    static short f=1;
    if(f)
    {
      store_in_eeprom(speed);
      f=0;
    }
    if(event_count == 10)
    {
        event_count = 9;
    }
}

static void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
    return;
}


void store_in_eeprom(int speed)
{
    static unsigned int j= 0x00;
    unsigned char data[11] = {0};
    data[0] = time[0];
    data[1] = time[1];
    data[2] = time[3];
    data[3] = time[4];
    data[4] = time[6];
    data[5] = time[7];
    //event
    data[6] = event[i][0];
    data[7] = event[i][1];
    //speed
    data[8]=((speed/10) + 48);
    data[9]=((speed%10) + 48);
    for(int i=0;i<10;i++)
    {
        write_external_eeprom(j+i, data[i]);
    }
    j = j + 10;
    if(j == 100)
        j = 0;
}

void fetch_password(void)
{
    orig_pass[0] = read_external_eeprom(101);
    orig_pass[1] = read_external_eeprom(102);
    orig_pass[2] = read_external_eeprom(103);
    orig_pass[3] = read_external_eeprom(104);
}


void login_screen(void)
{
    //U_S= under score,  E_P_T= enter password time (4sec)
    static short attempt=1,U_S=0, pass_count=0, pass_complet=0;
    GIE=1;
    if(E_P_T >= 0 )
    {
        
        if(pass_count < 4)
        {
            clcd_print("_Enter password_",LINE1(0));
            if (U_S++ < 200)
            {
               clcd_putch('_',LINE2(pass_count)); 
            }
            else if(U_S++ >200 && U_S < 400)
            {
                clcd_putch(' ',LINE2(pass_count));
            }
            else
                U_S =0;
            
            if(key == 11)
            {
                usr_pass[pass_count]=((char)0+48);
                clcd_putch('*',LINE2(pass_count));
                pass_count++;
                E_P_T=4;
            }
            else if(key == 12)
            {
                usr_pass[pass_count]=((char)1+48);
                clcd_putch('*',LINE2(pass_count));
                pass_count++;
                E_P_T=4;
            }        
        }
        else
        {
            usr_pass[4]='\0';
            pass_complet=1;
        }
    }
    else
    {
        CLEAR_DISP_SCREEN;
        main_flag=0;
        GIE=0;E_P_T=4;at_fail=120;//reset password entering timings
        attempt=1;U_S=0; pass_count=0; pass_complet=0; //reset password entering process
        return;
    }
        if(pass_complet)
        {
            GIE=0,E_P_T=4,at_fail=120; //reset password entering timings
            fetch_password();
            if ((strcmp(orig_pass,usr_pass))== 0)
            {
                CLEAR_DISP_SCREEN;
                clcd_print("Main Menu",LINE1(0));
                main_flag=2;
                key = 0xff;
                GIE=0;E_P_T=4;at_fail=120;//reset password entering timings
                attempt=1; U_S=0; pass_count=0; pass_complet=0; //reset password entering process
                __delay_ms(1500);
                CLEAR_DISP_SCREEN;
                return;
            }
            else if(attempt < 3)
            {
                CLEAR_DISP_SCREEN;
                clcd_print("wrong password",LINE1(0));     
                clcd_print("ATTEMPT LEFT",LINE2(0));
                clcd_putch((char)(3-attempt)+48,LINE2(13));
                __delay_ms(2000);
                CLEAR_DISP_SCREEN;
                pass_count=0;
                pass_complet=0;
                attempt++;
            }
            else if(attempt > 2)
            {
                GIE = 1;
                CLEAR_DISP_SCREEN;
                while(1)
                    {
                        clcd_print("Wait ...For",LINE1(0));
                        clcd_putch((at_fail%10)+48,LINE2(2));
                        clcd_putch((at_fail/10)%10 + 48,LINE2(1));
                        clcd_putch(((at_fail/100)%10) + 48,LINE2(0));
                        clcd_print("... sec",LINE2(3));
                        if(at_fail == 0 )
                        {
                            CLEAR_DISP_SCREEN;
                            GIE = 0;at_fail=120;//reset password entering timings
                            main_flag=0;
                            attempt=1;U_S=0; pass_count=0; pass_complet=0; //reset password entering process   
                            return;
                        }
                    }
            }
        }
}


void main_menu(void)
{
    static int j=0, count=0, flag=0;//flag 0 for 11 key and 1 for 12 key ;
    static unsigned long int delay1=0, delay2=0;
    if (key == (char*)11 )
    {
        if(++delay1 >= 500 )
        {
            CLEAR_DISP_SCREEN;
            clcd_print("entered in",LINE1(0));
            clcd_print(menu[count],LINE2(0));
            main_flag = (count+10);
            j=0;delay1=0;delay2=0; count=0; flag=0,key=0xff;
            __delay_ms(1500);
            CLEAR_DISP_SCREEN;
            return;
        }
    }
    else if (delay1>0 && delay1<500 && count > 0)
    {
        if(count < 4 && j > 0 && flag == 0)
        {
            j--;
        }
        count--;
        CLEAR_DISP_SCREEN;
        flag=0;
        delay1=0;
    }
    else
        delay1=0;
    if (key == (char*)12  )
    {
        if(++delay2 >= 500)
        {
            CLEAR_DISP_SCREEN;
            j=0;delay1=0; delay2=0; count=0; flag=0;
            main_flag=0;
            return;
        }
    }
    else if (delay2>0 && delay2 < 500 && count < 4)
    {
        if(count > 0 && j < 3 && flag== 1)
        {
            j++;
        }
        count++;
        CLEAR_DISP_SCREEN;
        flag=1;
        delay2=0;
    }
    else
    {
        delay2=0;
    }
    
    if(flag == 0)
    {
        clcd_putch('*',LINE1(0));
    }
    else
    {
        clcd_putch('*',LINE2(0));
    }
    clcd_print(menu[j],LINE1(1));
    clcd_print(menu[j+1],LINE2(1));
}

void change_password(void)
{
    static char read_pass[5];
    static char confirm_pass[5];
    static unsigned long int delay1=0, delay2=0;
    static unsigned int pass_count=0, pass_count2=0,cs_f=0;//cs_f = clearscreen flag
    
    
    if(pass_count < 4)
        clcd_print("new password",LINE1(0));
    else if (pass_count > 3)
    {
        if(cs_f == 0)
        {
           CLEAR_DISP_SCREEN;
           cs_f = 1;
        }
        clcd_print("confirm password",LINE1(0));
    }
    
    
     if (key == (char*)11 || key == (char*)12)
    {
        if(key == (char*)11 && ++delay1 >= 500 )
        {
            delay1=0;
        }
        else if(key == (char*)12 && ++delay2 >= 500)
        {
            CLEAR_DISP_SCREEN;
            delay1=0;delay2=0;pass_count=0;pass_count2=0;
            main_flag=2;
            return;
        }
    }
     else if (delay1 > 0 && delay1<500)
     {
         if(pass_count < 4 )
         {
            read_pass[pass_count]=((char)0+48);
            clcd_putch('*',LINE2(pass_count));
            pass_count++;
            delay1=0;
         }
         else if (pass_count >3 && pass_count2 <4)
         {
            confirm_pass[pass_count2]=((char)0+48);
            clcd_putch('*',LINE2(pass_count2));
            pass_count2++;
            delay1=0;
         }
     }
     else if (delay2 > 0 && delay2 < 500)
     {
          if(pass_count < 4 )
         {
            read_pass[pass_count]=((char)1+48);
            clcd_putch('*',LINE2(pass_count));
            pass_count++;
            delay2=0;
         }
         else if (pass_count >3 && pass_count2 <4)
         {
            confirm_pass[pass_count2]=((char)1+48);
            clcd_putch('*',LINE2(pass_count2));
            pass_count2++;
            delay2=0;
         }
     }
     else
     {
         delay1=0;delay2=0;
     }
    
    if (pass_count > 3 && pass_count2 > 3)
    {
        read_pass[4]='\0';
        confirm_pass[4]='\0';
        if(strcmp(read_pass,confirm_pass)== 0)
        {
            CLEAR_DISP_SCREEN;
            strcpy(orig_pass,read_pass);
            write_external_eeprom(101, orig_pass[0]); //password are in address 101 - 104
            write_external_eeprom(102, orig_pass[1]);
            write_external_eeprom(103, orig_pass[2]);
            write_external_eeprom(104, orig_pass[3]);
            clcd_print("password_updated",LINE1(0));
            __delay_ms(1500);
            CLEAR_DISP_SCREEN;
            delay1=0;delay2=0;pass_count=0;pass_count2=0;cs_f=0;
            main_flag=2;
            return;
        }
        else
        {
            CLEAR_DISP_SCREEN;
            clcd_print("pass_mismatch",LINE1(0));
            __delay_ms(1500);
            CLEAR_DISP_SCREEN;
            delay1=0;delay2=0;pass_count=0;pass_count2=0;cs_f=0;
            main_flag=2;
            return;
        }
    }
         
}
    


void set_time(void)
{
    static short flag=0,feild_change=0;
     static unsigned int delay1=0, delay2=0;
     GIE=1;
    clcd_print("HH:MM:SS",LINE1(4));
    if(flag == 0)
    {
        hr = (time[0] - '0')*10 + (time[1] - '0');
        min = (time[3] - '0')*10 + (time[4] - '0');
        sec = (time[6] - '0')*10 + (time[7] - '0');
    flag=1;
    }
    clcd_putch((hr / 10) + 48, LINE2(4));
    clcd_putch((hr % 10) + 48, LINE2(5));
    clcd_putch(':', LINE2(6));
    clcd_putch((min / 10) + 48, LINE2(7));
    clcd_putch((min % 10) + 48, LINE2(8));
    clcd_putch(':', LINE2(9));
    clcd_putch((sec / 10) + 48, LINE2(10));
    clcd_putch((sec % 10) + 48, LINE2(11));
    
    // key workings 
     if (key == (char*)11 )
    {
        if(++delay1 >= 500 )
        {
            time[0] = ((hr/10) + 48);
            time[1] = ((hr%10) + 48);
            time[3] = ((min/10) + 48);
            time[4] = ((min % 10) + 48);
            time[6] = ((sec/10) + 48);
            time[7] = ((sec%10) + 48);
            write_ds1307(HOUR_ADDR,(time[0]<<4)| (time[1]&0x0f));
            write_ds1307(MIN_ADDR,(time[3]<<4)| (time[4]&0x0f));
            write_ds1307(SEC_ADDR,(time[6]<<4)| (time[7]&0x0f));
            flag=0;feild_change=0;delay1=0; delay2=0;GIE=0;
            CLEAR_DISP_SCREEN;
            main_flag=1;
            return;
        }
    }
    else if (delay1>0 && delay1<500)
    {
        if (feild_change < 2)
            feild_change++;
        else
            feild_change = 0;
       delay1=0;
    }
    else
        delay1=0;
    if (key == (char*)12  )
    {
        if(++delay2 >= 500)
        {
            flag=0;feild_change=0;delay1=0; delay2=0;GIE=0;
            CLEAR_DISP_SCREEN;
            main_flag=2;
            return;
        }
    }
    else if (delay2>0 && delay2 < 500 )
    {
        if(feild_change == 0)
        {
            if(++sec > 59)
                sec=0;
        }
        if(feild_change == 1)
        {
            if(++min > 59)
                min=0;
        }
        if(feild_change == 2)
        {
            if (++hr > 23)
                hr=0;
        }
        
        delay2=0;
    }
    else
    {
        delay2=0;
    }
    
    
    //for feild blinking
    if(feild_change == 0)
    {
        if(blink)
        {
           clcd_putch((sec / 10) + 48, LINE2(10));
           clcd_putch((sec % 10) + 48, LINE2(11)); 
        }
        else
        {
           clcd_putch(' ', LINE2(10));
           clcd_putch(' ', LINE2(11)); 
        }
        
    }
    else if (feild_change == 1)
    {
         if(blink)
        {
            clcd_putch((min / 10) + 48, LINE2(7));
            clcd_putch((min % 10) + 48, LINE2(8));
        }
        else
        {
           clcd_putch(' ', LINE2(7));
           clcd_putch(' ', LINE2(8)); 
        }
    }
    else if (feild_change == 2)
    {
         if(blink)
        {
            clcd_putch((hr / 10) + 48, LINE2(4));
            clcd_putch((hr % 10) + 48, LINE2(5));
        }
        else
        {
           clcd_putch(' ', LINE2(4));
           clcd_putch(' ', LINE2(5)); 
        }
    }
}

void view_log(void)
{
    if(event_count)
    {
        static unsigned int delay1=0, delay2=0,index=0;
        static short flag=1;
        if(flag)
        {
             flag=0;
            read_from_EEPROM(index*10);  
        }
        clcd_print("# TIME     EV SP",LINE1(0));
        if (key == (char*)11 || key == (char*)12)
        {
            if(key == (char*)11 && ++delay1 >= 500 )
            {
                delay1=0;
            }
            else if(key == (char*)12 && ++delay2 >= 500)
            {
                CLEAR_DISP_SCREEN;
                delay1=0, delay2=0,index=0;
                main_flag = 2;
                return;
            }
        }
        else if (delay1 > 0 && delay1<500)
        {  
             if(index < event_count)
            {
                 index++;
                read_from_EEPROM(index*10);
            }
            delay1 = 0; 
        }
        else if (delay2 > 0 && delay2 < 500)
        {
             if(index > 0)
            {
                 index--;
                flag = 1;
                read_from_EEPROM(index*10);
            }
            delay2 = 0;
        }
        else
        {
             delay1=0;delay2=0;
        }
        char log_event[16];
        log_event[0] = index+48;
        log_event[1] = ' ';
        log_event[2] = data[0];
        log_event[3] = data[1];
        log_event[4] = ':';
        log_event[5] = data[2];
        log_event[6] = data[3];
        log_event[7] = ':';
        log_event[8] = data[4];
        log_event[9] = data[5];
        log_event[10] = ' ';
        log_event[11] = data[6];
        log_event[12] = data[7];
        log_event[13] = ' ';
        log_event[14] = data[8];
        log_event[15] = data[9];
        log_event[16] = '\0';
        clcd_print(log_event,LINE2(0));
    }
    else
    {
        clcd_print("Empty Log",LINE1(0));
        __delay_ms(1500);
        CLEAR_DISP_SCREEN;
        main_flag = 2;
        return;
    }
    
}

void read_from_EEPROM(char *j)
{
   
    for( int i=0; i<10; i++)
    {
        data[i] = read_external_eeprom(j+i);
    }
    return;
}

void downlode_log(void)
{
    if(event_count)
    {
    clcd_print("downloding ...",LINE1(0));
    unsigned char downlode_events[10][17];
    for(int i=0; i<=event_count; i++)
    {
        read_from_EEPROM(i*10);
        downlode_events[i][0] = (i)+48;
        downlode_events[i][1] = ' ';
        downlode_events[i][2] = data[0];
        downlode_events[i][3] = data[1];
        downlode_events[i][4] = ':';
        downlode_events[i][5] = data[2];
        downlode_events[i][6] = data[3];
        downlode_events[i][7] = ':';
        downlode_events[i][8] = data[4];
        downlode_events[i][9] = data[5];
        downlode_events[i][10] = ' ';
        downlode_events[i][11] = data[6];
        downlode_events[i][12] = data[7];
        downlode_events[i][13] = ' ';
        downlode_events[i][14] = data[8];
        downlode_events[i][15] = data[9];
        downlode_events[i][16] = '\0';
    }
    init_uart();
    GIE = 1;
    puts("# TIME     EV SP\n\r");
    for(int k=0; k<event_count; k++)
    {
        puts(downlode_events[k]);
        puts("\n\r");
    }
    __delay_ms(1500);
    CLEAR_DISP_SCREEN;
     PEIE = 0;
    GIE = 0;
    main_flag=2;
    return;
    }
    else
    {
        clcd_print("Empty Log",LINE1(0));
        __delay_ms(1500);
        CLEAR_DISP_SCREEN;
        main_flag = 2;
        return;
    }
    
}

void clear_log(void)
{
    clcd_print("Clear Log",LINE1(0));
    clcd_print("Successfully",LINE2(0));
    event_count = 0;
    main_flag = 2;
    __delay_ms(1500);
    CLEAR_DISP_SCREEN;
    return;
}