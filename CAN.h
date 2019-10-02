#ifndef CAN_H_
#define CAN_H_

typedef struct message{ 
    uint8_t data[8];
    int length;
    int ID;
} message;


#endif