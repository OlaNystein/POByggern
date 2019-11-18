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

struct screen* start_game(struct screen* menu, char* direction, int* status, int difficulty){
    message to_node2;
    joy_position joy_pos;
    slider_position slider_pos;

    to_node2.ID = 2;
    to_node2.length = 5;
    int start_game = 1;
    to_node2.data[4] = start_game;
    int exit_game = 0;
    
    while(!exit_game){
        CAN_send(&to_node2);
        
    }


}
