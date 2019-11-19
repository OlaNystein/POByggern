#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "CAN.h"
#include "menu.h"
#include "joystick.h"
#include "game2.h"
#include "oled.h"

static message m, tonode2;
static int start, exit_game, lives, points;

struct screen* start_game(struct screen* menu, char* direction, int* status, int difficulty){
    
    start = 1;
    exit_game = 0;
    lives = 3;
    points = 0;

    tonode2.ID = 1;
    tonode2.length = 6;
    tonode2.data[0] = 0;
    tonode2.data[1] = 0;
    tonode2.data[2] = 0;
    tonode2.data[3] = 0;
    tonode2.data[4] = start;
    tonode2.data[5] = difficulty;

    CAN_send(&tonode2);
    

    while(1){
        tonode2.ID = 1;
        tonode2.length = 6;
        tonode2.data[0] = joy_getPos().x;
        tonode2.data[1] = joy_getPos().y;
        tonode2.data[2] = joy_button(0);
        tonode2.data[3] = joy_getSliderPos().right;
        tonode2.data[4] = start;
        tonode2.data[5] = difficulty;

        CAN_send(&tonode2);
        

        if(exit_game){
            return menu;
        }

        if(joy_button(1)){ //right button
            menu = draw_screen(menu, joy_getPos().direction, &status, lives, points);
            int exit_pause = 0;
            _delay_ms(2000);
            while(!exit_pause){

                oled_refresh();

                if(joy_button(1)){

                    menu = draw_screen(menu, joy_getPos().direction, &status, lives, points);
                    exit_pause = 1;
                    _delay_ms(2000);
                    oled_refresh();
                }
                if(joy_button(0)){
                    menu = draw_screen(menu, joy_getPos().direction, &status, lives, points);
                    exit_pause = 1;
                    exit_game = 1;
                    _delay_ms(2000);
                    oled_refresh();
                    start = 0;
                }
            }

        }

      

        if(lives == 0){ 
            menu = draw_screen(menu, joy_getPos().direction, &status, lives, points);
            oled_refresh();
            while(!joy_button(0)){}
            _delay_ms(1000);
            menu = menu->parent;
            exit_game = 1;
            start = 0;
        } else {
            menu = draw_screen(menu, joy_getPos().direction, &status, lives, points);
            oled_refresh();
        }

        if(get_CAN().ID != 2){
            m = get_CAN();
            lives = m.data[0];
            points = m.data[1];
        }
    }
    return menu;
}
