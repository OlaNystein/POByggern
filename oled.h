#ifndef OLED_H_
#define OLED_H_

uint8_t page, column;

void oled_command(uint8_t command);
int oled_init(void);
int oled_home(void);
int oled_goto_line(uint8_t line);
int oled_goto_col(uint8_t col);
int oled_pos(uint8_t line, uint8_t col);
int oled_clear_line(uint8_t line);
int oled_clear(void);
int oled_printChar(char c);
int oled_printString(char *s);
int oled_sram_clear_line(uint8_t line);
int oled_sram_clear(void);
int oled_sram_print_char(char c);
int oled_sram_printString(char* s);
void oled_refresh(void);


#endif