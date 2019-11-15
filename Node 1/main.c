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
    //SRAM_test();
    SRAM_init();
    oled_init();
   _delay_ms(2000);
  
  struct screen* menu = init_menu();
  int i = 0;  
   
    joy_init();
    joy_position pos;
    message msg;
    slider_position sli;
    int left_button_press = 0;
    msg.ID = 3;
    msg.length = 5;
    //msg.data[0] = 0;
    int status = 0;
    int lives = 3;
    CAN_init();
    sei();

    SRAM_test();





    while(1){

        pos = joy_getDir();
        //printf("%d\t%d\r\n", button_select(menu), status);
        sli = joy_getSliderPos();
        if(joy_button(0)){
            left_button_press = 1;
            printf("%d\r\n", left_button_press);
        }      

        //printf("Slider right: %d\r\n", sli.right);

        menu = draw_screen(menu, pos.direction, &status);
        oled_refresh();

       if(menu->name == "game"){
            menu = draw_screen(menu, pos.direction, &status);
            oled_refresh();
            menu = start_game(menu, pos.direction, &status);
        }
        //printf("%s\n\r", menu->name);

        //printf("pos.x: %d pos.y : %d direction: %s buttonpress: %d\r\n", pos.x, pos.y, pos.direction, joy_button(1));
        /*msg.data[0] = pos.x;
        msg.data[1] = pos.y;
        msg.data[2] = sli.left;
        msg.data[3] = sli.right;
        msg.data[4] = left_button_press;
        CAN_send(&msg);
        msg.data[4] = 0;
        left_button_press = 0;*/
        
        //message new = CAN_recieve();
        //printf("ID: %d\tDATA:%d\t%d\t%d\n\r", new.ID, new.data[0], new.data[1], new.data[2]);
        
        _delay_ms(100);
   
        
    }
    return 0;
}
