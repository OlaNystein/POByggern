#include <avr/io.h>
#define F_CPU 16000000
#include "util/delay.h"
#include "uart.h"
#include "ADC.h"
#include "setup.h"
#include "SPI.h"
#include <avr/interrupt.h>
#include "CAN.h"
#include "MCP.h"
#include "pwm.h"
#include "game.h"
#define FOSC F_CPU
#define BAUD 9600
#define MYUBURR FOSC/16/BAUD-1

unsigned int servo = 1157;
int score = 0;
int detected_goal = 0;


int main(void){
   UART_Init(MYUBURR); 
   EIMSK |= (1 << INT0); //enable int0
   EICRA |= (1 << ISC01); //trigger falling edge of interrupt
   EICRA &= ~(1 << ISC00); //trigger falling edge of interrupt
   CAN_init();

   sei();

   


  

   if (CAN_init()==0){
       printf("CAN working\n\r");
   }else{
       printf("CAN error\n\r");
       }
    


    pwm_init();
    ADC_init();
    
    
    while(1){
        message m = CAN_recieve();
        //printf("ID: %d\tDATA:%d\t%d\t%d\n\r", m.ID, m.data[0], m.data[1]);
        servo = pwm_pulse(servo, m);

        uint16_t d = ADC_read();
        printf("%d\r\n", d);
        
        count_score(&score, d, &detected_goal);
        printf("Score: %d\t Flag: %d\r\n", score, detected_goal);
        
        //CAN_send(&msg);
        //message new = CAN_recieve();
        //printf("ID: %d\tDATA:%d\t%d\t%d\n\r", new.ID, new.data[0], new.data[1], new.data[2]);
    }
    return 0;
}
