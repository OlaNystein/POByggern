#include <avr/io.h>
#define F_CPU 4915200UL
#include "util/delay.h"
#include "uart.h"


int main(void){
    UART_Init(MYUBURR);
    UART_Transmit('p');
    //UART_Recieve();

    DDRB = (1<<DDB0);
    while(1){
        PORTB ^= (1<<PB0);
        _delay_ms(500);
    }

    return 0;
}
