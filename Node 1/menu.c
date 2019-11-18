#include "setup.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "oled.h"
#include "menu.h"
#include "joystick.h"
#include "sram.h"
#define F_CPU 4915200UL

struct screen main_menu, play_game, high_scores, options, pause_menu;


struct screen* init_menu(void){

    main_menu.name = "main";
    main_menu.parent = NULL;
    main_menu.child[0] = &play_game;
    main_menu.child[1] = &high_scores;
    main_menu.child[2] = &options;
    main_menu.select = 1;

    play_game.name = "game";
    play_game.parent = &main_menu;
    play_game.child[0] = &pause_menu;
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

    pause_menu.name = "pause_menu";
    pause_menu.parent = &play_game;
    pause_menu.child[0] = NULL;
    pause_menu.child[1] = NULL;
    pause_menu.child[2] = NULL;
    pause_menu.select = 1;

    return &main_menu;
}

struct screen* draw_screen(struct screen* display, char* direction, int* status, int lives, int points){
    if (direction == "NEUTRAL"){
        *status  = 0;
    }
    //*status = 0;
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
    if(joy_button(1) && *status == 0 && strcmp(display->name, "game") == 0){
        *status = 1;
        if(display->child[0] != NULL){
            display = display->child[0];
        }
    }
    //printf("status %d, dir: %d\n\r", *status, *direction);
    if(joy_button(1) &&  *status == 0 && strcmp(display->name, "pause_menu") == 0){
        *status = 1;
        if(display->parent != NULL){
            display = display->parent;
        }
    }
    if(joy_button(0) &&  *status == 0 && strcmp(display->name, "pause_menu") == 0){
        *status = 1;
        if(display->parent != NULL){
            display = display->parent->parent;
            display->select = 2;
        }
    }
    /*if(lives == 0 && *status == 0 && && strcmp(display->name, "game") == 0){
        *status = 1;
        if(display->parent != NULL){
            display = display->parent;
        }      
    }*/

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
        oled_pos(7, 1);
        //oled_sram_printString("Score: 0");
        oled_pos(2, 1);
        //oled_sram_printString("Lives: ###");
        if(lives == 3){
            oled_sram_printString("Lives: ###");
        }
        else if(lives == 2){
            oled_sram_printString("Lives: ##");
        }
        else if(lives == 1){
            oled_sram_printString("Lives: #");
        }
        else if(lives == 0){
            oled_sram_clear();
            oled_pos(3, 3);
            oled_sram_printString("You lost!");
            oled_pos(5,4);
            oled_sram_printString("Score:");
            char result[50];
            sprintf(result, "%d", points);
            //printf("hihihihi_ %s\n\r", result);
            oled_sram_printString(result);
            oled_pos(7, 0);
            oled_sram_printString("Ok");
        }
        display->select = -1;
    }

    else if (strcmp(display->name, "pause_menu") == 0){
        int i = 0;
        oled_sram_clear();
        oled_pos(3, 3);
        oled_sram_printString("Quit game?");
        oled_pos(7, 0);
        oled_sram_printString("Yes");
        oled_pos(7, 14);
        oled_sram_printString("No");
        display->select = -1;
    }
    else if (strcmp(display->name, "hs") == 0){
        oled_sram_clear();
        oled_pos(0, 0);
        oled_sram_printString("--High  Scores--");
        oled_pos(1, 1);
        oled_sram_printString("1. Ola-------15");
        oled_pos(2, 1);
        oled_sram_printString("2. Petter----12");
        oled_pos(7, 1);
        oled_sram_printString("Back");
        oled_pos(7,0);
        oled_sram_printString('@');
        display->select = -1;
    }
    else if (strcmp(display->name, "options") == 0){
        oled_sram_clear();
        oled_pos(0, 0);
        oled_sram_printString("----Settings----");
        oled_pos(1,2);
        oled_sram_printString("Music");
        oled_pos(2, 2);
        oled_sram_printString("Difficulty");
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
    if(strcmp(direction, "RIGHT") == 0 && *status == 0 && display->name != "game"){
        *status = 1;
        if(display->child[display->select] != NULL){
            display = display->child[display->select-1];
        }
        }
    else if (strcmp(direction, "LEFT")==0 && *status == 0 && display->name != "game"){
        *status = 1;
        if(display->parent != NULL){
            display = display->parent;
        }
    }
    return display;
}

int button_select(struct screen* display){
    if (joy_button(1) == 2){
        //printf("select: %d\r\n", main_menu.select);
        return (display->select);
        
    }
    return 0;
}