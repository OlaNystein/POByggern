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
    if (signal < 100 && signal > 0 && *detected_goal == 0){
        printf("GOAL!!!\r\n");
        *score += 1;
        *detected_goal = 1;
        if(*lives > 0){
            *lives -= 1;
        }
        printf("reallives: %d\r\n",*lives);
    }
    if (signal >= 220 && *detected_goal == 1){
        *detected_goal = 0;
    }
    return 0;
}

int game_init(void){
    // draw et funny screen på lcd'en og fyr opp livene
    // initialiser motorposisjon og enkoder
    //initialiser alt rett og slett
    return 0;
}

int start_game(void){
    //game_init();
    int score = 0;
    int detected_goal = 0;
    int lastscore = 0;
    int lives = 3;
    int shoot_status = 0;
    int shot_counter = 0;
    int points = 0;
    message msg;
    msg = CAN_recieve();
    message to_node1;
    to_node1.ID = 2;
    to_node1.length = 3;
    to_node1.data[0] = lives;
    CAN_send(&to_node1);
    calibrate_encoder();
    int just_started = 1;
    //printf("run game: %d\n\r", msg.data[4]);
    while(msg.data[4] == 1 || just_started == 1){
        just_started = 0;
        shot_counter++;
        //printf("counter: %d\n\r", counter);
        if(shot_counter > 60){
            shoot_status = 0;
        }
        msg = CAN_recieve();
        //printf("exit: %d\n\r", msg.data[4]);
        //printf("solenoid: %d\n\r", msg.data[2]);
        printf("run game: %d\n\r", msg.data[4]);
        PID(msg);
        servo = pwm_pulse(servo, msg);

        if(msg.data[2] == 1 && shoot_status == 0){ //register left button press sent over CAN bus
            shoot_status = 1;
            shot_counter = 0;
            printf("SHOOT\n\r");
            solenoid_pulse();
            points += 1;
        }
        uint16_t goal_signal = ADC_read();
        printf("goal signal: %d\n\r", goal_signal);
        count_score(&score, goal_signal, &detected_goal, &lives);
        to_node1.data[0] = lives;
        to_node1.data[1] = points;
        //printf("lives: %d\n\r", to_node1.data[0]);
        CAN_send(&to_node1);//fjerne ett liv på oled skjermen!! kan det sendes over CAN bussen?
        //_delay_ms(2000); // ett lite delay før spillet kjører igjen
        


    }
    //to_node1.data[0] = 0; // setter lives = 0 
    //CAN_send(&to_node1); // sender 0 liv til node 1, må printe you lost på oled skjermen

}