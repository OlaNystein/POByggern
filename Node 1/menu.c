#include "setup.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "oled.h"
#include "menu.h"
#include "joystick.h"
#include "sram.h"


struct screen main_menu, play_game, high_scores, options;


struct screen* init_menu(void){

    main_menu.name = "main";
    main_menu.parent = NULL;
    main_menu.child[0] = &play_game;
    main_menu.child[1] = &high_scores;
    main_menu.child[2] = &options;
    main_menu.select = 1;

    play_game.name = "game";
    play_game.parent = &main_menu;
    play_game.child[0] = NULL;
    play_game.child[1] = NULL;
    play_game.child[2] = NULL;
    play_game.select = 1;

    high_scores.name = "hs";
    high_scores.parent = &main_menu;
    high_scores.child[0] = NULL;
    high_scores.child[1] = NULL;
    high_scores.child[2] = NULL;
    high_scores.select = 1;

    options.name = "options";
    options.parent = &main_menu;
    options.child[0] = NULL;
    options.child[1] = NULL;
    options.child[2] = NULL;
    options.select = 1;

    return &main_menu;
}

void draw_screen(struct screen* display, char* direction, int* status){
    if (direction == "NEUTRAL"){
        *status  = 0;
    }
    //printf("yo\r\n");
    //printf("\n\rstatus: %d", *status);
    /* oled_sram_clear();
    oled_pos(0, 0);
    oled_sram_printString(main_menu.name);
    oled_pos(1,2);
    oled_sram_printString("option1");
    oled_pos(2, 2);
    oled_sram_printString("option2");
    oled_pos(3, 2);
    oled_sram_printString("option3");
    if((strcmp(direction, "UP") == 0) && *status == 0)  {
        *status = 1;
        if(display->select > 1){
            display->select = display->select -1;
            //printf("UP: %d\n\r", display->select);
        }
    } else if((strcmp(direction, "DOWN") == 0) && *status == 0){
        *status = 1;
        if(display->select < 3){
            display->select = display->select + 1;
            //printf("DOWN: %d\r\n", display->select);
        }
    }
    oled_pos(display->select, 0);
    oled_sram_print_char('X');*/

    if (strcmp(display->name, "main") == 0){
        //printf("yo\r\n");
        oled_sram_clear();
        oled_pos(0, 0);
        oled_sram_printString("---Main  Menu---");
        oled_pos(1,2);
        oled_sram_printString("Play Game");
        oled_pos(2, 2);
        oled_sram_printString("High Scores");
        oled_pos(3, 2);
        oled_sram_printString("Settings");

        if((strcmp(direction, "UP") == 0) && *status == 0)  {
            *status = 1;
            if(display->select > 1){
                display->select = display->select -1;
                //printf("UP: %d\n\r", display->select);
            }
        } else if((strcmp(direction, "DOWN") == 0) && *status == 0){
            *status = 1;
            if(display->select < 3){
                display->select = display->select + 1;
                //printf("DOWN: %d\r\n", display->select);
            }
        }
        oled_pos(display->select, 0);
        oled_sram_print_char('@');
    }
    else if (strcmp(display->name, "game") == 0){
        int i = 0;
        
        oled_sram_clear();
        oled_pos(0, 0);
        oled_sram_printString("--Playing Game--");
        oled_pos(2, 1);
        oled_sram_printString("Lives: ###");
        oled_pos(7, 1);
        oled_sram_printString("Score: 0");
        display->select = -1;
    }
    else if (strcmp(display->name, "hs") == 0){
        oled_sram_clear();
        oled_pos(0, 0);
        oled_sram_printString("--High  Scores--");
        oled_pos(1, 1);
        oled_printString("1. Ola-------15");
        oled_pos(2, 1);
        oled_printString("2. Petter----12");
        oled_pos(7, 1);
        oled_printString("Back");
        oled_pos(7,0);
        oled_printChar('@');
        display->select = -1;
    }
    else if (strcmp(display->name, "options") == 0){
        oled_sram_clear();
        oled_pos(0, 0);
        oled_sram_printString("----Settings----");
        oled_pos(1,2);
        oled_sram_printString("Music");
        oled_pos(2, 2);
        oled_sram_printString("Font");
        oled_pos(3, 2);
        oled_sram_printString("Back");

        if((strcmp(direction, "UP") == 0) && *status == 0)  {
            *status = 1;
            if(display->select > 1){
                display->select = display->select -1;
                //printf("UP: %d\n\r", display->select);
            }
        } else if((strcmp(direction, "DOWN") == 0) && *status == 0){
            *status = 1;
            if(display->select < 3){
                display->select = display->select + 1;
                //printf("DOWN: %d\r\n", display->select);
            }
        }
        
        oled_pos(display->select, 0);
        oled_sram_print_char('@');

    }
}

int button_select(struct screen* display){
    if (joy_button(1) == 2){
        //printf("select: %d\r\n", main_menu.select);
        return (display->select);
        
    }
    return 0;
}