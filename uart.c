#include <avr/io.h>
#define F_CPU 4915200UL
#include "util/delay.h"
#include "uart.h"



void UART_Init(unsigned int ubrr){
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0A = (1<<RXEN0)|(1<<TXEN0);
    UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
}

void UART_Transmit(unsigned char data){
    while(!(UCSR0A & (1<<UDRE0)))
        ;
    UDR0 = data;
}

unsigned char UART_Recieve(void){
    while(!(UCSR0A & (1<<RXC0)))
        ;
    return UDR0;
}




