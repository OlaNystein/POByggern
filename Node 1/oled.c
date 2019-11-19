#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "oled.h"
#include "fonts.h"
#include "sram.h"

volatile char *oled_cmd = (char *) 0x1000;
volatile char *oled_data = (char *) 0x1200;

uint8_t page, column;

void oled_command(uint8_t command){
    *oled_cmd = command;
}

int oled_init(void){



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

    
    oled_clear();
    oled_home();
    
    return 0;

}

int oled_home(void){
    page = 0;
    column = 0;

    oled_command(0x21);
    oled_command(0x00);
    oled_command(0x7F);
    
    oled_command(0x22);
    oled_command(0x00);
    oled_command(0x7);   
    return 0;
}

int oled_goto_line(uint8_t line){
    oled_home();

    if(line < 8){
        page = line;
        oled_command(0x00);
        oled_command(0x10);
        oled_command(0xB0 | line);
    }

    return 0;
}

int oled_goto_col(uint8_t col){

    if(col < 128/8){
        column = col*8;
        oled_command(0x00 + ((col*8) & 0b00001111));
        oled_command(0x10 + ((col*8)>>4));
    }
    return 0;
}

int oled_pos(uint8_t line, uint8_t col){
    oled_goto_line(line);
    oled_goto_col(col);
    return 0;
}

int oled_clear_line(uint8_t line){
    oled_goto_line(line);

    for (int i = 0; i < 128; i++){
        *oled_data = 0x00;
    }
    oled_home();

    return 0;
}

int oled_clear(void){
    for (int i = 0; i < 8; i++){
        oled_clear_line(i);
    }
    return 0;
}


int oled_printChar(char c){
    
    for (int i = 0; i < 8; i++){
        *oled_data = pgm_read_byte(&font8[c-' '][i]);
    }
    return 0;
}

int oled_printString(char *s){
    while(*s != '\0'){
        oled_printChar(*s);
        s++;
    }
    return 0;
}

int oled_sram_clear_line(uint8_t line){
    oled_goto_line(line);

    for (int i = 0; i<128; i++){
        SRAM_write(page*128 + i, 0);
    }

    oled_home();
    return 0;
}

int oled_sram_clear(void){
    for(int j = 0; j<8; j++){
        oled_sram_clear_line(j);
    }
    return 0;
}

int oled_sram_print_char(char c){
    for (int i = 0; i < 8; i++){
        
        SRAM_write(page*128+column+i, pgm_read_byte(&font8[c-' '][i]));
    }
    column += 8;

    return 0;
}

int oled_sram_printString(char* s){
    while(*s != '\0'){
        oled_sram_print_char(*s);
        s++;
    }
    return 0;
}

void oled_refresh(void){
    for(int i = 0; i < 8; i++){
        oled_goto_line(i);
        for(int j = 0; j < 128; j++){
            *oled_data =  SRAM_read(i*128+j);
        }
    }
}
