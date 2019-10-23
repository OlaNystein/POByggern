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