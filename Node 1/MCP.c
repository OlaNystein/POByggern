#include <avr/io.h>
#include "util/delay.h"
#include "MCP.h"
#include "SPI.h"

int MCP_init(void){
    
    SPI_init();
    
    MCP_reset();
    
    return 0;
}

int MCP_reset(void){
    SPI_select();

    SPI_transmit(MCP_RESET);
    
    SPI_deselect();

    _delay_ms(200);
    return 0;
}

uint8_t MCP_read(uint8_t adress){
    SPI_select();
    
    SPI_transmit(MCP_READ);
    SPI_transmit(adress);
    
    uint8_t data = SPI_recieve();
    
    SPI_deselect();
    
    return data;
}

int MCP_write(uint8_t adress, uint8_t data){
    SPI_select();

    SPI_transmit(MCP_WRITE);
    SPI_transmit(adress);
    SPI_transmit(data);

    SPI_deselect();
    return 0;
}

int MCP_REQTS(uint8_t tx){
    if(tx < 8){
        tx = tx | MCP_RTS;
    }
    else{
        tx = MCP_RTS;
    }
    SPI_select();

    SPI_transmit(tx);

    SPI_deselect();

    return 0;
}

uint8_t MCP_RS(void){
    SPI_select();
    SPI_transmit(MCP_READ_STATUS);
    uint8_t res = SPI_recieve();
    SPI_deselect();

    return res;
}

int MCP_bit_modify(uint8_t adress, uint8_t mask, uint8_t data){
    SPI_select();
    SPI_transmit(MCP_BITMOD);
    SPI_transmit(adress);
    SPI_transmit(mask);
    SPI_transmit(data);
    SPI_deselect();
    return 0;
}

