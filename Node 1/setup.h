#ifndef _SETUP_H_
#define _SETUP_H_

#define FOSC 4915200UL
#define BAUD 9600
#define MYUBURR FOSC/16/BAUD-1

#define set_bit(reg,bit) (reg |= (1<<bit))
#define clear_bit(reg,bit) (reg &= ~(1<<bit))
#define test_bit(reg,bit) (reg & (1 << bit))

#endif