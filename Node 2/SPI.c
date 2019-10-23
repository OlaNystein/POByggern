#include <avr/io.h>
#include "util/delay.h"
#include <avr/interrupt.h>
#include "SPI.h"

/*
SS - PB0
MOSI - PB2
MISO - PB3
SCK - PB1
*/

int SPI_init(void){
    DDRB |= (1<<PB1) | (1<<PB2) | (1<<PB0) | (1<<PB7); //setting SCK, MOSI, SS as output pins.

    DDRB &= ~(1<<PB3); // setting MISO as input pin.

    SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) ; // activating SPI in mastermode and setting clk freq to osc/16


    return 0;
}

int SPI_transmit(char data){
    SPDR = data;

    while(!(SPSR & (1<<SPIF)));


    return 0;
}

uint8_t SPI_recieve(void){
    SPI_transmit(0);
    return SPDR;
}

int SPI_select(void){
    PORTB &= ~(1<<PB7);
    return 0;
}

int SPI_deselect(void){
    PORTB |= (1<<PB7);
    return 0;
}


