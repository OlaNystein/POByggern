#include "TWI_Master.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "DAC.h"

int DAC_init(void){
    TWI_Master_Initialise();
    DDRD |= (1 << PD0) | (1 << PD1); //SCL and SDA as outputs (from atmega to I/O)
    return 0;
}

int DAC_send(uint8_t m){

    unsigned char size = 3;
    unsigned char* msg = malloc(3*sizeof(char));

    msg[0] = 0x50; //slave address 0
    msg[1] = 0x00; //
    msg[2] = m; //data

    TWI_Start_Transceiver_With_Data(msg, size);
    return 0;
}