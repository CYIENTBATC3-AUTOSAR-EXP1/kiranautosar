[Yesterday 11:39 AM] Raju Bodige
/** File:   main.c
* Author: rb65404
*
* Created on October 11, 2022, 4:49 PM
*/


// CONFIG1H#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)#pragma config OSCS = OFF       // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))

// CONFIG2L#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)#pragma config BOR = ON         // Brown-out Reset Enable bit (Brown-out Reset enabled)#pragma config BORV = 25        // Brown-out Reset Voltage bits (VBOR set to 2.5V)

// CONFIG2H#pragma config WDT = ON         // Watchdog Timer Enable bit (WDT enabled)#pragma config WDTPS = 128      // Watchdog Timer Postscale Select bits (1:128)

// CONFIG4L#pragma config STVR = ON        // Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will cause Reset)#pragma config LVP = ON         // Low-Voltage ICSP Enable bit (Low-Voltage ICSP enabled)

// CONFIG5L#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-001FFFh) not code protected)#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code protected)#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code protected)#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code protected)

// CONFIG5H#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code protected)#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code protected)

// CONFIG6L#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-001FFFh) not write protected)#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write protected)#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write protected)#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write protected)

// CONFIG6H#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write protected)#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write protected)#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write protected)

// CONFIG7L#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-001FFFh) not protected from Table Reads executed in other blocks)#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from Table Reads executed in other blocks)#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from Table Reads executed in other blocks)#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from Table Reads executed in other blocks)

// CONFIG7H#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from Table Reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 20000000

int flag=0;
int can_buf[8];
void can_init()    {        TRISBbits.RB2=0;        TRISBbits.RB3=1;        TRISBbits.RB0=0;

        //PIE3bits.RXB0IE= Enable;        //PIE3bits.RXB0IP= 1;    }
sys_init()
{    GIE=1;   // Global interrrupt enable    PEIE=1;   // pheriphral enable
}       
//int can_receiver

void set_baud_rate(int baud)
{    CANCON=0x80;    //configaration mode       while(!((CANSTAT & 0x80)==0x80))    {              if(baud == 250)       {                      BRGCON1 =0xC1;           BRGCON1 =0xAE;           BRGCON1 =0xA5;                      CIOCON  =0x20;           CANCON  =0x08;                  }   }
}

can_rec_config()
{    PIE3bits.RXB0IE= 1;        IPR3bits.RXB0IP= 1;        CANCON=0x0E;        RXF0SIDH=0x00;    RXF0SIDL=0x00;        RXM0SIDH=0x00;    RXM0SIDL=0x00;
}    char can_send_msg(int type, int id, int *data)    {        CANCON =0x80;        if(type)        {                TXB0SIDH =((id >>3)&0xff);        TXB0SIDL =((id <<5)&0xff);                __delay_ms(100);                TXB0DLC= 0X08;                TXB0D0=data[0];        TXB0D1=data[1];        TXB0D2=data[2];        TXB0D3=data[3];        TXB0D4=data[4];        TXB0D5=data[5];        TXB0D6=data[6];        TXB0D7=data[7];                TXB0CON=0x08;        __delay_ms(10);        CANCON=0x08;       }        else{               }    }        void can_read()    {        //int can_buf[8];                can_buf[0]=RXB0D0;        can_buf[1]=RXB0D1;        can_buf[2]=RXB0D2;        can_buf[3]=RXB0D3;        can_buf[4]=RXB0D4;        can_buf[5]=RXB0D5;        can_buf[6]=RXB0D6;        can_buf[7]=RXB0D7;                RXB0CONbits.RXFUL = 0;                CANCON = 0x0E;

    }     
void main(void)
{   char data[8];    int index=0;        sys_init();    can_init();    set_baud_rate(250);    can_rec_config();        for(index=0; index<=7; index++)    {       data[index]=0x31 + index;               //CANCON=0X80;    }    while(1)    {      //  can_send_msg(int type,int id, int *data);        if(flag==1)        {            can_read();            PORTBbits.RB0=1;            //can_send_msg(type,id,can_buf[i]);            flag=0;            __delay_ms(10);            PORTBbits.RB0=0;        }    }
}
void __interrupt() ISR1()   {       if(PIR3bits.RXB0IF==1)       {               PIE3bits.RXB0IE= 0;               flag=1;       }   }

