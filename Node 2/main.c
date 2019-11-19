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
//#include "game.h"
#include "DAC.h"
#include "controller.h"
#include "solenoid.h"
#include "music.h"
#define FOSC F_CPU
#define BAUD 9600
#define MYUBURR FOSC/16/BAUD-1
#define BADISR_vect
int score = 0;
int detected_goal = 0;
//volatile message m;
//volatile int solenoid_shot = 0;
//volatile int counter = 0;
//unsigned int servo = 1155;

int main(void){
    cli();
    UART_Init(MYUBURR);
    

    EICRA |= (1 << ISC21); //trigger falling edge of interrupt
    EICRA &= ~(1 << ISC20); //trigger falling edge of interrupt

    EIMSK |= (1 << INT2); //enable int2
    
    EIFR |= (1 << INTF2);
    CAN_init();
    _delay_ms(20);
    if (CAN_init()==0){
        printf("CAN working\n\r");
    }else{
        printf("CAN error\n\r");
        }
    
    
    pwm_init();
    _delay_ms(20);
    ADC_init();
    _delay_ms(20);
    DAC_init();
    _delay_ms(20);
    controller_init();
    _delay_ms(20);
    calibrate_encoder();
    //timer_init();
    //printf("resets\n\r");
    solenoid_init();
    music_init();
    timer_init();
    sei();
    message m;
    m.ID = 3;
    m.length = 2;
    m.data[0] = 2;
    m.data[1] = 4;
    while(1){
        //printf("vi er i main \n\r");
        CAN_send(&m);
        message l = get_CAN();
        printf("%d\r\n", l.ID);
        printf("plis");
        //m.data[0] = 6
        //printf("mdata id: %d    mdata data: %d  diff: %d\n\r", get_CAN().ID, get_CAN().data[3], get_CAN().data[5]);
        // if(get_CAN().ID == 5){
        //     printf("play music: %d\n\r", get_CAN().data[0]);
        //     play_music(get_CAN().data[0]);
        // }
        // if(get_CAN().ID == 1 && get_CAN().data[4] == 1){
        //     printf("starting game..\n\r");
        //     start_game();
        // }
        //printf("sumerr: %d\terror: %d\tpos: %d\r\n", gety(), getu(), getpos());
        
    }
    return 0;
}

