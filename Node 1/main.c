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
    //ADC_init();
    //SRAM_test();
    SRAM_init();
    oled_init();
   _delay_ms(2000);
  
  struct screen* menu = init_menu();
  int i = 0;
  while(menu->name[i] != '\0'){
    printf("%c", menu->name[i]);
    i++;
  }

    joy_init();
    joy_position pos;
    message msg;
    msg.ID = 3;
    msg.length = 2;
    //msg.data[0] = 0;
    int status = 0;
    CAN_init();
    sei();
    SRAM_test();

    while(1){
        pos = joy_getDir();
        message from_node2;
        from_node2 = CAN_recieve();
        //printf("%d\t%d\r\n", button_select(menu), status);
        oled_refresh();
        //draw_screen(menu, pos.direction, &status);
        if(strcmp(pos.direction, "RIGHT")==0 && status == 0){
            status = 1;
            if(menu->child[menu->select] != NULL){
                menu = menu->child[menu->select-1];
            }
        }
        else if (strcmp(pos.direction, "LEFT")==0 && status == 0){
            status = 1;
            if(menu->parent != NULL){
                menu = menu->parent;
            }
        }
        if(menu->name == "game"){
            play_game(&menu, from_node2);
        }


        if(joy_button(0)== 1){
            printf("%d, du trykket\n\r");
        };
        /* while(menu->name[i] != '\0'){
            printf("%c", menu->name[i]);
            i++;
        }*/

        //printf("pos.x: %d pos.y : %d direction: %s buttonpress: %d\r\n", pos.x, pos.y, pos.direction, joy_button(1));
        /*msg.data[0] = pos.x;
        msg.data[1] = pos.y;
        CAN_send(&msg);*/
        
        //message new = CAN_recieve();
        //printf("ID: %d\tDATA:%d\t%d\t%d\n\r", new.ID, new.data[0], new.data[1], new.data[2]);
        
        _delay_ms(100);
   
        
    }
    return 0;
}
