#include <avr/io.h>
#define F_CPU 4915200UL
#include "util/delay.h"
#include "uart.h"
#include "sram.h"
#include "ADC.h"
#include "joystick.h"
#include "setup.h"
#include "oled.h"
#include "menu.h"
#include "SPI.h"
#include <avr/interrupt.h>
#include "CAN.h"
#include "MCP.h"
#define FOSC 4915200UL
#define BAUD 9600
#define MYUBURR FOSC/16/BAUD-1


int main(void){
    UART_Init(MYUBURR);
    MCUCR |= (1<<SRE);
    SFIOR |= (1<<XMM2);
    

    //interrupt setup

    GICR |= (1 << INT0); //enable int0
    MCUCR |= (1 << ISC01); //trigger falling edge of interrupt
    MCUCR &= ~(1 << ISC00); //trigger falling edge of interrupt

    //ADC_init();
    //SRAM_test();
    
    oled_init();
   _delay_ms(2000);
  
  struct screen* main_menu = init_menu();

  
   
    joy_init();
    joy_position pos;
    int* status = 0;
    message msg;
    msg.ID = 91;
    msg.length = 3;
    msg.data[0] = 2;
    msg.data[1] = 4;
    msg.data[2] = 6;
    CAN_init();
    sei();
    
    
    while(1){
        pos = joy_getPos();
        printf("pos.x: %d pos.y : %d\n\r", pos.x, pos.y);
        msg.data[0] = pos.x;
        msg.data[1] = pos.y;
        CAN_send(&msg);
        
        //message new = CAN_recieve();
        //printf("ID: %d\tDATA:%d\t%d\t%d\n\r", new.ID, new.data[0], new.data[1], new.data[2]);
        
        _delay_ms(500);
   
        
    }
    return 0;
}
