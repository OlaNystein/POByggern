
#define F_CPU 4915200UL
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "setup.h"
#include "ADC.h"
#include "util/delay.h"
#include <stdbool.h>
#include <avr/interrupt.h>

volatile bool ccF = false;

int ADC_init(void) {

    DDRF &= ~(1 << PF0);

    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);

    

    //ADCSRA |= (1 << ADIE);
    // | (1 << ADATE);
    ADCSRA |= (1 << ADEN);
    sei();
    //ADCSRA |= (1 << ADSC);
    
    //ADCSRB |= (1 << ADTS0);
    //ADCSRB &= ~(1 << ADTS0) & ~(1 << ADTS2);
    return 0;
}



uint16_t ADC_read(void){
    ADCSRA |= (1 << ADSC);
    if (ccF == true){
        ADCSRA |= (1 << ADSC);
        ccF = false;
        printf("Test");
        return ADC;
    }
    return ADC;
}

ISR(ADC_vec){
    ccF = true;
}




