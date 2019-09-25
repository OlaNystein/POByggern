#ifndef MENU_H_
#define MENU_H_
#include "joystick.h"

struct screen {
    char* name;
    struct screen *parent;
    struct screen * child[3];
    int8_t select;
};

struct screen* init_menu(void);
void draw_screen(struct screen* display, char* direction);
int button_select(struct screen* display);

#endif