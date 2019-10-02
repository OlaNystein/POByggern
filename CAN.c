#include <avr/io.h>
#include "util/delay.h"
#include "MCP.h"
#include "CAN.h"



int CAN_init(void){

    mcp_init();

    MCP_bit_modify(MCP_RXB0CTRL, 0b01100000, 0xFF); //recieve filter off

    MCP_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK);

    MCP_bit_modify(MCP_CANINTE, 0x01, 0x01);

}