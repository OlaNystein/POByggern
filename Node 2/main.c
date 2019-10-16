#include <avr/io.h>
#define F_CPU 4915200UL
#include "util/delay.h"
#include "uart.h"
#include "ADC.h"
#include "setup.h"
#include "SPI.h"
#include <avr/interrupt.h>
#include "CAN.h"
#include "MCP.h"
#define FOSC 4915200UL
#define BAUD 9600
#define MYUBURR FOSC/16/BAUD-1


int main(void){
   UART_Init(MYUBURR);
   /*sei();
   EIMSK |= (1 << INT0); //enable int0
   EICRA |= (1 << ISC01); //trigger falling edge of interrupt
   EICRA &= ~(1 << ISC00); //trigger falling edge of interrupt
   CAN_init();

   message msg;
    msg.ID = 51;
    msg.length = 3;
    msg.data[0] = 2;
    msg.data[1] = 4;
    msg.data[2] = 6;

   if (CAN_init()==0){
       printf("CAN working\n\r");
   }else{
       printf("CAN error\n\r");
       }*/


    
    
    while(1){
        printf("1");
        _delay_ms(2000);
        //CAN_send(&msg);
        //message new = CAN_recieve();
        //printf("ID: %d\tDATA:%d\t%d\t%d\n\r", new.ID, new.data[0], new.data[1], new.data[2]);
    }
    return 0;
}
