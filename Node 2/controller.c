#include "controller.h"
#include "TWI_Master.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "DAC.h"
#include <avr/interrupt.h>
#include "uart.h"

int controller_init(void){
    DDRH |= (1 << PH4);
    DDRH &= ~(1 << PH1);
    DDRH |= (1<<PH3) | (1<<PH5) | (1 << PH6);
    PORTH |= (1 << PH4);
    for(int i = 0; i<10000; i++){
    DAC_send(100);
    }
    DDRH |= (1 << PH1);
    printf("Controller initialized\r\n");
    return 0;
}

int two2dec(uint8_t twos) {
    if ((twos & 0x80) == 0){
        return twos;
    }
    return (uint8_t)(~(twos - 0x01))* -1;
}

void joy_to_voltage(uint8_t joy){
    int in = two2dec(joy);
    if(in >10){
        PORTH |= (1 << PH1);
        DAC_send(in);
    }
    else if (in < -10){
        PORTH &= ~(1 << PH1);
        DAC_send(in*(-1));
    }
    else {
        DAC_send(0);
    }
}

uint16_t controller_get_encoder_data(){
    uint16_t data;
    cli();
    DDRK = 0x00;
    PORTH &= ~(1<<PH5); //enable output of encoder
    PORTH &= ~(1<<PH3); //set SEL low to read high byte
    _delay_ms(20);
    data = (PINK <<8);
    PORTH |= (1<<PH3); //set SEL HIGH to read low byte
    _delay_ms(20);
    data |= PINK;
    //PORTH &= ~(1<<PH6); //toggle reset
    _delay_ms(20);
    PORTH |= (1<<PH6);
    PORTH |= (1 << PH5); //output disable
    sei();

    return data;
}



void init_timer(){
	//Trigger interrupt with interval of 100hz FQ 
	OCR3A = 10400;

	//Enable CTC mode
	TCCR3A |= (1 << COM3A0);

	//Prescale 8
	TCCR3B = (1 << CS31) | (1 << WGM32);	
	
	//Enable compare match A interrupt
	EIMSK |= (1 << OCIE3A);
}

ISR(TIMER3_COMPA_vect)
{
}