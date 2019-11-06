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

int count_score(int* score, int signal, int* detected_goal){
    if (signal < 150 && signal > 0 && *detected_goal == 0){
        printf("GOAL!!!\r\n");
        *score += 1;
        *detected_goal = 1;
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

int play_game(message msg){
    game_init();

    int score = 0;
    int detected goal = 0;
    int lives = 3;
    
    message to_node1;
    to_node1.ID = 2;
    to_node1.length = 3;
    to_node1.data[0] = lives;

    while(lives){
        //få inn styring av spillet

        if(msg[2] == 1){ //register left button press sent over CAN bus
            solenoid_pulse();
        }
        uint16_t goal_signal = ADC_read()
        count_score(&score, goal_signal, &detected_goal);
        if(detected_goal == 1){
            lives = lives - score;
            to_node1.data[0] = lives;
            CAN_send(&to_node1);//fjerne ett liv på oled skjermen!! kan det sendes over CAN bussen?
            _delay_ms(1000); // ett lite delay før spillet kjører igjen
        }
        


    }
    to_node1[0] = 0; // setter lives = 0 
    CAN_send(to_node1); // sender 0 liv til node 1, må printe you lost på oled skjermen

}