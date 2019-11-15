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


int game_init(void){
    //init lcd skjerm
    return 0;
}

struct screen* start_game(struct screen* menu, char* direction, int* status){
    message to_node2;
    joy_position joy_pos;
    slider_position slider_pos;
    int left_button_press = 0;
    int start_game = 1;
    to_node2.ID = 1;
    to_node2.length = 5;
    to_node2.data[4] = start_game;
    //to_node2.data[0] = 
    joy_pos = joy_getDir();
    slider_pos = joy_getSliderPos();
    message msg = CAN_recieve();
    status = 0;
    int exit_game = 0;
    int lives;
    while(!exit_game){ //msg.data[0] > 0){
        msg = CAN_recieve();
        if (msg.ID = 2){
            printf("lives: %d\n\r", msg.data[0]);
            lives = msg.data[0];
            //oled_sram_clear();
            //menu = draw_screen(menu, joy_pos.direction, &status, lives);
        }
        // må motta meldinger
        oled_refresh();
        joy_pos = joy_getDir();
        slider_pos = joy_getSliderPos();
        if(joy_button(0)){
            left_button_press = 1;
        }
        if(joy_button(1)){ //right button
            menu = draw_screen(menu, joy_pos.direction, &status);
            //menu = draw_screen(menu, direction, &status);
            int exit_pause = 0;
            _delay_ms(2000);
            while(!exit_pause){
                oled_refresh();
                joy_pos = joy_getDir();
                if(joy_button(1)){
                    menu = draw_screen(menu, joy_pos.direction, &status);
                    exit_pause = 1;
                    _delay_ms(2000);
                    oled_refresh();
                }
                if(joy_button(0)){
                    menu = draw_screen(menu, joy_pos.direction, &status);
                    exit_pause = 1;
                    exit_game = 1;
                    _delay_ms(2000);
                    oled_refresh();
                    start_game = 0;
                    to_node2.data[4] = start_game;
                }
                //printf("you quit the game\n\r");// draw "you quit the game"
                //_delay_ms(1000);
            }

        }
        to_node2.data[0] = joy_pos.x;
        to_node2.data[1] = joy_pos.y;
        to_node2.data[3] = slider_pos.right;
        to_node2.data[2] = left_button_press;
        
        
        CAN_send(&to_node2);

        left_button_press = 0;
        to_node2.data[2] = left_button_press;
        //printf("start game: %d \n\r", start_game);
    }
    //draw you lost screen
    return menu;
}