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
#include "DAC.h"
#include "controller.h"
#include "solenoid.h"
#include "music.h"
#define FOSC F_CPU
#define BAUD 9600
#define MYUBURR FOSC/16/BAUD-1
int score = 0;
int detected_goal = 0;
volatile message m;
//volatile int solenoid_shot = 0;
//volatile int counter = 0;
//unsigned int servo = 1155;

int main(void){
    cli();
   UART_Init(MYUBURR); 
   EIMSK |= (1 << INT2); //enable int2
   EICRA |= (1 << ISC01); //trigger falling edge of interrupt
   EICRA &= ~(1 << ISC00); //trigger falling edge of interrupt
   CAN_init();
   if (CAN_init()==0){
       printf("CAN working\n\r");
   }else{
       printf("CAN error\n\r");
       }
    
    //init_timer();
    pwm_init();
    ADC_init();
    DAC_init();
    controller_init();
    printf("resets\n\r");
    solenoid_init();
    music_init();
    sei();
    play_music(2);
    //play_note(5000);
    while(1){
        //printf("vi er i main \n\r");
        m = CAN_recieve();
        //m.data[0] = 6
        printf("mdata id: %d    mdata data: %d \n\r", m.ID, m.data[4]);
        if(m.ID == 1 && m.data[4] == 1){
            //printf("starting game..\n\r");
            start_game();
        }
        //joy_to_voltage2(-100);
        //printf("DATA:%d\r\n",(-m.data[0]+240)/3);
        /*if(m.data[4]== 1){
            solenoid_pulse();
            //printf("button: %d\n\r",m.data[4]);
            //solenoid_shot = 1;
        }
        /*uint16_t d = ADC_read();
        printf("%d\r\n", d);
        
        count_score(&score, d, &detected_goal);
        printf("Score: %d\t Flag: %d\r\n", score, detected_goal);*/
        
        //CAN_send(&msg);
        //message new = CAN_recieve();
        //printf("ID: %d\tDATA:%d\t%d\t%d\n\r", new.ID, new.data[0], new.data[1], new.data[2]);
    }
    return 0;
}


/*ISR(TIMER3_COMPA_vect){
    
    //printf("interrupt\r\n");

    PID(m);
    
    if(counter < 100 && solenoid_shot == 1){
        counter++;
    }else if(counter >= 100 && solenoid_shot == 1){
        solenoid_pulse();
        solenoid_shot = 0;
        counter = 0;
    }

//TCNT3 = 0x00;
}*/