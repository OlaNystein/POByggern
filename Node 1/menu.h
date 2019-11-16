#ifndef MENU_H_
#define MENU_H_


typedef struct screen {
    char* name;
    struct screen *parent;
    struct screen * child[3];
    int8_t select;
} screen;

struct screen* init_menu(void);
struct screen* draw_screen(struct screen* display, char* direction, int* status, int lives, int points);
int button_select(struct screen* display);

#endif