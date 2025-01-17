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

static int max, min, pos;
static int lastError = 0;
static int sumError = 0;
static double Kp = 1.5;
static double Ki = 3.5;
static double T = 0.01;
static double Kd = 0.001;
//static double Kp = 0.5;
/*static double Ki = 0;
static double T = 0.01;
static double Kd = 0;*/
volatile int solenoid_shot = 0;
volatile int counter = 0;


int controller_init(void){
    DDRH |= (1 << PH4);
    DDRH |= (1 << PH1);
    //DDRH &= ~(1 << PH1);
    DDRH |= (1<<PH3) | (1<<PH5) | (1 << PH6);
    PORTH |= (1 << PH4);
    
    
    
    printf("Controller initialized\r\n");
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
}

void PID(message m){
    double y = 100*((double)pos/((double)max-(double)min));
    int error = m.data[3] - (int)y;

    if(error < 0 && sumError > 0){
        sumError = 0;
    }else if(error > 0 && sumError < 0){
        sumError = 0;
    }
    
    sumError += error;

    double derivator = (Kd/T)*(error-lastError);

    lastError = error;
    
    
    //printf("%d\t%d\t%d\r\n", error, pos, sumError);
    int u = Kp*error + sumError*Ki*T + derivator;
    joy_to_voltage2(u);
    pos = -controller_get_encoder_data();
    if(pos > max){
        pos = max;
    }else if(pos < min){
        pos = min;
    }
    //printf("slider: %d min: %d, max: %d, pos: %d, error: %d, sumError: %d\r\n", m.data[3], min, max, pos, error, sumError);
    counter++;
    if(m.data[4] == 1){
        solenoid_shot = 1;
        if(counter >= 100){
            //solenoid_pulse();
            counter = 0;
            solenoid_shot = 0;
        }
    }
    
    _delay_ms(10);
}

