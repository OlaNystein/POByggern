#include <avr/io.h>
#define F_CPU 4915200UL
#include "util/delay.h"
#include "uart.h"
#include "SRAMtest.h"
#include "ADC.h"
#include "joystick.h"
#include "setup.h"
#include "oled.h"
#define FOSC 4915200UL
#define BAUD 9600
#define MYUBURR FOSC/16/BAUD-1

int main(void){
    UART_Init(MYUBURR);
    MCUCR |= (1<<SRE);
    SFIOR |= (1<<XMM2);
    //ADC_init();
    //SRAM_test();
    //PORTA = 0x01;
    oled_init();
   _delay_ms(1000);

    
    while(1){
        //joy_position pos;
        //pos = joy_getDir();
        //printf("Joy pos\t x: %d\t y: %d\t dir: %s\n\r", pos.x, pos.y, pos.direction);
        //slider_position spos;
        //spos = joy_getSliderPos();
        //printf("Slidepos\tleft: %d\tright: %d\n\r", spos.left, spos.right);
        //volatile char *ext_ram = (char *) 0x1400;
        //ext_ram[0] = 1;
     
        //oled_command(0xa5);
        //oled_printChar('a');
        _delay_ms(1000);
        //PORTA ^= (1<<PA0);   
        //PORTE = ~(1<<PE1);
        //_delay_ms(500);
        //PORTE = ~(0<<PE1);
        //_delay_ms(500);
    }
    return 0;
}
