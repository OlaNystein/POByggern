#include <avr/io.h>
#include "util/delay.h"
#include "MCP.h"
#include "CAN.h"
#include "setup.h"
#include <avr/interrupt.h>

uint8_t rxF = 0;

int CAN_init(void){

    MCP_init();

    MCP_bit_modify(MCP_RXB0CTRL, 0b01100000, 0xFF); //recieve filter off

    MCP_bit_modify(MCP_CANCTRL,MODE_MASK, MODE_NORMAL);

    //SPI_select();

    MCP_bit_modify(MCP_CANINTE, 0x01, 0x01);

    return 0;
}

int CAN_err(void){
    uint8_t err = MCP_read(MCP_TXB0CTRL);

    if (test_bit(MCP_TXB0CTRL, 4)){
        return -1; //transmission error
    }
    /*if (test_bit(MCP_TXB0CTRL, 5)){
        return -2; //message lost arbitration
    }*/
    return 0;
}

int CAN_send(message* m){

    if(CAN_sendcomplete()){
        printf("Sending message \n\r");
        MCP_write(MCP_TXB0SIDH,(int8_t)(m->ID >> 3));
        MCP_write(MCP_TXB0SIDL, (int8_t)(m->ID << 5)); //puts 8-bit address in the right registers

        MCP_write(MCP_TXB0DLC,(0x0F) & (m->length)); //set datalength, 0-8

        for (uint8_t i = 0; i < m->length; i++){
            MCP_write(MCP_TXB0D0+i, m->data[i]);
        }
        MCP_REQTS(1); //requesting to send via TXB0
        

    }
    else{
        if(CAN_err() < 0){
            return  -1;
        }
    }

    printf("Content of IDH: %x \n\r",  MCP_read(MCP_TXB0SIDH));
    printf("Content of IDL: %x \n\r",  MCP_read(MCP_TXB0SIDL));
    printf("Content of DL: %x \n\r",  MCP_read(MCP_TXB0DLC));
    printf("Content of D[0]: %x \n\r",  MCP_read(MCP_TXB0D0));

    //MCP_bit_modify(MCP_CANINTF, 0x01, 0x00);
    
    return 0;
}

int CAN_sendcomplete(void){
    if (test_bit(MCP_TXB0CTRL, 3)){ // checking if TXREQ bit in TXB0 register is 1 -> buffer is currently pending transmition
        return 0;
    }
    else{
        return 1;
    }
}



message CAN_recieve(void){
    message m;
    printf("rxF after send: %d\n\r", rxF);



    printf("REC of IDH: %x \n\r",  MCP_read(MCP_RXB0SIDH));
    printf("REC of IDL: %x \n\r",  MCP_read(MCP_RXB0SIDL));
    printf("REC of DL: %x \n\r",  MCP_read(MCP_RXB0DLC));
    printf("REC of D[0]: %x \n\r",  MCP_read(MCP_RXB0D0));
    
    //checks if a message is pending, set to 1 by interrupt
    if (rxF == 1){ 

        m.ID = ((MCP_read(MCP_RXB0SIDH) << 3) | (MCP_read(MCP_RXB0SIDL) >> 5));

        m.length = (MCP_read(MCP_RXB0DLC) & 0x0F);

        for(uint8_t i = 0; i < m.length; i++){
            m.data[i] = MCP_read(MCP_RXB0D0+i);
        }
        printf("message recieved\n\r");
        rxF = 0; //message recieved
    }else{
        m.ID = -1; // message not received
    }
    

    return m;

}

int CAN_interrupt(void){
    MCP_bit_modify(MCP_CANINTF, 0x00, 0);
    MCP_bit_modify(MCP_CANINTF, 0x01, 0);
    rxF = 1;
    return 0;
}


ISR(INT0_vect) {
    printf("Interrupt!!!\n\r");
    CAN_interrupt();
}