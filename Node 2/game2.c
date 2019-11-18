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
#include "game2.h"
#include "joystick.h"
#include "solenoid.h"

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
    message m = CAN_recieve();
    if (m.data[4] != 1){
        return;
    }
    
}