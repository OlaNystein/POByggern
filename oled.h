#ifndef OLED_H_
#define OLED_H_

uint8_t page, col;

void oled_command(uint8_t command);
int oled_init(void);
int oled_printChar(char c);



#endif