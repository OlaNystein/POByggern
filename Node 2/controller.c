#include "controller.h"
#include "TWI_Master.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "DAC.h"
#include <avr/interrupt.h>

int controller_init(void){
    DDRH |= (1 << PH4);
    DDRH |= (1 << PH1);
    //DDRH &= ~(1 << DDH1);
    DDRH |= (1<<PH3) | (1<<PH5) | (1 << PH6);
    PORTH |= (1 << PH4);
    return 0;
}

int two2dec(uint8_t twos) 
{
    int decimalNumber = 0, i = 0, remainder;
    while (n!=0)
    {
        remainder = n%10;
        n /= 10;
        decimalNumber += remainder*pow(2,i);
        ++i;
    }
    return decimalNumber;

    return sum;
}

void joy_to_voltage(uint8_t joy){
    int in = two2dec(joy);
    printf("%d\r\n", in);
    //if (joy < 101 && joy > 5)
    if(in >10){
        //double inn = (joy/100)*150;
        //printf("inn: %f\n\r", inn);
        //int in = (int)(joy/100)*150;
        //joy = 100;
        //printf("in: %d\n\r", in);
        PORTH |= (1 << PH1);
        DAC_send(in);
    }
    //else if (joy < 250 && joy > 155)
    else if (in < -10){
        //int in = (int)((-joy+256)/100)*150;
        //joy = 100;
        //printf("in: %d\n\r", in);
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