#ifndef JOYSTICK_H_
#define JOYSTICK_H_

uint8_t mid_x, mid_y;

typedef struct {
    int8_t x;
    int8_t y;
    char *direction;
} joy_position;

typedef struct {
    uint8_t left;
    uint8_t right;
} slider_position;

#endif

int joy_init(void);
int joy_cal(void);
int joy_button(int button);
joy_position joy_getPos(void);
joy_position joy_getDir(void);
slider_position joy_getSliderPos(void);

