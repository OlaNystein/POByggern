#ifndef SPI_H_
#define SPI_H_

int SPI_init(void);
int SPI_transmit(char data);
uint8_t SPI_recieve(void);
int SPI_select(void);
int SPI_deselect(void);


#endif
