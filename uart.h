#ifndef UART_H_
#define UART_H_
#include <stdio.h>
#include <stdint.h>
#define FOSC 4915200UL
#define BAUD 9600
#define MYUBURR FOSC/16/BAUD-1

void UART_Init(unsigned int ubrr);
void UART_Transmit(unsigned char data, FILE *f);
unsigned char UART_Recieve();
int UART_print(char * string);

#endif //UART_H_

