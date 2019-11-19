#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "CAN.h"
#include "menu.h"
#include "joystick.h"
#include "game.h"
#include "oled.h"


struct screen* start_game(struct screen* menu, char* direction, int* status, int difficulty){
    
    joy_position joy_pos;
    slider_position slider_pos;
    
    message to_node2;
    to_node2.ID = 1;
    to_node2.length = 6;
    to_node2.data[5] = difficulty;

    int left_button_press = 0;
    int start_game = 1;
    status = 0;
    int lives_status = 0;
    int exit_game = 0;
    int live = 3;
    int points = 0;

    message msg;
    while(!exit_game){ 
        _delay_ms(50);
        oled_refresh();
    
        if(live == 0){ // avslutter om liv == 0
            oled_refresh();
            while(!joy_button(0)){
            }
            _delay_ms(1000);
            menu = menu->parent;
            exit_game = 1;
            start_game = 0;
        }
        
        joy_pos = joy_getDir();
        slider_pos = joy_getSliderPos();
        to_node2.data[0] = joy_pos.x;
        to_node2.data[1] = joy_pos.y;
        to_node2.data[3] = slider_pos.right;
        to_node2.data[2] = left_button_press;
        to_node2.data[4] = start_game;
        CAN_send(&to_node2);


        
        left_button_press = 0;
        to_node2.data[2] = left_button_press;

        if(joy_button(0) && exit_game != 1){
            left_button_press = 1;
        }


        if(joy_button(1)){ //right button
            menu = draw_screen(menu, joy_pos.direction, &status, live, points);
            int exit_pause = 0;
            _delay_ms(2000);
            while(!exit_pause){
                oled_refresh();
                joy_pos = joy_getDir();
                if(joy_button(1)){
                    menu = draw_screen(menu, joy_pos.direction, &status, live, points);
                    exit_pause = 1;
                    _delay_ms(2000);
                    oled_refresh();
                }
                if(joy_button(0)){
                    menu = draw_screen(menu, joy_pos.direction, &status, live, points);
                    exit_pause = 1;
                    exit_game = 1;
                    _delay_ms(2000);
                    oled_refresh();
                    start_game = 0;
                    to_node2.data[4] = start_game;
                    CAN_send(&to_node2);
                }
            }

        }
        msg = get_CAN();
        if(msg.ID == 2){
            points = msg.data[1];
        }
        if (msg.ID == 2 && live!= msg.data[0] && msg.data[0] < 4 && msg.data[0] >= 0){

            if(lives_status == 0 && live == 3 && msg.data[0] != 3){
                lives_status = 1;
                live = 3;
                _delay_ms(2999);
            }

            else{
                live = msg.data[0];
                menu = draw_screen(menu, joy_pos.direction, &status, live, points);
            }
        }

        
    }
    return menu;
}