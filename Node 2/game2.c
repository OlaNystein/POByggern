#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "ADC.h"
#include "setup.h"
#include "SPI.h"
#include <avr/interrupt.h>
#include "CAN.h"
#include "MCP.h"
#include "pwm.h"
#include "game2.h"
#include "joystick.h"
#include "solenoid.h"

static message m, tonode1;
static int lives, detected_goal, points;

int count_score(int signal, int detected_goal, int lives){
    if (signal < 15 && signal > 0 && detected_goal == 0){
        detected_goal = 1;
        if(lives > 0){
            lives -= 1;
        }
    }
    if (signal >= 220 && detected_goal == 1){
        detected_goal = 0;
    }
    return 0;
}


int start_game(void){
    lives = 3;
    detected_goal = 0;
    points = 0;

    tonode1.ID = 2;
    tonode1.length = 2;
    tonode1.data[0] = lives;
    tonode1.data[1] = points;
    while(1){
        uint16_t signal = ADC_read();
        count_score(signal, detected_goal, lives);
        tonode1.data[0] = lives;
        tonode1.data[1] = points;
        if (signal >= 220 && detected_goal == 1){
            CAN_send(&tonode1);
        }
        
        if(get_CAN().ID == 1){
            m = get_CAN();
        }
        if(m.data[4] == 0){
            return 0;
        }


    }
}