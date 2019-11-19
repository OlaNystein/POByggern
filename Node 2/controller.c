#include "controller.h"
#include "TWI_Master.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "DAC.h"
#include <avr/interrupt.h>
#include "uart.h"
#include <math.h>
#include "CAN.h"
#include "solenoid.h"
#define F_CPU 16000000

volatile int max, min, pos, u, error;
volatile int lastError = 0;
volatile int sumError = 0;
static double Kp = 6.5;
static double Ki = 1;
static double T = 0.01;
static double Kd = 0.005;
volatile double y;
//static double Kp = 0.5;
/*static double Ki = 0;
static double T = 0.01;
static double Kd = 0;*/
volatile int solenoid_shot = 0;
volatile int counter = 0;


int controller_init(void){
    DDRH |= (1 << PH4);
    PINH = ~(1 << PH4);
    DDRH |= (1 << PH1);
    //DDRH &= ~(1 << PH1);
    DDRH |= (1<<PH3) | (1<<PH5) | (1 << PH6);
    PORTH |= (1 << PH4);
    
    
    
    printf("Controller initialized\r\n");
    return 0;
}

void timer_init(void){
    TCCR5B |= (1 << WGM32); // CTC

    //TCCR5A |= (1 << COM5A0);

    OCR5A = 77; 

    TCCR5B |= (1 << CS52) | (1 << CS50);

    TIMSK5 |= (1 << OCIE5A);

    TIFR5 |= (1 << OCF5A);
}

int two2dec(uint8_t twos) {
    if ((twos & 0x80) == 0){
        return twos;
    }
    return (uint8_t)(~(twos - 0x01))* -1;
}

void joy_to_voltage(uint8_t joy){
    int in = two2dec(joy);
    if(in >10){
        PORTH |= (1 << PH1);
        DAC_send(in);
    }
    else if (in < -10){
        PORTH &= ~(1 << PH1);
        DAC_send(in*(-1));
    }
    else {
        DAC_send(0);
    }
}

void joy_to_voltage2(int joy){
    if(joy > 255){
        joy = 255;
    }
    if(joy < -255){
        joy = -255;
    }
    if(joy > 0){
        PORTH |= (1 << PH1);
        DAC_send(joy);
    }
    else if (joy < 0){
        PORTH &= ~(1 << PH1);
        DAC_send(joy*(-1));
    }
    else {
        DAC_send(0);
    }
}

int controller_get_encoder_data(void){
    int data;
    cli();
    DDRK = 0x00;
    PORTH &= ~(1<<PH5); //enable output of encoder
    PORTH &= ~(1<<PH3); //set SEL low to read high byte
    _delay_ms(20);
    data = (PINK <<8);
    PORTH |= (1<<PH3); //set SEL HIGH to read low byte
    _delay_ms(20);
    data |= PINK;
    //PORTH &= ~(1<<PH6); //toggle reset
    _delay_ms(20);
    PORTH |= (1<<PH6);
    PORTH |= (1 << PH5); //output disable
    sei();

    return data;
}



void calibrate_encoder(void){
    PORTH &= ~(1 << PH1);
    for(int i = 0; i<10000; i++){
    DAC_send(100);
    }
    min = -controller_get_encoder_data();
    PORTH |= (1 << PH1);
    for(int i = 0; i<10000; i++){
    DAC_send(100);
    }
    max = -controller_get_encoder_data();
    pos = max;
    printf("min: %d, max: %d, pos: %d\r\n", min, max, pos);
    DAC_send(0);
}

int gety(void){
    return sumError;
}

int getu(void){
    return error;
}

int getpos(void){
    return pos;
}


void PID(){
    if(get_CAN().ID == 1){
        //cli();
        y = 100*((double)pos/((double)max-(double)min));
        error = get_CAN().data[3] - (int)y;
        /*if(get_CAN().data[5] == 2){
            Kp = 4.5;
            Ki = 0;
            T = 0.01;
            Kd = 0;
        }
        else{
            Kp = 1.5;
            Ki = 2.5;
            T = 0.01;
            Kd = 0.001;
        }*/

        if(error < 0 && sumError > 0){
            sumError = 0;
        }else if(error > 0 && sumError < 0){
            sumError = 0;
        }
        if(sumError >  200){
            sumError = 200;
        } else if (sumError < -200){
            sumError = 200;
        }
        
        sumError += error;

        double derivator = (Kd/T)*(error-lastError);

        lastError = error;
        
        
        //printf("%d\t%d\t%d\r\n", error, pos, sumError);
        u = Kp*error + sumError*Ki*T + derivator;
        
        pos = -controller_get_encoder_data();
        if(pos > max){
            pos = max;
        }else if(pos < min){
            pos = min;
        }
    }
    joy_to_voltage2(u);
    //_delay_ms(10);
    //sei();
}

ISR(TIMER5_COMPA_vect){
    PID();
    TIFR5 |= (1 << OCF5A);
    //counter++;
}