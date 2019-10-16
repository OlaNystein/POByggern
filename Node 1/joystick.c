#include <avr/io.h>
#include <stdio.h>
#include "ADC.h"
#include "joystick.h"
#include "setup.h"

int joy_init(void){
    ADC_init();

    DDRB &= ~(1<<PB0) & ~(1<<PB1) & ~(1<<PB2);
    PORTB |= (1<<PB0) | (1<<PB1) | (1<<PB2);

    joy_cal();

    return 0;
}

int joy_cal(void){
    mid_x = ADC_read(1);
    mid_y = ADC_read(0);
    return 0;
}

int joy_button(int button){
    switch (button){
        case 0: //left button
            if(test_bit(PINB, PB0)) {return 1;}
            break;
        case 1: //right button
            if(test_bit(PINB, PB1)) {return 2;}
            break;
        case 2: //joybutton
            if(test_bit(PINB, PB2)) {return 3;}
            break;
        default:
            break;
    }
    return 0;
}

joy_position joy_getPos(void){
    joy_position pos;
    uint8_t x, y;

    x = ADC_read(1);
    y = ADC_read(0);

    if(x>mid_x){
        pos.x = 100*(x-mid_x)/(256-mid_x);
    }
    else if(x<mid_x){
        pos.x = 100*(x-mid_x)/mid_x;
    }
    else{
        pos.x = 0;
    }

    if(y>mid_y){
        pos.y = 100*(y-mid_y)/(256-mid_y);
    }
    else if(y<mid_y){
        pos.y = 100*(y-mid_y)/mid_y;
    }
    else{
        pos.y = 0;
    }
    return pos;
}

joy_position joy_getDir(void){
    joy_position pos;

    pos = joy_getPos();

    if(pos.x < 45){
        pos.direction = "LEFT";
    } else if(pos.x > 55){
        pos.direction = "RIGHT";
    }

    if(pos.y < 30){
        pos.direction = "DOWN";
    } else if(pos.y > 70){
        pos.direction = "UP";
    }

    if(pos.x >= 45 && pos.x <= 55 && pos.y >= 45 && pos.y <= 55){
        pos.direction = "NEUTRAL";
    }
    return pos;
}

slider_position joy_getSliderPos(void){
    slider_position pos;
    uint8_t l, r;
    r = ADC_read(2);
    l = ADC_read(3);

    pos.left = 100*l/256;
    pos.right = 100*r/256;

    return pos;
}

