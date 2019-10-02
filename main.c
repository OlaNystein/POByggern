#include <avr/io.h>
#define F_CPU 4915200UL
#include "util/delay.h"
#include "uart.h"
#include "sram.h"
#include "ADC.h"
#include "joystick.h"
#include "setup.h"
#include "oled.h"
#include "menu.h"
#include "SPI.h"
#define FOSC 4915200UL
#define BAUD 9600
#define MYUBURR FOSC/16/BAUD-1


int main(void){
    UART_Init(MYUBURR);
    MCUCR |= (1<<SRE);
    SFIOR |= (1<<XMM2);
    //sei();
    //ADC_init();
    //SRAM_test();
    //PORTA = 0x01;
    oled_init();
   _delay_ms(2000);
   //oled_command(0xa5);
   //oled_clear();
   //oled_pos(1,4);
   //oled_goto_line(0);
  struct screen* main_menu = init_menu();
  //printf("select: %d", main_menu->select);
  
   //oled_goto_line(2);
   //oled_pos(1,10);
   //oled_printString("Stokk");
   //oled_clear_line(1);
    int i = 0;
    int j = 0;
    joy_position pos;
    //oled_printChar('A');
    int* status = 0;
        

    SPI_init();
    
    while(1){
        printf("før");
        SPI_select();
        SPI_transmit('a');
        SPI_deselect();
        _delay_ms(500);
        printf("etter");
        //pos = joy_getDir();
        //draw_screen(main_menu, pos.direction, &status);
        //printf("%s", lastpos);
        //oled_refresh();

        //printf("%s", pos.direction);
        //if (pos.direction != "NEUTRAL"){
        //    draw_screen(main_menu, pos.direction);
        //}
        //printf("%d", button_select(main_menu));
        //button_select(main_menu);

        //printf("Joy pos\t x: %d\t y: %d\t dir: %s\n\r", pos.x, pos.y, pos.direction);
        //slider_position spos;
        //spos = joy_getSliderPos();
        //printf("Slidepos\tleft: %d\tright: %d\n\r", spos.left, spos.right);
        //volatile char *ext_ram = (char *) 0x1400;
        //ext_ram[0] = 1;

        /* oled_pos(j, i);
        oled_printString("0123456789ABCDEF");
        _delay_ms(500);
        //oled_clear_line(j);
        i++;
        j++;
        if (j == 8){j = 0;}
        if (i == 16){i = 0;}
        _delay_ms(1000);*/

        //PORTA ^= (1<<PA0);   
        //PORTE = ~(1<<PE1);
        //_delay_ms(500);
        //PORTE = ~(0<<PE1);
        //_delay_ms(500);
    }
    return 0;
}
