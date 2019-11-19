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
#include "game.h"

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

    ADC_init();
    SRAM_init();
    oled_init();
   _delay_ms(2000);
  
    struct screen* menu = init_menu();
   
    joy_init();
    joy_position pos;
 
    int status = 0;
    int lives = 3;
    int points = 0;
    int difficulty = 1;
  
    CAN_init();
    sei();

    SRAM_test();

    while(1){
        pos = joy_getDir();
 
        menu = draw_screen(menu, pos.direction, &status, lives, points);
        oled_refresh();
        if(menu->name == "difficulty"){
            difficulty = menu->select;
            if(strcmp(pos.direction, "RIGHT") == 0){
            
                menu = draw_screen(menu, pos.direction, &status, lives, points);
                oled_refresh();
            }
        }
       if(menu->name == "game"){
            menu = draw_screen(menu, pos.direction, &status, lives, points);
            oled_refresh();
            menu = start_game(menu, pos.direction, &status, difficulty);
        }
        _delay_ms(100);
    }
    return 0;
}
