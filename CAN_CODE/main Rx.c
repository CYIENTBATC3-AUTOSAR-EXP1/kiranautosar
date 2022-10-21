/*
 * File:   main.c
 * Author: kb70669
 *
 * Created on October 20, 2022, 11:48 AM
 */




// PIC18F458 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config OSCS = OFF       // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config BORV = 25        // Brown-out Reset Voltage bits (VBOR set to 2.5V)

// CONFIG2H
#pragma config WDT = ON         // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 128      // Watchdog Timer Postscale Select bits (1:128)

// CONFIG4L
#pragma config STVR = OFF       // Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will not cause Reset)
#pragma config LVP = OFF        // Low-Voltage ICSP Enable bit (Low-Voltage ICSP disabled)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-001FFFh) not code protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-001FFFh) not write protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-001FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from Table Reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from Table Reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

/* Defining Clock Frequency*/
#define _XTAL_FREQ 20000000

/* Global Variables */
char CAN_RX_FLAG=0;
int CAN_RX_Buff[8];
int CAN_RX_ID=0,CAN_RX_DLC=0;

int CAN_set_baud(int baud)
{
    CANCON = 0xE0;                    // CAN Configuration mode and  Receive Buffer 0
    while(!((CANSTAT &0x80) == 0x80)); // wait until the CAN set to Configuration mode
    
    if(baud == 125){
    /* 125KHz*/
        BRGCON1 = 0xC1;
        BRGCON2 = 0xB1;
        BRGCON3 = 0x45;
        if( (BRGCON1 == 0xC1) && (BRGCON2 == 0xB1) && (BRGCON3 == 0x45))
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    
    if(baud == 250){
    /* 250KHz*/
        BRGCON1 = 0xC1;
        BRGCON2 = 0xAE;
        BRGCON3 = 0xA5;
    
        if( (BRGCON1 == 0xC1) && (BRGCON2 == 0xAE) && (BRGCON3 == 0xA5))
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    
    CIOCON = 0x20;
    CANCON = 0x08;  // getting into CAN Normal mode
    
    return 0;
}

/*
 * System Init
 * Initializing Interrupts and all
 */
void System_Init()
{
    GIE = 1;    /* Enabling Global Interrupts */
    PEIE = 1;   /* Enabling Peripheral Interrupts */
}
/*
 * CAN Initialization
 */
void CAN_Init()
{
    TRISBbits.RB2 = 0;  // CAN TX
    TRISBbits.RB3 = 1;  // CAN RX
}

/*
 * CAN_Send_msg is used to transmit the data on the bus
 * attributes "type" define mode of bus
 *  1 - Standard mode
 *  0 - Extended mode 
 * "DLC" defines Data Length Code
 * "ID" is nothing but Identifier
 */
void CAN_Send_msg(char type,char DLC,int ID,int *data)
{
//   int data[8];
//   int index=0;
//   for(index=0;index<8;index++)
//   {
//       data[index] = 0x60 + index;
//   }
   
    CANCON = 0x08;  // getting into CAN Normal mode     
    if(type)    /* Standard Mode */
    {
        TXB0SIDH = ((ID >> 3) & 0xFF);
        TXB0SIDL = ((ID << 5) & 0xFF);
        
        __delay_ms(10); /* making some 10 ms delay to set Identifier*/
        
        //TXB0DLC = 0x08; /* Data Length 8 */
        
        TXB0DLC = DLC; /* Filling Data Length Code in reg*/
        
        /* Filling with data in transmit buffers */
        TXB0D0 = data[0];
        TXB0D1 = data[1];
        TXB0D2 = data[2];
        TXB0D3 = data[3];
        TXB0D4 = data[4];
        TXB0D5 = data[5];
        TXB0D6 = data[6];
        TXB0D7 = data[7];
        
        TXB0CON = 0x08;  /* Request to send the data */
        
         __delay_ms(10); /* making some 10 ms delay*/
        
        CANCON = 0x08;  // getting into CAN Normal mode     
    }
    else  /* Extended Mode */
    {
        
    }
}

void CAN_RX_Config()
{
    PIE3bits.RXB0IE = 1;    /* Enabling CAN RX interrupt */
    IPR3bits.RXB0IP = 1;    /* Setting High Priority to CAN RX interrupt */
    CANCON = 0x0E;
   
    
    RXF0SIDH = 0x00;    /* RECEIVE ACCEPTANCE FILTER to 0x00 */
    RXF0SIDL = 0x00;
    
    RXM0SIDL = 0x00;    /* RECEIVE ACCEPTANCE MASK to 0x00 */
    RXM0SIDH = 0x00;
   
    
    
//    RXB0D0
}

void CAN_Read()
{
    CANCON = 0x0E;   // CAN Normal mode and  Receive Buffer 0
    CAN_RX_DLC     = RXB0DLC; /* Taking DLC Data from CAN RX */
    CAN_RX_ID      = ((RXB0SIDL >> 5)&0x0F) | ((RXB0SIDH << 5)& 0xFFF0);
    
    CAN_RX_Buff[0] = RXB0D0;
    CAN_RX_Buff[1] = RXB0D1;
    CAN_RX_Buff[2] = RXB0D2;
    CAN_RX_Buff[3] = RXB0D3;
    CAN_RX_Buff[4] = RXB0D4;
    CAN_RX_Buff[5] = RXB0D5;
    CAN_RX_Buff[6] = RXB0D6;
    CAN_RX_Buff[7] = RXB0D7;
    
    RXB0CONbits.RXFUL = 0;  /* Resetting control register */
}


void main(void) {
   
   int data[8]; /* declaring buffers */
   int index=0;
   for(index=0;index<8;index++)
   {
       data[index] = 0x60 + index;  /* defining buffers with fixed data */
   }
    
   TRISBbits.RB0 = 0;   /* RB0 is connected to LED */
   TRISBbits.RB1 = 0;   /* RB1 is connected to LED */

   System_Init();     /* Initializing Interrupts and all */
   CAN_Init();        /* Initializing CAN */ 
   CAN_set_baud(250); /* Setting Baud rate as 250 kbps */
   
   CAN_RX_Config();   /* Initializing CAN RX */
   
   PORTBbits.RB1 = 1; /* defining Default state of LED 1 */
   

   while(1)
   {
//       CAN_Send_msg(1,0x08,0x100,&data[0]); /* Transmitting the data continuously with 0x100 ID */

       if(CAN_RX_FLAG)
       {
           CAN_RX_FLAG = 0;
           CAN_Read();
           PORTBbits.RB0 = ~PORTBbits.RB0;
           CAN_Send_msg(1,CAN_RX_DLC,0x100,CAN_RX_Buff);  /* Transmitting the data whatever received */            
       }
       
//       __delay_ms(100);
   }

   
    return;
}

/* Interrupt Service Routine */
void __interrupt() ISR1()
{
    if(PIR3bits.RXB0IF) /* Checking CAN RX Interrupt Flag */
    {
        PORTBbits.RB1 = ~PORTBbits.RB1;     /* Toggle LED1 */
        PIR3bits.RXB0IF = 0;                /* Clearing RX Interrupt Flag */
        CAN_RX_FLAG = 1;                    /* Setting the Flag */
    } 
}
