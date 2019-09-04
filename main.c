#include <avr/io.h>
#define F_CPU 4915200UL
#include "util/delay.h"
#include "uart.h"
#define FOSC 4915200UL
#define BAUD 9600
#define MYUBURR FOSC/16/BAUD-1

int main(void){
    //UART_Init(MYUBURR);
    //MCUCR |= (1<<SRE);
    DDRE = 0;
    DDRA = 0;
    //DDRA = (0<<DDA0);
    while(1){
        PORTA ^= (1<<PA0); //& (0<<PA1) & (0<<PA2) & (0<<PA3) & (0<<PA4) & (0<<PA5) & (0<<PA6) & (0<<PA7);     
        PORTE = ~(1<<PE1);
        _delay_ms(500);
        PORTE = ~(0<<PE1);
        _delay_ms(500);
        //PORTE = (1<<PE1);
        //printf(UART_Recieve());
        //printf("Entered %c\r\n", UART_Recieve());
}
    return 0;
}
