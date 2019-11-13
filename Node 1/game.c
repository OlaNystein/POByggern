#include <avr/io.h>
#include "util/delay.h"
#include "uart.h"
#include <avr/interrupt.h>
#include "setup.h"
#include "CAN.h"
#include "game.h"
#include "joystick.h"
#include "menu.h"

int game_init(void){
    //init lcd skjerm
    return 0;
}

int play_game(screen* menu, message msg){

    message to_node2;
    joy_position joy_pos;
    slider_position slider_pos;
    int left_button_press;
    to_node2.ID = 1;
    to_node2.length = 5;
    to_node2.data[0] = 
    joy_pos = joy_getDir();
    slider_pos joy_getSliderPos();
    while(msg[0] > 0){
        //draw screen, lives og score.
        if(joy_button(0)){
            left_button_press = 1;
        }
        if(joy_button(1)){ //right button
            // draw "you quit the game"
            _delay_ms(1000);
            return 0;
        }
        to_node2[0] = joy_pos.x;
        to_node2[1] = joy_pos.y;
        to_node2[2] = slider_pos.left;
        to_node2[4] = left_button_press;
        CAN_send(&to_node2);

        left_button_press = 0;
        to_node2[4] = left_button_press;
    }
    //draw you lost screen
    return 0;
}