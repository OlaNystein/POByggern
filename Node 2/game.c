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
#include "joystick.h"
#include "solenoid.h"

unsigned int servo = 1155;

int count_score(int* score, int signal, int* detected_goal, int* lives){
    if (signal < 15 && signal > 0 && *detected_goal == 0){
        printf("signal: %d GOAL!!!\r\n", signal);
        *score += 1;
        *detected_goal = 1;
        if(*lives > 0){
            *lives -= 1;
        }
    }
    if (signal >= 220 && *detected_goal == 1){
        //*detected_goal = 0;
    }
    return 0;
}

int start_game(void){

    int score = 0;
    int detected_goal = 0;
    int lives = 3;
    int shoot_status = 0;
    int shot_counter = 0;
    int points = 0;

    message msg;
    message to_node1;
    to_node1.ID = 2;
    to_node1.length = 3;
    to_node1.data[0] = lives;


   

    msg = CAN_recieve();
    while(msg.ID = 1 && msg.data[4] == 1){

        if(CAN_recieve().ID == 1){
            msg = CAN_recieve();
            servo = pwm_pulse(servo, msg);
        }

        shot_counter++;
        if(shot_counter > 60){
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