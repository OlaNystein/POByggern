/*#include <avr/io.h>
#define F_CPU 4915200UL
#include "util/delay.h"
*/
#include <stdlib.h>
void SRAM_test(void);
int SRAM_init(void);
int SRAM_write(unsigned int address, unsigned int data);
unsigned int SRAM_read(unsigned int address);