#include <avr/io.h>
#include "util/delay.h"
#include "setup.h"
#include "CAN.h"



int pwm_init(void){
    TCCR1A |= (1 << COM1A1) | (1 << COM1A0) | (1 << WGM11); //setting OCnA/OCnB/OCnC on compare match and enables fast pwm
    TCCR1B |= (1 << WGM12) | (1 << WGM13); //enables fast pwm
    TCCR1B |= (1 << CS12); // setting clock select to clk/256 from prescaler
    DDRB |= (1 << PINB5);

    ICR1 = 1250; // pwm period of 20ms

    OCR1A = 1157; // setting pulse width to 1.5
   
    return 0;

}

unsigned int pwm_pulse(unsigned int servo, message m){
    //printf("ID: %d\tDATA:%d\t%d\t%d\n\r", m.ID, m.data[0], m.data[1]);
    if ((m.data[0] < 101 && m.data[0] > 5)&&(servo >= 1126  && servo <= 1181)){
        servo++;
    }
    if ((m.data[0] < 250 && m.data[0] > 155)&&(servo >= 1130  && servo <= 1185)){
        servo--;
    }
    if(servo >= 1126  && servo <= 1185) {
        OCR1A = servo;
    }
    return servo;
}

