#include "controller.h"
#include "TWI_Master.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "DAC.h"
#include <avr/interrupt.h>

int solenoid_init(void){
    DDRB |= (1 << PB6);
    PORTB |= (1 << PB6);
    return 0;
}

int solenoid_pulse(void){ //run this when controller button press
    PORTB &= ~(1 << PB6);
    _delay_ms(700);
    PORTB |= (1 << PB6);
    return 0;
}