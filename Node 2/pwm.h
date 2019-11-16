#include "CAN.h"
#ifndef PWM_H_
#define PWM_H_

int pwm_init(void);
unsigned int pwm_pulse(unsigned int servo, message m);

#endif