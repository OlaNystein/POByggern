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
    CAN_send(&to_node2);
    //to_node2.data[0] = 
    joy_pos = joy_getDir();
    slider_pos = joy_getSliderPos();
    message msg;
    status = 0;
    int lives_status = 0;
    int exit_game = 0;
    uint8_t live = 3;
    //menu = draw_screen(menu, joy_pos.direction, &status, live);
    while(!exit_game){ 
        msg = CAN_recieve();
        oled_refresh();

        left_button_press = 0;
        to_node2.data[2] = left_button_press;
        //printf("menuname inside game: %s\n\r",menu->name);
        //menu = draw_screen(menu, joy_pos.direction, &status, live);
        
        printf("id: %d, data: %d\n\r", msg.ID, msg.data[1]);
        //lives = msg.data[0];
        //_delay_ms(2000);
        //live = 1;
        //menu = draw_screen(menu, joy_pos.direction, &status, live);
        //printf("msgID: %d lives: %d\n\r", msg.ID, live);        
        if (msg.ID == 2 && live!= msg.data[0] && msg.data[0] < 4 && msg.data[0] >= 0 && live != 0){
            printf("lives: %d\n\r", msg.data[0]);
            if(lives_status == 0){
                live = 3;
                lives_status = 1;
            }
            else{
                live = msg.data[0];
                menu = draw_screen(menu, joy_pos.direction, &status, live);
            }
            //menu = draw_screen(menu, joy_pos.direction, &status, live);
            printf("menuname inside game: %s\n\r",menu->name);
        }
        if(live == 0){
            oled_refresh();
            while(!joy_button(0)){
            }
            _delay_ms(1000);
            //menu = draw_screen(menu, joy_pos.direction, &status, live);
            menu = menu->parent;
            exit_game = 1;
            start_game = 0;
            to_node2.data[4] = start_game;
            
        }
        // må motta meldinger

        if(joy_button(0) && exit_game != 1){
            left_button_press = 1;
        }
        if(joy_button(1)){ //right button
            menu = draw_screen(menu, joy_pos.direction, &status, live);
            //menu = draw_screen(menu, direction, &status);
            int exit_pause = 0;
            _delay_ms(2000);
            while(!exit_pause){
                oled_refresh();
                printf("menuname inside game: %s\n\r",menu->name);
                joy_pos = joy_getDir();
                if(joy_button(1)){
                    menu = draw_screen(menu, joy_pos.direction, &status, live);
                    exit_pause = 1;
                    _delay_ms(2000);
                    oled_refresh();
                }
                if(joy_button(0)){
                    menu = draw_screen(menu, joy_pos.direction, &status, live);
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
        joy_pos = joy_getDir();
        slider_pos = joy_getSliderPos();
        to_node2.data[0] = joy_pos.x;
        to_node2.data[1] = joy_pos.y;
        to_node2.data[3] = slider_pos.right;
        to_node2.data[2] = left_button_press;
        
        CAN_send(&to_node2);
        //printf("start game: %d \n\r", start_game);
    }
    //draw you lost screen
    return menu;
}