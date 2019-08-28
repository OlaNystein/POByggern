#ifndef UART_H_
#define UART_H_
#include <stdio.h>
#include <stdint.h>
#define FOSC 4915200UL
#define BAUD 9600
#define MYUBURR FOSC/16/BAUD-1

void UART_Init(unsigned int ubrr);
void UART_Transmit(unsigned char data);
unsigned char UART_Recieve(void);

#endif //UART_H_

