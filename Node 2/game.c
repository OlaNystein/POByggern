#include <avr/io.h>
#include "util/delay.h"
#include "uart.h"
#include "ADC.h"
#include "setup.h"
#include "SPI.h"
#include <avr/interrupt.h>
#include "CAN.h"
#include "MCP.h"
#include "pwm.h"
#include "game.h"
#include "controller.h"
#include "joystick.h"
#include "solenoid.h"

unsigned int servo = 1155;
static int shoot_status;

int count_score(int* score, int signal, int* detected_goal, int* lives){
    if (signal < 15 && signal > 0 && *detected_goal == 0){
        *score += 1;
        *detected_goal = 1;
        if(*lives > 0){
            *lives -= 1;
        }
    }
    return 0;
}

int start_game(void){

    int score = 0;
    int detected_goal = 0;
    int lives = 3;
    shoot_status = 0;
    int shot_counter = 0;
    int points = 0;

    message msg;
    message to_node1;
    to_node1.ID = 2;
    to_node1.length = 3;
    to_node1.data[0] = lives;


   

    msg = get_CAN();
    set_difficulty(msg);

    CAN_send(&to_node1);
    while(msg.data[4] == 1){
        _delay_ms(50);
        msg = get_CAN();
        servo = pwm_pulse(servo, msg);

        shot_counter++;
        if(shot_counter > 50){
            shoot_status = 0;
        }
        if(msg.ID == 1 && msg.data[2] == 1 && shoot_status == 0){ 
            shoot_status = 1;
            shot_counter = 0;
            solenoid_pulse();
            points += 1;
        }

        //register goals
        uint16_t goal_signal = ADC_read();
        count_score(&score, goal_signal, &detected_goal, &lives);
        to_node1.data[0] = lives;

        to_node1.data[1] = points;
        if(detected_goal == 1 && goal_signal >= 220){
            CAN_send(&to_node1);
            detected_goal = 0;
        }
    }
}