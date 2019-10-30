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