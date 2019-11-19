#ifndef CAN_H_
#define CAN_H_

typedef struct message_t{ 
    uint8_t data[8];
    int length;
    int ID;
} message;


int CAN_init(void);
int CAN_send(message* m);
int CAN_sendcomplete(void);
message CAN_recieve(void);
message get_CAN(void);
int CAN_interrupt(void);
int CAN_err(void);
#endif