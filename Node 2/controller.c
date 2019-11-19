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

static int max, min, pos, u;
static int lastY;
static int lastDerivative = 0;
static int sumError = 0;
static double Kp = 0;
static double Ki = 0;
static double T = 0.01;
static double Kd = 0.000;
volatile int solenoid_shot = 0;
volatile int counter = 0;


int controller_init(void){
    DDRH |= (1 << PH4);
    DDRH |= (1 << PH1);
    //DDRH &= ~(1 << PH1);
    DDRH |= (1<<PH3) | (1<<PH5) | (1 << PH6);
    PORTH |= (1 << PH4);

    return 0;
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

void timer_init(void){
    TCCR5B |= (1 << WGM32); // CTC

    //TCCR5A |= (1 << COM5A0);

    OCR5A = 77; 

    TCCR5B |= (1 << CS52) | (1 << CS50);

    TIMSK5 |= (1 << OCIE5A);

    TIFR5 |= (1 << OCF5A);
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

}

void set_difficulty(message m){
    if (m.data[5] == 1){
        //difficulty normal
        Kp = 11;
        Ki = 2;
        Kd = 2;
    }
    else if (m.data[5] == 2){
        //difficulty hard
        Kp = 1;
        Ki = 10;
        Kd = 0.002;
    }
}

void PID(){
    message msg = get_CAN();
    if(msg.ID == 1){
        double y = 100*((double)pos/((double)max-(double)min));
        int error = msg.data[3] - (int)y;
        if(error < 0 && sumError > 0){
            sumError = 0;
        }else if(error > 0 && sumError < 0){
            sumError = 0;
        }
        if(sumError > 1000){
            sumError = 1000;
        }
        if(sumError < -1000){
            sumError = -1000;
        }
        
        sumError += error;

        double cutoff = 10;
        double RC = 1.0/(cutoff*6.28);
        double derivative = RC/(RC + T)*(lastDerivative + y - lastY);
        lastY = y;


        lastDerivative = derivative;
        
        u = Kp*error + sumError*Ki*T - (Kd/RC)*derivative;
        
        pos = -controller_get_encoder_data();
        if(pos > max){
            pos = max;
        }else if(pos < min){
            pos = min;
        }

    }    
    joy_to_voltage2(u);
}

ISR(TIMER5_COMPA_vect){
    PID();
    TIFR5 |= (1 << OCF5A);
}