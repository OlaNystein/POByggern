#include "setup.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "oled.h"
#include "menu.h"
#include "joystick.h"
#include "sram.h"

struct screen main_menu;
static int joystick_push;

struct screen* init_menu(void){
    joystick_push = 0;

    main_menu.name = "---Main  Menu---";
    main_menu.parent = NULL;
    main_menu.child[0] = NULL;
    main_menu.child[1] = NULL;
    main_menu.child[2] = NULL;
    main_menu.select = 1;

    return &main_menu;
}

void draw_screen(struct screen* display, char* direction){
    oled_sram_clear();
    oled_pos(0, 0);
    oled_sram_printString(main_menu.name);
    oled_pos(1,2);
    oled_sram_printString("option1");
    oled_pos(2, 2);
    oled_sram_printString("option2");
    oled_pos(3, 2);
    oled_sram_printString("option3");
    //printf("%d", strcmp(direction, "UP"));

    if(strcmp(direction, "UP") == 0)  {
        if(display->select > 1){
            display->select = display->select -1;
            //printf("UP: %d\n\r", display->select);
        }
    } else if(strcmp(direction, "DOWN") == 0){
        if(display->select < 3){
            display->select = display->select + 1;
            //printf("DOWN: %d\r\n", display->select);
        }
    }
    oled_pos(display->select, 0);
    oled_sram_print_char('X');
}

int button_select(struct screen* display){
    if (joy_button(1)){
        //printf("select: %d\r\n", main_menu.select);
        return (display->select);
    }
    return 0;
}