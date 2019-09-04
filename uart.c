#include <avr/io.h>
#define F_CPU 4915200UL
#include "util/delay.h"
#include "uart.h"

FILE *UART;

void UART_Init(unsigned int ubrr){
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);

    UART = fdevopen(&UART_Transmit, &UART_Recieve);
}

void UART_Transmit(unsigned char data, FILE *f){
    while(!(UCSR0A & (1<<UDRE0)))
        ;
    UDR0 = data;
}

unsigned char UART_Recieve(){
    while(!(UCSR0A & (1<<RXC0)))
        ;
        printf("%c", UDR0);
    return UDR0;
}

int UART_print(char * string){
    int i = 0;
    while(string[i] != 0x00){
        UART_Transmit(string[i], UART);
        i++;
    }
    return 0;
}



