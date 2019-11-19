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

int main(void){
    cli();
    UART_Init(MYUBURR); 
    EIMSK |= (1 << INT2); //enable int2
    EICRA |= (1 << ISC21); //trigger falling edge of interrupt
    EICRA &= ~(1 << ISC20); //trigger falling edge of interrupt
    CAN_init();
    if (CAN_init()==0){
        printf("CAN working\n\r");
    }else{
        printf("CAN error\n\r");
        }
    
    pwm_init();
    ADC_init();
    DAC_init();
    
    controller_init();
    calibrate_encoder();
    solenoid_init();
    music_init();
    timer_init();
    sei();
    
    message msg;
    while(1){
        msg = get_CAN();
        if(msg.ID == 5){
            play_music(msg.data[0]);
            msg.ID = -1;
        }
        if(msg.data[4] == 1){
            start_game();
        }
        _delay_ms(100);
        
    }
    return 0;
}
