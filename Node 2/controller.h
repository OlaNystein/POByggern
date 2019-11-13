#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "TWI_Master.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "DAC.h"
#include "CAN.h"
#include <avr/interrupt.h>

typedef struct reg_t{
    double Kp;
    char* diff;
}reg;

int controller_init(void);
void joy_to_voltage(uint8_t joy);
int controller_get_encoder_data(void);
int two2dec(uint8_t twos);
void init_timer();
void calibrate_encoder(void);
void joy_to_voltage2(int joy);
void PID(message m);

#endif 