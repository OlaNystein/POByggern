#ifndef GAME_H_
#define GAME_H_

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

#endif

int count_score(int* score, int signal, int* detected_goal, int* lives);
int start_game(void);

