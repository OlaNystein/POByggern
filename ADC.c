
#define F_CPU 4915200UL
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "setup.h"
#include "ADC.h"
#include "util/delay.h"

int ADC_init(void) {
    //MCUCR |= (1<<SRE);
    //SFIOR |= (1<<XMM2);

    DDRB &= ~(1<<PINB2);

    return 0;
}

/* 
    Channel 0 -y
    Channel 1 -x
    Channel 2 -r-slider
    Channel 3 -l-slider
 */

uint8_t ADC_read(uint8_t channel) {
    volatile char *adc = (char *) 0x1400; //1400

    if (channel > 3) {return 0;}

    *adc =  0x04 | channel;
    //channel from 0 to 2

    //while(test_bit(PINB, PINB2));
    _delay_us(60);
   // volatile uint8_t readout = adc[0];
    //_delay_us(1);
    return *adc;
}

