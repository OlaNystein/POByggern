#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "oled.h"
#include "fonts.h"
//#include "sram.h"

volatile char *oled_cmd = (char *) 0x1000;
volatile char *oled_data = (char *) 0x1200;

uint8_t page, col;

void oled_command(uint8_t command){
    *oled_cmd = command;
}

int oled_init(void){

    //MCUCR |= (1<<SRE);
    //SFIOR |= (1<<XMM2);

    //Oppsett av oled
    oled_command(0xAE); //display off
    oled_command(0xA1); //segment remap
    oled_command(0xDA); //common pads hardware: alternative
    oled_command(0x12);
    oled_command(0xC8); //common output scan direction:com63-com0
    oled_command(0xA8); //multiplex ration mode:63
    oled_command(0x3F);
    oled_command(0xD5); //display divide ratio/osc. freq. mode
    oled_command(0x80);
    oled_command(0x81); //contrast control
    oled_command(0x50);
    oled_command(0xD9); //set pre-charge period
    oled_command(0x21); 
    oled_command(0x20); //Set memory addressing mode
    oled_command(0x02);
    oled_command(0xDB); //VCOM deselect level mode
    oled_command(0x30); 
    oled_command(0xAD); //master configuration
    oled_command(0x00);
    oled_command(0xA4); //out follows RAM content
    oled_command(0xA6); //set normal display
    oled_command(0xAF); //display on

    /* oled_command(0xB0); //Set page start address
    oled_command(0x00); //Set lower column start address
    oled_command(0x10); //Set higher column start address */

    return 0;

}

int oled_printChar(char c){
    
    for (int i = 0; i < 8; i++){
        *oled_data = pgm_read_byte(&font8[c][i]);
    }
    return 0;
}