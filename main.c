#include <avr/io.h>
#define F_CPU 4915200UL
#include "util/delay.h"
#include "uart.h"
#include "SRAMtest.h"
#define FOSC 4915200UL
#define BAUD 9600
#define MYUBURR FOSC/16/BAUD-1

int main(void){
    UART_Init(MYUBURR);
    MCUCR |= (1<<SRE);
    SFIOR|= (1<<XMM2);
    SRAM_test();
    //PORTA = 0x01;
    //volatile char *ext_ram = (char *) 0x1800;
    //ext_ram[16] = 1;
    while(1){
        //volatile char *ext_ram = (char *) 0x1800;
        
        //PORTA ^= (1<<PA0);   
        //PORTE = ~(1<<PE1);
        //_delay_ms(500);
        //PORTE = ~(0<<PE1);
        //_delay_ms(500);
    }
    return 0;
}
